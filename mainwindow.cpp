#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    board = new MyWidget;
    board->setAttribute(Qt::WA_StyledBackground, true);
    board->setStyleSheet("background-color:rgb(210, 220, 255)");
    this->setCentralWidget(board);
    label = new QLabel;
    label->setMinimumSize(label->sizeHint());
    label->setAlignment(Qt::AlignHCenter);
    this->statusBar()->addWidget(label);
    this->statusBar()->setStyleSheet(QString("QStatusBar::item{border: 0px}"));
    label->setText(" 就绪");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_line_triggered()
{
    board->mode = LINE;
    label->setText(" 画线");
}

void MainWindow::on_action_circle_triggered()
{
    board->mode = CIRCLE;
    label->setText(" 画圆");
}
