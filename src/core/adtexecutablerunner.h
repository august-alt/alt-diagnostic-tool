#ifndef ADTEXECUTABLERUNNER_H
#define ADTEXECUTABLERUNNER_H

#include "adtexecutablerunnerprivate.h"

#include <memory>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusConnectionInterface>
#include <QtDBus/QDBusInterface>

class ADTExecutableRunner : public QObject
{
public:
    Q_OBJECT

public:
    ADTExecutableRunner(QJsonDocument QJsonDocument);

    int getAmountOfTasks();

    void cancelTasks();

    void resetStopFlag();

    bool isAnyErrorsInTask();

    ADTExecutable *getTask(int id);

public slots:
    void runTasks();

private:
    void executeTask(std::unique_ptr<ADTExecutable> &task);

private:
    std::unique_ptr<ADTExecutableRunnerPrivate> d;

    volatile bool stopFlag;

    std::unique_ptr<QDBusConnection> dbus;
    std::unique_ptr<QDBusInterface> dbusInterface;

private:
    void connectExecutableSignals(std::unique_ptr<ADTExecutable> &task);
    void disconnectExecutableSignals(std::unique_ptr<ADTExecutable> &task);

signals:
    void onProgressUpdate(int progress);
    void messageChanged(QString);
    void onError();

    void beginTask(ADTExecutable *task);
    void finishTask(ADTExecutable *task);

    void begin();
    void finish();
};

#endif //ADTEXECUTABLERUNNER_H
