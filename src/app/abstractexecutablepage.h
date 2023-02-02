#ifndef ABSTRACTEXECUTABLEPAGE_H
#define ABSTRACTEXECUTABLEPAGE_H

#include "../core/adtexecutable.h"

class AbstractExecutablePage
{
public:
    AbstractExecutablePage();

public slots:
    virtual void beginAllTasks()  = 0;
    virtual void finishAllTasks() = 0;

    virtual void beginCurrentTask(ADTExecutable *task)  = 0;
    virtual void finishCurrentTask(ADTExecutable *task) = 0;

    virtual ~AbstractExecutablePage();
};

#endif // ABSTRACTEXECUTABLEPAGE_H
