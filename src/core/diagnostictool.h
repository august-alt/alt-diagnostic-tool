#ifndef DIAGNOSTICTOOL_H
#define DIAGNOSTICTOOL_H

#include "diagnostictoolprivate.h"

#include <QJsonDocument>

class DiagnosticTool
{
public:
    DiagnosticTool(QJsonDocument &document);

private:
    std::unique_ptr<DiagnosticToolPrivate> d;
};

#endif // DIAGNOSTICTOOL_H
