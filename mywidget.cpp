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
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        if(mode == LINE || mode == CIRCLE) {
            startPoint = event->pos();
//            qDebug() << "press mouse" << endl;
        }
    }
}

void MyWidget::mouseMoveEvent(QMouseEvent *event) {
    if(event->buttons() & Qt::LeftButton) {
        if(mode == LINE || mode == CIRCLE) {
            endPoint = event->pos();
//            qDebug() << "move" << " ";
//            drawLine();       // 鼠标拖动实时画线
        }
    }
}

void MyWidget::mouseReleaseEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        if(mode == LINE) {
            endPoint = event->pos();
//            qDebug() << "leave" << endl;
            drawLine();         // 松开后画线
        }
        else if(mode == CIRCLE) {
            endPoint = event->pos();
            drawCircle();
        }
    }
}

void MyWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, 800, 600, *pix);
}

void MyWidget::drawLine() {
    QPainter painter(pix);
//  pix.fill(Qt::white);
    int x1, x2, y1, y2;
    if(startPoint.x() <= endPoint.x()) {
        x1 = startPoint.x(), x2 = endPoint.x();
        y1 = startPoint.y(), y2 = endPoint.y();
    }
    else {
        x1 = endPoint.x(), x2 = startPoint.x();
        y1 = endPoint.y(), y2 = startPoint.y();
    }
//    qDebug() << x1 << " " << y1 << " " << x2 << " " << y2;
//    painter.drawLine(QPoint(x1,y1), QPoint(x2,y2));

    int a = y1 - y2, b = x2 - x1;
    int d, delta1, delta2;
    int x = x1, y = y1;
    painter.drawPoint(x, y);

    // k does not exist
    if(x1 == x2) {
        for(y = qMin(y1, y2); y <= qMax(y1, y2); y++) {
            painter.drawPoint(x, y);
        }
    }
    else {
        // 0 <= k <= 1
        if(y1 >= y2 && (y1-y2 <= x2-x1)) {
            d = 2*a - b;
            delta1 = 2*a, delta2 = 2*(a-b);
            while(x < x2) {
                if(d < 0) {
                    x++;
                    d += delta1;
                } else {
                    x++, y--;
                    d += delta2;
                }
                painter.drawPoint(x, y);
            }
        }
        // k > 1
        else if(y1 > y2 && (y1-y2 > x2-x1)) {
            d = a - 2*b;
            delta1 = -2*b, delta2 = 2*(a-b);
            while(y > y2) {
                if(d < 0) {
                    y--, x++;
                    d += delta2;
                } else {
                    y--;
                    d += delta1;
                }
                painter.drawPoint(x, y);
            }
        }
        // -1 <= k < 0
        else if(y1 < y2 && (y2-y1 <= x2-x1)) {
            d = 2*a + b;
            delta1 = 2*a, delta2 = 2*(a+b);
            while(x < x2) {
                if(d < 0) {
                    x++, y++;
                    d += delta2;
                } else {
                    x++;
                    d += delta1;
                }
                painter.drawPoint(x, y);
            }
        }
        // k < -1
        else if(y1 < y2 && (y2-y1 > x2-x1)) {
            d = a + 2*b;
            delta1 = 2*b, delta2 = 2*(a+b);
            while(y < y2) {
                if(d < 0) {
                    y++;
                    d += delta1;
                } else {
                    y++, x++;
                    d += delta2;
                }
                painter.drawPoint(x, y);
            }
        }
    }

    update();
}

void MyWidget::drawCircle() {
    QPainter painter(pix);
    int centerX, centerY, r;
    int x, y, p;
    centerX = (startPoint.x()+endPoint.x()) / 2;
    centerY = (startPoint.y()+endPoint.y()) / 2;
    r = (int)(sqrt((startPoint.x()-endPoint.x())*(startPoint.x()-endPoint.x()) + \
             (startPoint.y()-endPoint.y())*(startPoint.y()-endPoint.y()))/2.0);
    x = 0, y = r;
    p = 3 - 2*r;
    for(; x <= y; x++) {
        painter.drawPoint(x+centerX, y+centerY);
        painter.drawPoint(y+centerX, x+centerY);
        painter.drawPoint(y+centerX, -x+centerY);
        painter.drawPoint(x+centerX, -y+centerY);
        painter.drawPoint(-x+centerX, -y+centerY);
        painter.drawPoint(-y+centerX, -x+centerY);
        painter.drawPoint(-y+centerX, x+centerY);
        painter.drawPoint(-x+centerX, y+centerY);
        if(p >= 0) {
            p += (4*(x-y) + 10);
            y--;
        } else
            p += (4*x + 6);
    }
    update();
}



