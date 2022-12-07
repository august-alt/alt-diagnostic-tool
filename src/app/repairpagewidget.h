#ifndef REPAIRPAGEWIDGET_H
#define REPAIRPAGEWIDGET_H

#include <QWidget>

namespace Ui
{
class RepairPageWidget;
}

class RepairPageWidget : public QWidget
{
    Q_OBJECT

public:
    RepairPageWidget(QWidget *parent = nullptr);

private:
    Ui::RepairPageWidget *ui;

private:
    RepairPageWidget(const RepairPageWidget &) = delete;            // copy ctor
    RepairPageWidget(RepairPageWidget &&)      = delete;            // move ctor
    RepairPageWidget &operator=(const RepairPageWidget &) = delete; // copy assignment
    RepairPageWidget &operator=(RepairPageWidget &&) = delete;      // move assignment
};

#endif //REPAIRPAGEWIDGET_H
