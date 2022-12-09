#ifndef BASEEXECUTOR_H
#define BASEEXECUTOR_H

#include <QDataStream>

class ADTBaseExecutor : public QObject
{
public:
    virtual bool toJson(QJsonObject &document) = 0;

    virtual bool fromJson(const QJsonObject &document) = 0;
};

#endif //BASEEXECUTOR_H
