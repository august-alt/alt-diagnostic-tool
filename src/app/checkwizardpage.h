/***********************************************************************************************************************
**
** Copyright (C) 2022 BaseALT Ltd. <org@basealt.ru>
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

#ifndef CHECKWIZARDPAGE_H
#define CHECKWIZARDPAGE_H

#include "../core/diagnostictool.h"

#include <QLabel>
#include <QPlainTextEdit>
#include <QThread>
#include <QVBoxLayout>
#include <QWizardPage>

namespace Ui
{
class CheckWizardPage;
}

class CheckWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    CheckWizardPage(DiagnosticTool *diagTool, QWidget *parent = nullptr);

    virtual bool isComplete() const override;

    virtual int nextId() const override;

private:
    Ui::CheckWizardPage *ui;

    DiagnosticTool *diagnosticTool;

    bool isOpening = false;

    bool isCompleteChecks;

    QThread *workingThread;

    QLabel *currentIconLabel;
    QLabel *currentTextLabel;
    QVBoxLayout *summaryLayout;
    QVBoxLayout *detailsLayout;
    QPlainTextEdit *detailsText;
    QPushButton *currentCheckDetailsButton;
    QPushButton *backToSummaryLogsButton;

private:
    void showEvent(QShowEvent *event) override;

    void runChecks();

private slots:
    void onProgressUpdate(int progress);

    void messageChanged(QString message);

    void beginAllChecks();

    void finishAllChecks();

    void cancelButtonPressed(int currentPage);

    void beginCurrentCheck(ADTExecutable *check);
    void finishCurrentCheck(ADTExecutable *check);

    void currentCheckDetailsButton_clicked();

    void onbackToSummaryLogsButton_clicked();

    void currentIdChanged(int id);

    void cleanUpUi();

private:
    CheckWizardPage(const CheckWizardPage &) = delete;            // copy ctor
    CheckWizardPage(CheckWizardPage &&)      = delete;            // move ctor
    CheckWizardPage &operator=(const CheckWizardPage &) = delete; // copy assignment
    CheckWizardPage &operator=(CheckWizardPage &&) = delete;      // move assignment
};

#endif // CHECKWIZARDPAGE_H
