#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTextCodec>

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
    //errorDlg = new QErrorMessage(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_line_triggered()
{
    board->mode = LINE;
    label->setText(" 画线");
    board->status = READY;
}

void MainWindow::on_action_circle_triggered()
{
    board->mode = CIRCLE;
    label->setText(" 画圆");
    board->status = READY;
}

void MainWindow::on_action_pencil_triggered()
{
    board->mode = PENCIL;
    label->setText(" 铅笔");
    board->status = READY;
}

void MainWindow::changeLineWidth()
{
    //board->lineWidth = line_width->value();
    board->changeWidth(line_width->value());
}

void MainWindow::on_action_color_triggered()
{
    QColor color = QColorDialog::getColor(Qt::red, this, tr("设置画笔颜色"));
    //board->color = color;
    board->changeColor(color);
}

void MainWindow::on_action_ellipse_triggered()
{
    board->mode = ELLIPSE;
    label->setText(" 椭圆");
    board->status = READY;
}

void MainWindow::on_action_rectangle_triggered()
{
    board->mode = RECT;
    label->setText(" 矩形");
    board->status = READY;
}

void MainWindow::on_action_fill_triggered()
{
    board->mode = FILL;
    label->setText(" 填充");
    board->status = READY;
}

void MainWindow::on_action_clear_triggered()
{
    board->clearAll();
    label->setText(" 就绪");
}

void MainWindow::on_action_rotate_triggered()
{
    bool ok;
    int value = QInputDialog::getInt(this, tr("旋转角度"), tr("请输顺时针旋转的角度"), 0, 0, 359, 1, &ok);
    if(ok)
        board->rotate(value);
}

void MainWindow::on_action_zoom_in_triggered()
{
    board->zoomIn();
}

void MainWindow::on_action_zoom_out_triggered()
{
    board->zoomOut();
}

void MainWindow::on_action_polygon_triggered()
{
    board->mode = POLYGON;
    label->setText(" \345\244\232\350\276\271\345\275\242");
    board->status = READY;
}

void MainWindow::on_action_curve_triggered()
{
    board->mode = CURVE;
    label->setText(" 曲线");
    board->status = READY;
}

void MainWindow::on_action_undo_triggered()
{
    board->undo();
}

void MainWindow::on_action_redo_triggered()
{
    board->redo();
}

