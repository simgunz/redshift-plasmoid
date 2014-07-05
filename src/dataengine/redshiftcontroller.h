/***************************************************************************
 *   Copyright (C) 2012 by Simone Gaiarin <simgunz@gmail.com>              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see <http://www.gnu.org/licenses/>.  *
 **************************************************************************/

#ifndef REDSHIFTCONTROLLER_H
#define REDSHIFTCONTROLLER_H

#include <KProcess>
#include <QThread>

#include <Plasma/DataEngine>

class RedshiftController : public QThread
{
    Q_OBJECT

public:
    enum RedshiftState {
        Undefined,
        Stopped,
        Running,
        RunningManual
    };
    enum RunMode {
        Auto,
        AlwaysOn,
        AlwaysOff
    };
    RedshiftController();
    ~RedshiftController();
    void toggle();
    void restart();
    RedshiftState state();
    int currentTemperature();
    void setTemperature(bool increase);

    //! Constant defining the minimum allowed screen color temperature.
    static const int MinTemperature = 1000;

    //! Constant defining the maximum allowed screen color temperature.
    static const int MaxTemperature = 9900;

    //! Constant defining the default screen color temperature used when redshift switch to manual mode.
    static const int DefaultManualTemperature = 5000;

    //! Constant defining the temperature step used when increasing/decreasing the screen color temperature in manual mode.
    static const int TemperatureStep = 100;

private Q_SLOTS:
    void dataUpdated(const QString &sourceName, const Plasma::DataEngine::Data &data);
    void setReadyForStart();
private:
    void applyChanges(bool toggle = false);
    void start();
    void stop();
    void readConfig();
    KProcess *m_process;
    /** Real redshift state */
    RedshiftState m_state;
    /** Auto mode redshift state, can be different from the real
     * state if the mode is different from manual
     */
    RedshiftState m_autoState;
    QString m_currentActivity;
    int m_runMode;
    bool m_readyForStart;
    Plasma::DataEngine *m_activitiesEngine;
    float m_latitude;
    float m_longitude;
    int m_dayTemp;
    int m_nightTemp;
    float m_gammaR;
    float m_gammaG;
    float m_gammaB;
    float m_brightness;
    bool m_smooth;
    bool m_autolaunch;
    int m_method;
    bool m_manualMode;
    int m_manualTemp;

signals:
    void stateChanged(RedshiftController::RedshiftState state, int temperature);
};

#endif // REDSHIFTCONTROLLER_H
