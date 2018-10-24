#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "mywidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_line_triggered();

    void on_action_circle_triggered();

private:
    Ui::MainWindow *ui;
    MyWidget *board;    // MainWindow上绘制图形的部分
    QLabel *label;      // 状态栏上显示状态
};

enum {READY, LINE, CIRCLE, RECT};

#endif // MAINWINDOW_H
