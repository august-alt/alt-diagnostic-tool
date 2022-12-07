#include "checkPageWidget.h"
#include "ui_checkPageWidget.h"

CheckPageWidget::CheckPageWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CheckPageWidget())
{
    ui->setupUi(this);
}
