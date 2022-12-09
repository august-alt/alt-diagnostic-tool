#ifndef ADTEXECUTABLE_H
#define ADTEXECUTABLE_H

#include <QJsonObject>
#include <QObject>
#include <QString>

class ADTExecutable : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id MEMBER m_id)
    Q_PROPERTY(QString name MEMBER m_name)
    Q_PROPERTY(QString category MEMBER m_category)
    Q_PROPERTY(QString command MEMBER m_command)
    Q_PROPERTY(QString args MEMBER m_args)

public:
    int m_id;
    QString m_name;
    QString m_category;
    QString m_command;
    QString m_args;

public:
    ADTExecutable();
};

#endif //ADTEXECUTABLE_H
