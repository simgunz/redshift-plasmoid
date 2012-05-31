#include <QtCore/QCoreApplication>
#include <QDBusMessage>
#include <QDBusConnection>

#include <redshiftenabler.h>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    RedshiftEnabler *enabler = new RedshiftEnabler();
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.connect("", "/", "org.kde.redshift", "readyCheck",
                 enabler, SLOT(sendSignal()));
    enabler->sendSignal();
    return app.exec();
}
