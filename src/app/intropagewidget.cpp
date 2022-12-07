#include "introPageWidget.h"
#include "ui_introPageWidget.h"

IntroPageWidget::IntroPageWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::IntroPageWidget())
{
    ui->setupUi(this);
}
