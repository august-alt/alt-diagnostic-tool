#include "diagnostictool.h"

#include <QJsonDocument>

DiagnosticTool::DiagnosticTool(QJsonDocument &document)
    : d(std::make_unique<DiagnosticToolPrivate>(document))
{}
