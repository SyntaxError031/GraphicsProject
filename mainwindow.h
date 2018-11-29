#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QDialog>
#include "mywidget.h"
#include <QSpinBox>
#include <QColorDialog>
#include <QInputDialog>

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
    void on_action_pencil_triggered();    
    void changeLineWidth();

    void on_action_color_triggered();

    void on_action_ellipse_triggered();

    void on_action_rectangle_triggered();

    void on_action_fill_triggered();

    void on_action_clear_triggered();

    void on_action_rotate_triggered();

private:
    Ui::MainWindow *ui;
    MyWidget *board;    // MainWindow上绘制图形的部分
    QLabel *label;      // 状态栏上显示状态
    QSpinBox *line_width;
};



#endif // MAINWINDOW_H
