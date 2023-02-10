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
    ExecutableStatusWidget(const ExecutableStatusWidget &) = delete;
    ExecutableStatusWidget(ExecutableStatusWidget &&)      = delete;
    ExecutableStatusWidget &operator=(const ExecutableStatusWidget &) = delete;
    ExecutableStatusWidget &operator=(ExecutableStatusWidget &&) = delete;
};

#endif // EXECUTABLESTATUSWIDGET_H
