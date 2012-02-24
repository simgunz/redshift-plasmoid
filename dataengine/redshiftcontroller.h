/**************************************************************************
*   Copyright (C) 2012 by Simone Gaiarin <simgunz@gmail.com>              *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
**************************************************************************/

#ifndef REDSHIFTCONTROLLER_H
#define REDSHIFTCONTROLLER_H

#include <KProcess>
#include <QDebug>
#include "redshiftsettings.h"

class RedshiftController : public QObject
{
    Q_OBJECT
    public:
        enum RedshiftState {
        Running,
        Paused,
        Stopped
        };
        RedshiftController();
        ~RedshiftController();
        void start();
        void toggle();
        void stop();
        void restart();
        void readConfig();
    private:
        KProcess *m_process;
        bool m_state;
        float m_latitude;
        float m_longitude;
        int m_dayTemp;
        int m_nightTemp;
        float m_gammaR;
        float m_gammaG;
        float m_gammaB;
        bool m_smooth;
        bool m_autolaunch;
        
    signals:
        void statusChanged(int status);
};

#endif // REDSHIFTCONTROLLER_H