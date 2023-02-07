#include "adtexecutablerunnerprivate.h"
#include "adtjsonconverter.h"

#include <QJsonArray>
#include <QJsonObject>
#include <qjsondocument.h>

ADTExecutableRunnerPrivate::ADTExecutableRunnerPrivate(QJsonDocument &document)
    : tasks(std::make_unique<std::vector<std::unique_ptr<ADTExecutable>>>())
    , jsonObject(std::make_unique<QJsonDocument>(document))
{
    parseTasks(jsonObject.get());
}

void ADTExecutableRunnerPrivate::parseTasks(QJsonDocument *doc)
{
    if (!doc->isArray())
    {
        return;
    }

    QJsonArray taskArray = doc->array();

    if (taskArray.isEmpty())
    {
        return;
    }

    tasks->clear();

    for (int i = 0; i < taskArray.size(); i++)
    {
        auto task       = std::make_unique<ADTExecutable>();
        QJsonObject obj = taskArray[i].toObject();
        ADTJsonConverter::JSonToObject(*task.get(), &obj);
        tasks->push_back(std::move(task));
    }
}
