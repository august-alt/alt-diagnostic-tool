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

#ifndef REPAIRWIZARDPAGE_H
#define REPAIRWIZARDPAGE_H

#include "../core/diagnostictool.h"

#include <QWizardPage>

namespace Ui
{
class RepairWizardPage;
}

class RepairWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    RepairWizardPage(DiagnosticTool *diagTool, QWidget *parent = nullptr);

    virtual bool isComplete() const override;

private:
    Ui::RepairWizardPage *ui;

    bool isOpening = false;

    DiagnosticTool *diagnosticTool;

    bool isCompleteResolvers;

    QThread *workingThread;

private:
    void showEvent(QShowEvent *event) override;

    void runResolvers();

private slots:
    void onProgressUpdate(int progress);

    void messageChanged(QString message);

    void disableNextButton();

    void enableNextButton();

    void cancelButtonPressed(int currentPage);

    void on_detailsPushButton_clicked();

    void appendNextLogLine(QString line);

private:
    RepairWizardPage(const RepairWizardPage &) = delete;            // copy ctor
    RepairWizardPage(RepairWizardPage &&)      = delete;            // move ctor
    RepairWizardPage &operator=(const RepairWizardPage &) = delete; // copy assignment
    RepairWizardPage &operator=(RepairWizardPage &&) = delete;      // move assignment
};

#endif // REPAIRWIZARDPAGE_H
