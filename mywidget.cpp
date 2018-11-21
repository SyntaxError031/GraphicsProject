#include "mywidget.h"
#include "ui_mywidget.h"
#include <QtDebug>
#include "mainwindow.h"

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    pix = new QPixmap(800, 600);
    pix->fill(Qt::white);
    mode = READY;
    lineWidth = 1;
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        if(mode == LINE) {
            figure = new Line();
            figure->setStartPoint(event->pos());
        }
        else if(mode == CIRCLE) {
            figure = new Circle();
            figure->setStartPoint(event->pos());
        }
        else if(mode == PENCIL) {
            figure = new Line();
            figure->setStartPoint(event->pos());
//            qDebug() << "press mouse" << endl;
        }
        else if(mode == ELLIPSE) {
            figure = new Ellipse();
            figure->setStartPoint(event->pos());
        }
        else if(mode == RECT) {
            figure = new Rectangle();
            figure->setStartPoint(event->pos());
        }
    }
}

void MyWidget::mouseMoveEvent(QMouseEvent *event) {
    if(event->buttons() & Qt::LeftButton) {
        if(mode == PENCIL) {
            figure->setEndPoint(event->pos());
            figure->draw();
            drawBuffer();
            figure->buffer.clear();
            figure->setStartPoint(event->pos());      // 终点变为下一次的起点
        }
    }
}

void MyWidget::mouseReleaseEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        if(mode == LINE) {
//            figure->setEndPoint(event->pos());
//            figure->draw();
//            drawBuffer();
//            endPoint = event->pos();
//            drawLine();         // 松开后画线
            figure->setEndPoint(event->pos());
            figure->draw();
            drawBuffer();
//            exists.insert(exists.end(), figure->buffer.begin(), figure->buffer.end());
//            figure->buffer.clear();
        }
        else if(mode == CIRCLE) {
            figure->setEndPoint(event->pos());
            figure->draw();
            drawBuffer();
        }
        else if(mode == ELLIPSE) {
            figure->setEndPoint(event->pos());
            figure->draw();
            drawBuffer();
        }
        else if(mode == RECT) {
            figure->setEndPoint(event->pos());
            figure->draw();
            drawBuffer();
        }
    }
}

void MyWidget::drawBuffer() {
    QPainter painter(pix);
    QPen pen;
    pen.setWidth(lineWidth);
    pen.setColor(color);
    painter.setPen(pen);

    int size = figure->buffer.size();
    for(int i = 0; i < size; i++) {
        painter.drawPoint(*figure->buffer[i]);
    }
    update();
}

/*
void MyWidget::clearBuffer() {
    QPainter painter(pix);
    QPen pen;
    pen.setWidth(lineWidth);
    pen.setColor(Qt::white);
    painter.setPen(pen);

    for(int i = 0; i < figure->buffer.size(); i++) {
        if(std::find(exists.begin(), exists.end(), figure->buffer[i]) == exists.end()) {
            painter.drawPoint(*figure->buffer[i]);
        }
    }
    update();
}
*/

void MyWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, 800, 600, *pix);
}




