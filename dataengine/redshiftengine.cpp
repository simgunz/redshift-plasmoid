#include "redshiftengine.h"
#include "redshiftservice.h"

RedshiftEngine::RedshiftEngine(QObject* parent, const QVariantList& args)
    : Plasma::DataEngine(parent, args)
{
    Q_UNUSED(args)
}

RedshiftEngine::~RedshiftEngine()
{
}

void RedshiftEngine::init()
{

}

bool RedshiftEngine::sourceRequestEvent(const QString &name)
{
    if(name == "Controller")
    {
        Plasma::DataContainer *container = containerForSource("Controller");
        if(!container)
        {
            addSource(new RedshiftContainer(this));
        }
        return true;
    }
    return false;
}

bool RedshiftEngine::updateSourceEvent(const QString &name)
{      
    return true;
}

Plasma::Service *RedshiftEngine::serviceForSource(const QString &source)
{
    RedshiftContainer* container = qobject_cast<RedshiftContainer*>(containerForSource(source));    
    if(container)
        return container->service();
    else
        return DataEngine::serviceForSource(source);
}

K_EXPORT_PLASMA_DATAENGINE(timekpr,RedshiftEngine)
  
#include "redshiftengine.moc"