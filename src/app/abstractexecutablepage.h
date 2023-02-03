#ifndef ABSTRACTEXECUTABLEPAGE_H
#define ABSTRACTEXECUTABLEPAGE_H

#include "../core/adtexecutable.h"

#include <QWizard>

class AbstractExecutablePage : public QWizardPage
{
    Q_OBJECT
public:
    AbstractExecutablePage(QWidget *parent = nullptr);
    virtual ~AbstractExecutablePage() = default;

public slots:
    virtual void beginAllTasks();
    virtual void finishAllTasks();

    virtual void beginCurrentTask(ADTExecutable *task);
    virtual void finishCurrentTask(ADTExecutable *task);

    virtual void onProgressUpdate(int progress);

    virtual void messageChanged(QString message);
};

#endif // ABSTRACTEXECUTABLEPAGE_H
