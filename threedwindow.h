#ifndef THREEDWINDOW_H
#define THREEDWINDOW_H

#include <QMainWindow>
#include "threedwidget.h"
#include <QFileDialog>

namespace Ui {
class ThreeDWindow;
}

class ThreeDWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ThreeDWindow(QWidget *parent = nullptr);
    ~ThreeDWindow();

private slots:
    void on_action_O_triggered();

private:
    Ui::ThreeDWindow *ui;
    ThreeDWidget *threeDBoard;
};

#endif // THREEDWINDOW_H
