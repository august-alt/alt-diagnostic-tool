#ifndef CHECKPAGEWIDGET_H
#define CHECKPAGEWIDGET_H

#include <QWidget>

namespace Ui
{
class CheckPageWidget;
}

class CheckPageWidget : public QWidget
{
    Q_OBJECT

public:
    CheckPageWidget(QWidget *parent = nullptr);

private:
    Ui::CheckPageWidget *ui;

private:
    CheckPageWidget(const CheckPageWidget &) = delete;            // copy ctor
    CheckPageWidget(CheckPageWidget &&)      = delete;            // move ctor
    CheckPageWidget &operator=(const CheckPageWidget &) = delete; // copy assignment
    CheckPageWidget &operator=(CheckPageWidget &&) = delete;      // move assignment
};

#endif //CHECKPAGEWIDGET_H
