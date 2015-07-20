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

#ifndef REDSHIFTOSDWIDGET_H
#define REDSHIFTOSDWIDGET_H

class QPixmap;
class QTimer;

namespace Plasma
{
    class Label;
}

/*!
 * \class RedshiftOSDWidget
 *
 * The RedshiftOSDWidget class provides an On Screen Display (OSD)
 * dialog showing the screen color temperature.
 */
class RedshiftOSDWidget : public QDialog
{
    Q_OBJECT

public:

    /*!
     * Default constructor.
     *
     * \param parent The QWidget this OSD is parented to.
     */
    RedshiftOSDWidget(QWidget *parent = 0);

    /*!
     * Recommended widget size as a function of the icon size.
     *
     * \returns Recommended widget size.
     */
    virtual QSize sizeHint() const;

    /*!
     * Shows the OSD and automatically hides it after 2 seconds.
     */
    void activateOSD();

    /*!
     * Sets the color temperature to display.
     *
     * \param temperature The color temperature in Kelvin.
     */
    void setCurrentTemperature(int temperature);

private:

    //! The icon displayed in the OSD.
    QLabel *m_iconLabel;

    //! Displays the current color temperature in Kelvin.
    QLabel *m_temperatureLabel;

    //! OSD auto-hide timer.
    QTimer *m_autoHideTimer;

    //! Redshift pixmap icon.
    QPixmap m_redshiftPixmap;
};

#endif
