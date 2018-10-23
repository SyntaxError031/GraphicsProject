#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m = new MyWidget;
    m->setAttribute(Qt::WA_StyledBackground, true);
    m->setStyleSheet("background-color:rgb(210, 220, 255)");
    this->setCentralWidget(m);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_line_triggered()
{
    m->mode = 1;
}
