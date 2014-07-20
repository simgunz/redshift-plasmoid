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
 * \file redshiftapplet.h
 *
 * Contains the interface of the RedshiftApplet class.
 */

#ifndef REDSHIFTAPPLET_H
#define REDSHIFTAPPLET_H

#include "ui_redshift.h"
#include "ui_activities.h"

#include <QGraphicsGridLayout>
#include <QGraphicsSceneWheelEvent>

#include <KIcon>

#include <Plasma/Applet>
#include <Plasma/DataEngine>
#include <Plasma/IconWidget>
#include <Plasma/ToolTipContent>

class RedshiftOSDWidget;

/*!
 * The RedshiftApplet class provides a native KDE interface to redshift.
 *
 * RedshiftApplet is a plasma widget that allows the user to interact with the redshift daemon.
 * By clicking on the icon of the widget it's possible to start and stop redshift.
 * By scrolling the mouse wheel over the icon it is possible to set a fixed color temperature, bringing
 * redshift in "Manual" mode, i.e, the temperature won't be adjusted automatically anymore.
 * To get back to the auto-adjustment mode the user have to click on the widget icon.
 * When the temperature is adjusted manually an OSD that display the current color temperature is shown.
 * By clicking with the right mouse button the user can access the settings dialog.
 * When the mouse passes over the widget icon a tooltip is shown to display the current redshift status
 * and a simple help.
 */
class RedshiftApplet : public Plasma::Applet
{
    Q_OBJECT

public:

    /*!
     * Default constructor.
     *
     * \param parent The QObject this applet is parented to
     * \param args Extra arguments
     */
    RedshiftApplet(QObject *parent, const QVariantList &args);

    /*!
     * Deconstructor.
     */
    ~RedshiftApplet();

    /*!
     * Applet initialization.
     *
     * This method is called once the applet is loaded. It loads the main icon of the
     * widget, sets the tooltip, connects the widget to the redshift dataEnginge
     * and connects all the signals and slots.
     */
    void init();

public slots:

    /*!
     * Called when the dataEngine reports some changes in its state.
     *
     * The icon and the tooltip are updated corresponding to the dataEngine status.
     * The widget status is set to passive (icon hidden in the tray menu) when
     * redshift is in "Auto" mode, while is set to active when redshift is
     * in "Manual" mode.
     *
     * \param sourceName The name of the source that has called the method.
     * \param data A QVariantMap containing the data the source passed to the method.
     */
    void dataUpdated(const QString &sourceName, const Plasma::DataEngine::Data &data);

    /*!
     * Makes a call to the redshift dataEnginge service to toggle redshift on/off.
     */
    void toggle();

    /*!
     * Show an OSD showing the current color temperature.
     *
     * \param temperature The color temperature in Kelvin.
     */
    void showRedshiftOSD(int temperature);

protected:

    /*!
     * Create a settings dialog for the widget.
     *
     * The settings dialog can be accessed from the right click menu.
     *
     * \param parent The KConfigDialog this config page is parented to.
     */
    void createConfigurationInterface(KConfigDialog *parent);

    /*!
     * Called when applet configuration values have been changed.
     *
     * Makes a call to the redshift dataEnginge service to restart redshift in
     * order to apply the new settings. The screen color will undergo
     * an out-in color transition.
     *
     * \warning This is not the desired behavior, but is the only possible solution
     * since redshift won't allow to change the settings once running.
     */
    void configChanged();

protected slots:

   /*!
    * Called when the mouse wheel is scrolled over the widget icon.
    *
    * Makes a call to the redshift dataEnginge service to increase/decrease the
    * color temperature of the screen. This action will bring redshift in "Manual"
    * mode, so that it won't adjust the color temperature during the day
    * but it will keep it fixed.
    */
  virtual void wheelEvent(QGraphicsSceneWheelEvent *event);

private slots:

    /*!
     * Saves the redshift behavior for each activities in the configuration file
     * when the user finishes editing the settings.
     *
     * For each activity redshift can be set in the state "Always On", "Always Off"
     * or "Manual".
     */
    void configAccepted();

private:

    //! The button that constitute the body of the widget.
    Plasma::IconWidget *m_button;

    //! The widget tooltip.
    Plasma::ToolTipContent m_tooltip;

    //! The main layout of the widget
    QGraphicsGridLayout *m_layout;

    //! Redshift parameters configuration ui page.
    Ui::RedshiftConfig m_redshiftUi;

    //! Redshift activities configuration ui page.
    Ui::ActivitiesConfig m_activitiesUi;

    //! Pointer to the Redshift dataengine.
    Plasma::DataEngine *m_engine;

    //! Pointer to the Redshift OSD object.
    RedshiftOSDWidget *m_redshiftOSD;
};

#endif
