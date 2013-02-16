/***************************************************************************
 *   Copyright (C) 2012 by Simone Gaiarin <simgunz@gmail.com>              *
 *   Adapted from kdemultimedia/kmix/osdwidget.h                           *
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

#ifndef REDSHIFTOSDWIDGET__H
#define REDSHIFTOSDWIDGET__H

#include <Plasma/Dialog>

class QTimer;
class QPixmap;

namespace Plasma
{
class Label;
}

class RedshiftOSDWidget : public Plasma::Dialog
{
Q_OBJECT
public:
    RedshiftOSDWidget(QWidget * parent = 0);
    void setCurrentTemperature(int temperature);
    void activateOSD();
    virtual QSize sizeHint() const;

private:
    Plasma::Label *m_iconLabel;
    Plasma::Label *m_temperatureLabel;
    QTimer *m_hideTimer;
    QPixmap m_redshiftPixmap;
};

#endif
