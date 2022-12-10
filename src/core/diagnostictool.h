#ifndef DIAGNOSTICTOOL_H
#define DIAGNOSTICTOOL_H

#include "diagnostictoolprivate.h"

#include <QJsonDocument>

class DiagnosticTool : public QObject
{
public:
    Q_OBJECT

public:
    DiagnosticTool(QJsonDocument &document);

public slots:
    void runChecks();
    void runResolvers();
    void cancelTask(bool stop);

private:
    std::unique_ptr<DiagnosticToolPrivate> d;

signals:
    void progressChanged(int progress);
    void messageChanged(QString);
};

#endif // DIAGNOSTICTOOL_H
