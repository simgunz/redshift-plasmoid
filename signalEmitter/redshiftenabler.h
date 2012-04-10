#include <QDBusMessage>
#include <QDBusConnection>

class RedshiftEnabler : public QObject
{
    Q_OBJECT
public slots:
    void sendSignal();
};