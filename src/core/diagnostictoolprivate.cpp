#include "diagnostictoolprivate.h"
#include "adtjsonconverter.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <qfile.h>

DiagnosticToolPrivate::DiagnosticToolPrivate(QJsonDocument &document)
    : checks(std::make_unique<std::vector<std::unique_ptr<ADTExecutable>>>())
    , resolvers(std::make_unique<std::vector<std::unique_ptr<ADTExecutable>>>())
    , currentDocument(std::make_unique<QJsonDocument>(document))
{
    parseExecutors(currentDocument.get());
}

void DiagnosticToolPrivate::parseExecutors(QJsonDocument *doc)
{
    QJsonObject object = doc->object();

    if (object.contains("checks") && object["checks"].isArray())
    {
        checks.get()->clear();

        QJsonArray checksArray = object["checks"].toArray();

        for (int i = 0; i < checksArray.size(); i++)
        {
            auto check      = std::make_unique<ADTExecutable>();
            QJsonObject obj = checksArray[i].toObject();
            ADTJsonConverter::JSonToObject(check.get(), &obj);
            checks.get()->emplace_back(std::move(check));
        }
    }

    if (object.contains("resolvers") && object["resolvers"].isArray())
    {
        resolvers.get()->clear();

        QJsonArray resolversArray = object["resolvers"].toArray();

        for (int i = 0; i < resolversArray.size(); i++)
        {
            auto resolver   = std::make_unique<ADTExecutable>();
            QJsonObject obj = resolversArray[i].toObject();
            ADTJsonConverter::JSonToObject(resolver.get(), &obj);
            resolvers.get()->emplace_back(std::move(resolver));
        }
    }
}
