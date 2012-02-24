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