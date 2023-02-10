#ifndef ABSTRACTEXECUTABLEPAGE_H
#define ABSTRACTEXECUTABLEPAGE_H

#include "../core/adtexecutable.h"
#include "../core/adtexecutablerunner.h"
#include "executablestatuswidget.h"

#include "adtwizardpage.h"
#include "ui_checkwizardpage.h"

#include <QPlainTextEdit>
/***********************************************************************************************************************
**
** Copyright (C) 2023 BaseALT Ltd. <org@basealt.ru>
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**
***********************************************************************************************************************/

#include <QWizard>

class AbstractExecutablePage : public QWizardPage
{
    Q_OBJECT
public:
    AbstractExecutablePage(ADTExecutableRunner *run, QWidget *parent = nullptr);
    virtual ~AbstractExecutablePage();

    virtual void runTasks();

    virtual bool isComplete() const override;

public slots:
    virtual void beginAllTasks();
    virtual void finishAllTasks();

    virtual void beginCurrentTask(ADTExecutable *task);
    virtual void finishCurrentTask(ADTExecutable *task);

    virtual void onProgressUpdate(int progress);

    virtual void messageChanged(QString message);

public:
    ADTWizardPage<Ui::CheckWizardPage> *pageUi;

protected:
    ADTExecutableRunner *runner;

    bool isOpening = false;

    bool isCompleteTasks;

    QThread *workingThread;

    ExecutableStatusWidget *currentCheckWidget;
    QVBoxLayout *summaryLayout;
    QVBoxLayout *detailsLayout;
    QPlainTextEdit *detailsText;
    QPushButton *currentCheckDetailsButton;
    QPushButton *backToSummaryLogsButton;

protected:
    virtual void enableButtonsAfterChecks();
    virtual void disableButtonsBeforeChecks();

    virtual void cleanUpUi();

private slots:
    virtual void currentTaskDetailsButton_clicked(int id);

    virtual void exchangeWidgetsInStackedWidget();

private:
    AbstractExecutablePage(const AbstractExecutablePage &) = delete;
    AbstractExecutablePage(AbstractExecutablePage &&)      = delete;
    AbstractExecutablePage &operator=(const AbstractExecutablePage &) = delete;
    AbstractExecutablePage &operator=(AbstractExecutablePage &&) = delete;
};

#endif // ABSTRACTEXECUTABLEPAGE_H
