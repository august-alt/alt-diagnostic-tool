#include "adtjsonconverter.h"

#include <QMetaProperty>
#include <QObject>

QJsonObject ADTJsonConverter::propertyToJson(QObject *obj)
{
    QJsonObject json;

    for (int i = 1; i < obj->metaObject()->propertyCount(); i++)
    {
        QMetaProperty prop = obj->metaObject()->property(i);

        auto propName = prop.name();

        json[propName] = obj->property(propName).toString();
    }

    return json;
}

void ADTJsonConverter::JSonToObject(QObject *obj, QJsonObject *json)
{
    QVariantMap map = json->toVariantMap();

    for (auto key : map.keys())
    {
        obj->setProperty(key.toStdString().c_str(), map.value(key));
    }
}
