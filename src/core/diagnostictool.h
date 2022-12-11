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
    void cancelTask();

public slots:
    void runChecks();
    void runResolvers();

private:
    std::unique_ptr<DiagnosticToolPrivate> d;

    volatile bool stopFlag;

signals:
    void progressChanged(int progress);
    void messageChanged(QString);

    void begin();
    void finish();
};

#endif // DIAGNOSTICTOOL_H
