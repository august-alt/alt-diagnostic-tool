#ifndef EXECUTABLESTATUSWIDGET_H
#define EXECUTABLESTATUSWIDGET_H

#include <QWidget>

namespace Ui
{
class ExecutableStatusWidget;
}

class ExecutableStatusWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExecutableStatusWidget(int id, QWidget *parent = nullptr);
    ~ExecutableStatusWidget();

public:
    void setId(int id);
    int getId();

    void setText(QString text);

    void setIcon(QIcon &icon);

signals:
    void onDetailsButtonPressed(int id);

private slots:
    void on_detailedPushButton_clicked();

private:
    Ui::ExecutableStatusWidget *ui;

    int currentId;

private:
    ExecutableStatusWidget(const ExecutableStatusWidget &) = delete;            // copy ctor
    ExecutableStatusWidget(ExecutableStatusWidget &&)      = delete;            // move ctor
    ExecutableStatusWidget &operator=(const ExecutableStatusWidget &) = delete; // copy assignment
    ExecutableStatusWidget &operator=(ExecutableStatusWidget &&) = delete;      // move assignment
};

#endif // EXECUTABLESTATUSWIDGET_H
