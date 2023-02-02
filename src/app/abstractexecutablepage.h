#ifndef ABSTRACTEXECUTABLEPAGE_H
#define ABSTRACTEXECUTABLEPAGE_H

#include "../core/adtexecutable.h"

class AbstractExecutablePage : public QObject
{
    Q_OBJECT
public:
    AbstractExecutablePage();

public slots:
    virtual void beginAllTasks();
    virtual void finishAllTasks();

    virtual void beginCurrentTask(ADTExecutable *task);
    virtual void finishCurrentTask(ADTExecutable *task);

    virtual void onProgressUpdate(int progress);

    virtual void messageChanged(QString message);

    ~AbstractExecutablePage();
};

#endif // ABSTRACTEXECUTABLEPAGE_H
