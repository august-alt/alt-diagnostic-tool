#ifndef DIAGNOSTICTOOLPRIVATE_H
#define DIAGNOSTICTOOLPRIVATE_H

#include "adtcheck.h"
#include "adtresolver.h"

#include <memory>

class DiagnosticToolPrivate
{
public:
    DiagnosticToolPrivate(QJsonDocument &document);

public:
    std::unique_ptr<std::vector<std::unique_ptr<ADTExecutable>>> checks;
    std::unique_ptr<std::vector<std::unique_ptr<ADTExecutable>>> resolvers;

    std::unique_ptr<QJsonDocument> currentDocument;

private:
    void parseExecutors(QJsonDocument *doc);
    //void saveExecutors(QJsonObject *doc);
};

#endif // DIAGNOSTICTOOLPRIVATE_H
