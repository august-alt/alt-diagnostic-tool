#include "finishPageWidget.h"
#include "ui_finishPageWidget.h"

FinishPageWidget::FinishPageWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FinishPageWidget())
{
    ui->setupUi(this);
}
