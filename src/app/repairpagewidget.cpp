#include "repairPageWidget.h"
#include "ui_repairPageWidget.h"

RepairPageWidget::RepairPageWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RepairPageWidget())
{
    ui->setupUi(this);
}
