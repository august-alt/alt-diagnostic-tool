#ifndef ADTEXECUTABLE_H
#define ADTEXECUTABLE_H

#include "adtbaseexecutor.h"

#include <QJsonObject>
#include <QObject>
#include <QString>

class ADTExecutable : public ADTBaseExecutor
{
    Q_OBJECT
    Q_PROPERTY(int id READ getId WRITE setId)
    Q_PROPERTY(QString name READ getName WRITE setName)
    Q_PROPERTY(QString category READ getCategory WRITE setCategory)
    Q_PROPERTY(QString command READ getCommand WRITE setCommand)
    Q_PROPERTY(QString args READ getArgs WRITE setArgs)
    Q_PROPERTY(QMap<int, QString> errorCodes READ getErrorCodes WRITE setErrorCodes)

public:
    int getId() const;
    void setId(int id);

    QString getName();
    void setName(const QString name);

    QString getCategory();
    void setCategory(const QString command);

    QString getCommand();
    void setCommand(const QString command);

    QString getArgs();
    void setArgs(const QString args);

    const QMap<int, QString> getErrorCodes();
    void setErrorCodes(QMap<int, QString> &errorCodes);

public:
    ADTExecutable();

public:
    virtual bool toJson(QJsonObject &document) override;
    virtual bool fromJson(const QJsonObject &document) override;

private:
    int m_id;
    QString m_name;
    QString m_category;
    QString m_command;
    QString m_args;
    QMap<int, QString> m_errorCodes;

private:
    QJsonArray convertQMapToJsonArray(QMap<int, QString> &map);
    QMap<int, QString> convertJsonArrayToQMap(QJsonArray &array);
};

#endif //ADTEXECUTABLE_H
