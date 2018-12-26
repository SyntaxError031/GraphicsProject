#include "threedwindow.h"
#include "ui_threedwindow.h"

ThreeDWindow::ThreeDWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ThreeDWindow)
{
    ui->setupUi(this);
    threeDBoard = new ThreeDWidget;
    this->setCentralWidget(threeDBoard);
    setWindowTitle(tr("三维展示"));
    setWindowIcon(QIcon(":/image/images/palette_48px_28678_easyicon.net.ico"));
}

ThreeDWindow::~ThreeDWindow()
{
    delete ui;
}

void ThreeDWindow::on_action_O_triggered()
{
    QString openPath = QFileDialog::getOpenFileName(this, tr("打开三维模型"), tr(""), tr("3D Models (*.off)"));
    if(!openPath.isEmpty())
        threeDBoard->readFile(openPath);
}
