#include "abstractexecutablepage.h"

AbstractExecutablePage::AbstractExecutablePage(QWidget *parent)
    : QWizardPage(parent)
{}

void AbstractExecutablePage::beginAllTasks() {}

void AbstractExecutablePage::finishAllTasks() {}

void AbstractExecutablePage::beginCurrentTask(ADTExecutable *task) {}

void AbstractExecutablePage::finishCurrentTask(ADTExecutable *task) {}

void AbstractExecutablePage::onProgressUpdate(int progress) {}

void AbstractExecutablePage::messageChanged(QString message) {}
