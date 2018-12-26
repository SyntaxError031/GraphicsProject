#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QDialog>
#include "mywidget.h"
#include <QSpinBox>
#include <QColorDialog>
#include <QInputDialog>
#include <QErrorMessage>
#include <QFileDialog>
#include <QMessageBox>
#include "threedwindow.h"

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

    void on_action_zoom_in_triggered();

    void on_action_zoom_out_triggered();

    void on_action_polygon_triggered();

    void on_action_curve_triggered();

    void on_action_undo_triggered();

    void on_action_redo_triggered();

    void on_action_N_triggered();

    void on_action_S_triggered();

    void on_action_O_triggered();

    void on_action_3D_triggered();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    MyWidget *board;    // MainWindow上绘制图形的部分
    QLabel *label;      // 状态栏上显示状态
    QSpinBox *line_width;
//    QErrorMessage *errorDlg;
//    QFileDialog *fileDlg;
    QString path;
    ThreeDWindow *threeD;
};



#endif // MAINWINDOW_H
