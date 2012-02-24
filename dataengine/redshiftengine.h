 
#ifndef REDSHIFTENGINE_H
#define REDSHIFTENGINE_H

#include <Plasma/DataEngine>
#include <KSharedConfigPtr>
#include <KSharedConfig>
#include <KConfigGroup>
#include <KProcess>

#include <QStringList>
#include <QDate>
#include <QFile>
#include <QFileSystemWatcher>

#include "redshiftcontroller.h"
#include "redshiftcontainer.h"

class RedshiftEngine : public Plasma::DataEngine
{
    Q_OBJECT

    public:
        RedshiftEngine(QObject* parent, const QVariantList& args);
        ~RedshiftEngine();
        Plasma::Service *serviceForSource(const QString &source);
        void init();
    protected slots:
        bool updateSourceEvent(const QString& source);
    protected:
        bool sourceRequestEvent(const QString& name);	
};

#endif //REDSHIFTENGINE_H
