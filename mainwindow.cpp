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
    QLabel *setLineWidthReminder = new QLabel(" 粗细: ");
    ui->mainToolBar->addWidget(setLineWidthReminder);
    line_width = new QSpinBox;
    line_width->setRange(1, 5);
    line_width->setValue(1);
    line_width->setSuffix(" px");
    ui->mainToolBar->addWidget(line_width);
    QObject::connect(line_width, SIGNAL(valueChanged(int)), this, SLOT(changeLineWidth()));

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

void MainWindow::on_action_pencil_triggered()
{
    board->mode = PENCIL;
    label->setText(" 铅笔");
}

void MainWindow::changeLineWidth() {
    board-> lineWidth = line_width->value();
}

void MainWindow::on_action_color_triggered()
{
    QColor color = QColorDialog::getColor(Qt::red, this, tr("设置画笔颜色"));
    qDebug() << color;
    board->color = color;
}
