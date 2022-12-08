#include "repairpagewidget.h"
#include "ui_repairpagewidget.h"

RepairPageWidget::RepairPageWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RepairPageWidget())
{
    ui->setupUi(this);
}
