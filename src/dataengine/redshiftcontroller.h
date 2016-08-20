/************************************************************************
* Copyright (C) 2012 by Simone Gaiarin <simgunz@gmail.com>              *
*                                                                       *
* This program is free software; you can redistribute it and/or modify  *
* it under the terms of the GNU General Public License as published by  *
* the Free Software Foundation; either version 3 of the License, or     *
* (at your option) any later version.                                   *
*                                                                       *
* This program is distributed in the hope that it will be useful,       *
* but WITHOUT ANY WARRANTY; without even the implied warranty of        *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
* GNU General Public License for more details.                          *
*                                                                       *
* You should have received a copy of the GNU General Public License     *
* along with this program; if not, see <http://www.gnu.org/licenses/>.  *
************************************************************************/

/*!
 * \file redshiftcontroller.h
 *
 * Contains the interface of the RedshiftController class.
 */

#ifndef REDSHIFTCONTROLLER_H
#define REDSHIFTCONTROLLER_H

#include <KProcess>

#include <Plasma/DataEngine>

/*!
 * The RedshiftController class is a wrapper around a redshift KProcess that allows to change all
 * the redshift parameters while it's running, and makes redshift plasma activity aware.
 *
 * RedshiftController is a wrapper around a redshift daemon. The redshift daemon can only be paused/resumed
 * by sending the SIGUSR1 but doesn't allow to change its parameters once launched. This wrapper adds this
 * kind of functionality. Redshift is launched through a KProcess and every time a parameter needs to be changed,
 * the KProcess is stopped and relaunched with the new parameters.
 *
 * Moreover it provides some wrapper methods to start/stop/pause redshift by sending the SIGUSR1 in a proper way.
 *
 * Finally RedshiftController adds a Plasma Activity aware functionality to redshift. In particular every time the user
 * changes activity the controller checks the user preferences and sets redshift to the state corresponding to
 * the current activity.
 * The allowed states are:
 * - Auto, redshift can be toggled on/off. The default state at login depends on the Autostart flag.
 * - Always enabled, redshift is forced to be enabled and can't be turned off (not very useful)
 * - Always disabled, redshift is forced to be disabled and can't be turned on (useful for activities where redshift
 * is undesirable, like an activity to watch movie or to do graphic editing)
 */

class RedshiftController : public QObject
{
    Q_OBJECT

public:

    /*!
     * Enumerates the possible states of RedshiftController.
     *
     * The possible values of the state are:
     * - Undefined
     * - Running, the redshift process is running and is adjusting the screen color temperature continuously
     * - RunningManual, the redshift process is not running, and the screen color temperature
     * has been set to a fixed value
     * - Stopped, the redshift process is not running, and the screen color temperature is the system default one
     */
    enum RedshiftState {
        Undefined,
        Stopped,
        Running,
        RunningManual
    };

    /*!
     * Enumerates the modes RedshiftController can operate.
     */
    enum RunMode {
        Auto,
        AlwaysOn,
        AlwaysOff
    };

    /*!
     * Default constructor.
     *
     * The constructor connects the controller to the dbus object dbus org.kde.redshift in order to receive
     * the readyForStart signal emitted by redshiftAutostartEnabler, which triggers the setReadyForStart method that
     * enables all the controller functionality.
     * Moreover the controller is connected to the Plasma Activities dataengine 'Status' source so that every time
     * the user changes activity the dataUpdated method is invoked and the state of redshift can be
     * changed accordingly to the current activity settings.
     */
    RedshiftController();

    /*!
     * Default deconstructor.
     *
     * If redshift is in Manual mode (so the process isn't running), resets the screen color temperature
     * to the default value by invoking redshift with the -x parameter. Otherwise terminates the redshift
     * running process with the method terminate, so that a smooth transition is performed (if enabled in the settings).
     */
    ~RedshiftController();

    /*!
     * Access method to the current redshift state.
     *
     * \returns The current redshift state.
     */
    RedshiftState state();

    /*!
     * Access method to the Manual mode screen color temperature.
     *
     * If redshift is in Manual mode the temperature (manually set) is returned otherwise the method returns 0.
     *
     * \todo Let return the current color temperature also when redshift is in Auto mode.
     * Since redshift 1.7 it was not possible to retrieve the current temperature from the redshift daemon.
     * From redshift 1.8 it's possible to retrieve the current temperature from the shell by calling redshift with the
     * -v parameter.
     *
     * \returns The current screen color temperature if in Manual mode, zero otherwise.
     */
    int currentTemperature();

    /*!
     * Switches to Manual mode and increases/decreases the screen color temperature by 100 Kelvin.
     *
     * If the controller is in Auto mode first brings it in Manual mode and sets the screen color temperature to the
     * DefaultManualTemperature value, then increases/decreases the screen color temperature. If the controller
     * is already in Manual mode it just increases/decreases the temperature.
     *
     * The redshift process is instantly killed if it's running, then the color temperature is set to a fixed value
     * by launching redshift with the -x parameter. After the temperature has been set the redshift process exits.
     *
     * \param increase If true the screen color temperature is increased otherwise it's decreaseds.
     */
    void setTemperature(bool increase);

    /*!
     * Toggles the state of redshift.
     *
     * If redshift is in Auto mode this method toggles the state from Running to NotRunning and vice versa, whilst
     * if it's in Manual mode (the state is RunningManual) it resets the screen color temperature,
     * sets the mode to Auto, and executes redshift so that the final state is Running.
     */
    void toggle();

    /*!
     * Restarts the redshift process to apply the new settings.
     *
     * Since the parameters cannot be changed once the redshift process is running, it needs to be
     * stopped and launched again with the new parameters.
     */
    void restart();

    //! Constant defining the minimum allowed screen color temperature.
    static const int MinTemperature;

    //! Constant defining the maximum allowed screen color temperature.
    static const int MaxTemperature;

    //! Constant defining the default screen color temperature used when redshift switch to Manual mode.
    static const int DefaultManualTemperature;

    //! Constant defining the temperature step used when increasing/decreasing the screen color temperature in Manual mode.
    static const int TemperatureStep;

signals:

    /*!
     * Emitted every time the state changes or, if in Manual mode, the temperature changes.
     *
     * This signal is caught by the RedshiftContainer in order to set the source data values.
     * The state can change if the user brings redshift in Manual mode, if he manually toggles redshift on/off,
     * and if he switches activity. The temperature changes every time the user scroll the wheel over the icon widget.
     *
     * \param state The current state.
     * \param temperature The screen color temperature in Kelvin, when redshift is in manual mode.
     */
    void stateChanged(RedshiftController::RedshiftState state, int temperature);

private slots:

    /*!
     * Sets a flag that enables the controller to execute the redshift process.
     *
     * Triggered when the dbus readyForStart is received.
     */
    void setReadyForStart();

    /*!
     * Invoked when the user changes plasma activity, changes the state of redshift accordingly to
     * the activities settings.
     *
     * As an example if the user switches to a plasma activity where the redshift activity setting is AlwaysOff,
     * the redshift process is suspended.
     *
     * \param sourceName The name of the source that has called the method.
     * \param data A QVariantMap containing the data the source passed to the method.
     */
    void dataUpdated(const QString &sourceName, const Plasma::DataEngine::Data &data);

private:

    /*!
     * Core method that switches redshift on/off.
     *
     * If toggle is true the next section performs a toggle of the state whereas
     * if toggle is false it realigns the real state with the auto state. This last option is used to:
     * - autostart redshift
     * - restores the Auto mode redshift state, after switching from Manual mode
     *
     * \param toggle If toggle is true the next section performs a toggle of the state,
     *               whereas if toggle is false it realigns the real state with the auto state.
     */
    void applyChanges(bool toggle = false);

    /*!
     * Enables redshift screen color management (the screen becomes red).
     *
     * If the redshift process is not already running it executes it,
     * otherwise it sends the SIGUSR1 signal to the process to resume it
     */
    void start();

    /*!
     * Disables redshift screen color management (the screen becomes neutral).
     *
     * If the redshift process is running it sends the SIGUSR1 signal to the process to suspend it.
     * It also resets the manual screen color temperature to the default value when redshift switch from
     * Manual mode to Auto mode.
     */
    void stop();

    /*!
     * Reads the redshift specific parameters and the activities parameters from a KconfigXT configuration.
     *
     * If redshift should be auto started on login, m_autoState is set to Running so that once applyChanges is
     * invoked it executes the redshift process (m_state is aligned to m_autoState).
     */
    void readConfig();

    //! Plasma activities dataengine.
    Plasma::DataEngine *m_activitiesEngine;

    //! The activity the user is on.
    QString m_currentActivity;

    //! The redshift process object.
    KProcess *m_process;

    //! Redshift enable flag, if true the redshift process can be executed.
    bool m_readyForStart;

    //! Actual redshift state.
    RedshiftState m_state;

    //! Stores the redshift Auto mode state, in order to restore it after switching from Manual mode.
    RedshiftState m_autoState;

    //! Current redshift operation mode.
    int m_runMode;

    //! Manual mode flag. If true redshift set a fixed color temperature and won't adjust it anymore.
    bool m_manualMode;

    //! Manual mode temperature to be set.
    int m_manualTemp;

    //! Autostart flag, if true redshift is acivated on login.
    bool m_autostart;

    // The parameters here below are the ones that should be passed to the redshift process when launched.
    // They are read from the KConfig configuration file.

    //! Flag to enable geoclue automatic location detection
    bool m_geoclueLocationEnabled;

    //! User latitude.
    float m_latitude;

    //! User longitude.
    float m_longitude;

    //! Daytime screen color temperature.
    int m_dayTemperature;

    //! Nighttime screen color temperature.
    int m_nightTemperature;

    //! Red gamma value.
    float m_gammaR;

    //! Green gamma value.
    float m_gammaG;

    //! Blue gamma value.
    float m_gammaB;

    //! Screen brightness value during the day.
    float m_dayBrightness;

    //! Screen brightness value during the night.
    float m_nightBrightness;

    //! Smooth color transition flag, if true the color transition is performed smoothly.
    bool m_smooth;

    //! Color adjstment method, can be randr or vidmode.
    QString m_renderModeString;
};

#endif // REDSHIFTCONTROLLER_H
