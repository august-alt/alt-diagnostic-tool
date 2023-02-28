#include "dbuschecker.h"

#include <QDBusConnection>
#include <QDBusInterface>

bool DBusChecker::checkDBusServiceOnSystemBus(const QString &serviceName,
                                              const QString &path,
                                              const QString &interfaceName)
{
    QDBusConnection connection = QDBusConnection::systemBus();
    if (!connection.isConnected())
    {
        return false;
    }

    QDBusInterface dBusInterface(serviceName, path, interfaceName, connection);
    if (!dBusInterface.isValid())
    {
        return false;
    }

    return true;
}
