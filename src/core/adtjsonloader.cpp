#include "adtjsonloader.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include <QJsonArray>

ADTJsonLoader::ADTJsonLoader() {}

QJsonDocument ADTJsonLoader::loadDocument(QString file, QString jsonArrayName)
{
    QFile jsonFile(file);

    QJsonDocument doc;

    if (!jsonFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Can't open json file!";
        return doc;
    }

    QByteArray fileData = jsonFile.readAll();

    jsonFile.close();

    doc = (QJsonDocument::fromJson(fileData));

    QJsonObject object = doc.object();

    if (object.contains(jsonArrayName) && object[jsonArrayName].isArray())
    {
        return QJsonDocument(object[jsonArrayName].toArray());
    }

    qWarning() << "Can't find in " << file << " json array with name " << jsonArrayName;

    return doc;
}
