#ifndef ADTEXECUTABLERUNNERPRIVATE_H
#define ADTEXECUTABLERUNNERPRIVATE_H

#include "adtexecutable.h"

#include <QJsonDocument>

class ADTExecutableRunnerPrivate
{
public:
    ADTExecutableRunnerPrivate(QJsonDocument &document);

public:
    std::unique_ptr<std::vector<std::unique_ptr<ADTExecutable>>> tasks;

    std::unique_ptr<QJsonDocument> jsonObject;

private:
    void parseTasks(QJsonDocument *doc);
};

#endif // ADTEXECUTABLERUNNERPRIVATE_H
