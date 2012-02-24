 
#ifndef REDSHIFTJOB_H
#define REDSHIFTJOB_H

#include <Plasma/ServiceJob>
#include "redshiftcontroller.h"

class RedshiftJob : public Plasma::ServiceJob
{
    Q_OBJECT

    public:
        RedshiftJob(RedshiftController *controller, const QString &operation, QMap<QString, QVariant> &parameters, QObject *parent = 0);
        ~RedshiftJob();

    protected:
        void start();
    private:
        RedshiftController *m_redshiftController;
};

#endif // REDSHIFTJOB_H
