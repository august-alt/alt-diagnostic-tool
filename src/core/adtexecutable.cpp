#include "adtexecutable.h"

#include <QJsonArray>

ADTExecutable::ADTExecutable()
    : ADTBaseExecutor()
    , m_id(0)
    , m_name("")
    , m_category("")
    , m_command("")
    , m_args("")
    , m_errorCodes({})
{}

int ADTExecutable::getId() const
{
    return m_id;
}

void ADTExecutable::setId(int id)
{
    m_id = id;
}

QString ADTExecutable::getName()
{
    return m_name;
}

void ADTExecutable::setName(const QString name)
{
    m_name = name;
}

QString ADTExecutable::getCategory()
{
    return m_category;
}

void ADTExecutable::setCategory(const QString category)
{
    m_category = category;
}

QString ADTExecutable::getCommand()
{
    return m_command;
}

void ADTExecutable::setCommand(const QString command)
{
    m_command = command;
}

QString ADTExecutable::getArgs()
{
    return m_args;
}

void ADTExecutable::setArgs(const QString args)
{
    m_args = args;
}

const QMap<int, QString> ADTExecutable::getErrorCodes()
{
    return m_errorCodes;
}

void ADTExecutable::setErrorCodes(QMap<int, QString> &errorCodes)
{
    m_errorCodes.clear();
    m_errorCodes = errorCodes;
}

bool ADTExecutable::toJson(QJsonObject &document)
{
    document["id"] = m_id;

    document["name"] = m_name;

    document["category"] = m_category;

    document["command"] = m_command;

    document["args"] = m_args;

    document["errorCodes"] = convertQMapToJsonArray(m_errorCodes);

    return true;
}

bool ADTExecutable::fromJson(const QJsonObject &document)
{
    if (document.contains("id") && document["id"].isDouble())
    {
        m_id = document["id"].toInt();
    }

    if (document.contains("name") && document["name"].isString())
    {
        m_name = document["name"].toString();
    }

    if (document.contains("category") && document["category"].isString())
    {
        m_category = document["category"].toString();
    }

    if (document.contains("command") && document["command"].isString())
    {
        m_command = document["command"].toString();
    }

    if (document.contains("args") && document["args"].isString())
    {
        m_args = document["m_args"].toString();
    }

    if (document.contains("errorCodes") && document["errrorCodes"].isArray())
    {
        QJsonArray array = document["errrorCodes"].toArray();

        m_errorCodes = convertJsonArrayToQMap(array);
    }

    return true;
}

QJsonArray ADTExecutable::convertQMapToJsonArray(QMap<int, QString> &map)
{
    QJsonArray jsonArray;

    for (auto key : map.keys())
    {
        QJsonObject obj;

        obj["id"] = key;

        obj["errorString"] = map.value(key);

        jsonArray.append(obj);
    }

    return jsonArray;
}

QMap<int, QString> ADTExecutable::convertJsonArrayToQMap(QJsonArray &array)
{
    QMap<int, QString> map;

    for (int i = 0; i < array.size(); i++)
    {
        QJsonObject obj;

        obj = array[i].toObject();

        map.insert(obj["id"].toInt(), obj["errorString"].toString());
    }

    return map;
}
