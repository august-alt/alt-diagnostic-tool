#ifndef ADTJSONLOADER_H
#define ADTJSONLOADER_H

#include <QJsonDocument>

class ADTJsonLoader
{
public:
    ADTJsonLoader();

    static QJsonDocument loadDocument(QString file, QString jsonArrayName);
};

#endif // ADTJSONLOADER_H
