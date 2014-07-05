/************************************************************************
* Copyright (C) 2012 by Simone Gaiarin <simgunz@gmail.com>              *
* Adapted from kdemultimedia/kmix/osdwidget.h                           *
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
 * \file redshiftosdwidget.h
 *
 * Contains the interface of the RedshiftOSDWidget class.
 */

#ifndef REDSHIFTOSDWIDGET__H
#define REDSHIFTOSDWIDGET__H

#include <Plasma/Dialog>

class QTimer;
class QPixmap;

namespace Plasma
{
class Label;
}


/*!
 * \class RedshiftOSDWidget
 *
 * The RedshiftOSDWidget class provides an On Screen Display (OSD)
 * dialog showing the current screen color temperature.
 */
class RedshiftOSDWidget : public Plasma::Dialog
{
    Q_OBJECT

public:

    /*!
     * Default constructor.
     *
     * \param parent The QWidget this OSD is parented to.
     */
    RedshiftOSDWidget(QWidget * parent = 0);

    /*!
     * Set the color temperature to display.
     *
     * \param temperature The color temperature in Kelvin
     */
    void setCurrentTemperature(int temperature);

    /*!
     * Show the OSD and automatically hide it after 2 seconds.
     */
    void activateOSD();

    /*!
     * Recommended widget size as a function of the icon size.
     *
     * \returns Recommended widget size
     */
    virtual QSize sizeHint() const;

private:

    //! The icon displayed in the OSD.
    Plasma::Label *m_iconLabel;

    //! Display the current color temperature in Kelvin.
    Plasma::Label *m_temperatureLabel;

    //! OSD autohide timer.
    QTimer *m_hideTimer;

    //! Redshift pixmap icon.
    QPixmap m_redshiftPixmap;
};

#endif
