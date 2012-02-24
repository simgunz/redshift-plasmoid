#include "redshiftjob.h"

RedshiftJob::RedshiftJob(RedshiftController *controller, const QString &operation, QMap<QString, QVariant> &parameters, QObject *parent) :
    ServiceJob(parent->objectName(), operation, parameters, parent), 
    m_redshiftController(controller)
{    
}

RedshiftJob::~RedshiftJob()
{
}

void RedshiftJob::start()
{
    const QString operation = operationName();

    if (operation == "toggle") {        
        m_redshiftController->toggle();        
        setResult(true);
        return;
    } else if (operation == "restart") {        
        m_redshiftController->restart();
        setResult(true);
        return;
    }
    setResult(false);
}
