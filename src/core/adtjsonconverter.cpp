#include "adtjsonconverter.h"

#include <QMetaProperty>
#include <QObject>

QJsonObject ADTJsonConverter::propertyToJson(QObject *obj, QString objectName)
{
    QJsonObject object;
    QJsonObject json;

    for (int i = 1; i < obj->metaObject()->propertyCount(); i++)
    {
        QMetaProperty prop = obj->metaObject()->property(i);

        auto propName = prop.name();

        json[propName] = obj->property(propName).toString();
    }

    object[objectName] = json;

    return object;
}

void ADTJsonConverter::JSonToObject(QObject *obj, QJsonObject *json, QString objectName)
{
    QVariantMap map = json->toVariantMap();

    auto fieldMap = map[objectName].toJsonObject().toVariantMap();

    for (auto key : fieldMap.keys())
    {
        obj->setProperty(key.toStdString().c_str(), fieldMap.value(key));
    }
}
