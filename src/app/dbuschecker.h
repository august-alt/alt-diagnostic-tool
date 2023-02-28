#ifndef DBUSCHECKER_H
#define DBUSCHECKER_H

#include <QString>

class DBusChecker
{
public:
    static bool checkDBusServiceOnSystemBus(const QString &serviceName,
                                            const QString &path,
                                            const QString &interface);
};

#endif // DBUSCHECKER_H
