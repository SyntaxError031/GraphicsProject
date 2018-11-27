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
    mode = NOTHING;
    status = READY;
    lineWidth = 1;
    rubberBand = NULL;
    figure = NULL;
    this->setMouseTracking(true);
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        if(status == READY) {
            if(mode == LINE || mode == CIRCLE || mode == ELLIPSE || mode == RECT) {
                status = DRAWING;
                origin = event->pos();
                if(!rubberBand) {
                    rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
                    rubberBand->setGeometry(QRect(origin, QSize()));
                    rubberBand->show();
                }

                switch(mode) {
                case LINE:
                    figure = new Line(); break;
                case CIRCLE:
                    figure = new Circle(); break;
                case ELLIPSE:
                    figure = new Ellipse(); break;
                case RECT:
                    figure = new Rectangle(); break;
                }
                figure->setStartPoint(event->pos());
            }
            else if(mode == PENCIL) {
                status = DRAWING;
                figure = new Line();
                figure->setStartPoint(event->pos());
            }
        }
        else if(status == EDITABLE) {
            if(isInControlBtn(event->pos()) != -1) {      // 在控制按钮上
                status = EDITING;
                // TODO: 进行编辑
            }
            //else if() {     // 在图形区域内
            //    status = EDITING;
                // TODO: 进行平移
            //}
            else {
                // TODO: buffer中的内容画定在画布上，清空buffer和辅助线
                status = READY;
                figure->buffer.clear();
                clearControlBtn();
                preControlBtn.clear();
                figure->controlBtn.clear();
                //TODO: 清空辅助线
            }
        }

    }
}

void MyWidget::mouseMoveEvent(QMouseEvent *event) {
    if(event->buttons() & Qt::LeftButton) {
        if(status == DRAWING) {
            if(mode == LINE || mode == CIRCLE || mode == ELLIPSE || mode == RECT)
                rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
            if(mode == PENCIL) {
                figure->setEndPoint(event->pos());
                figure->draw();
                drawBuffer();
                figure->buffer.clear();
                figure->setStartPoint(event->pos());      // 终点变为下一次的起点
            }
        }
        else if(status == EDITING) {
            // TODO: 在编辑
        }
    }
    if(mode == LINE || mode == CIRCLE || mode == ELLIPSE || mode == RECT) {
        if(isInControlBtn(event->pos()) != -1) {
            // TODO: 根据button序号分配不同朝向的图标
            setCursor(Qt::SizeVerCursor);
        }
//        else if() {     // 在编辑图形区域内
//            // TODO: 变成SizeAllCursor
//        }
        else {
            setCursor(Qt::CrossCursor);
        }
    }
    else {      // 不在图形绘制模式
        setCursor(Qt::ArrowCursor);
    }
}

void MyWidget::mouseReleaseEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        if(status == DRAWING) {
            if(mode == LINE || mode == CIRCLE || mode == ELLIPSE || mode == RECT) {
                rubberBand->hide();
                rubberBand = NULL;
            }
            if(mode == LINE) {
                figure->setEndPoint(event->pos());
                figure->draw();
                drawBuffer();
                figure->generateControlBtn();
                drawControlBtn();
                status = EDITABLE;
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
        else if(status == EDITING) {
            status = EDITABLE;
            // TODO: 编辑完成
        }
    }
}

void MyWidget::drawBuffer() {
    /* 保存buffer位置原来点的颜色 */
    QImage img = pix->toImage();
    preBuffer.resize(figure->buffer.size());
    for(int i = 0; i < figure->buffer.size(); i++) {
        preBuffer[i] = img.pixelColor(*figure->buffer[i]);
    }

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

void MyWidget::drawControlBtn() {
    /* 保存控制按钮原来的颜色 */
    QImage img = pix->toImage();
    vector<QColor> tmp(figure->controlBtn[0].size());
    preControlBtn.resize(figure->controlBtn.size(), tmp);
    for(int i = 0; i < figure->controlBtn.size(); i++) {
        for(int j = 0; j < figure->controlBtn[i].size(); j++)
            preControlBtn[i][j] = img.pixelColor(*figure->controlBtn[i][j]);
    }
    QPainter painter(pix);
    QPen pen;
    for(int i = 0; i < figure->controlBtn.size(); i++) {
        pen.setColor(Qt::white);
        painter.setPen(pen);
        for(int j = 0; j < 9; j++) {
            painter.drawPoint(*figure->controlBtn[i][j]);
        }
        pen.setColor(Qt::black);
        painter.setPen(pen);
        for(int j = 9; j < figure->controlBtn[i].size(); j++) {
            painter.drawPoint(*figure->controlBtn[i][j]);
        }
    }
    update();
}

void MyWidget::clearControlBtn() {
    QPainter painter(pix);
    QPen pen;
    /* 将控制按钮恢复为原来的颜色 */
    for(int i = 0; i < figure->controlBtn.size(); i++) {
        for(int j = 0; j < figure->controlBtn[i].size(); j++) {
            pen.setColor(preControlBtn[i][j]);
            painter.setPen(pen);
            painter.drawPoint(*figure->controlBtn[i][j]);
        }
    }
    update();
}


void MyWidget::clearBuffer() {
    QPainter painter(pix);
    QPen pen;
    /* 将buffer位置的点恢复为原来的颜色 */
    pen.setColor(Qt::white);
    painter.setPen(pen);
    for(int i = 0; i < figure->buffer.size(); i++) {
        pen.setColor(preBuffer[i]);
        painter.setPen(pen);
        painter.drawPoint(*figure->buffer[i]);
    }

    update();
}


int MyWidget::isInControlBtn(QPoint point) {
    /* 点是否在控制按钮范围内
     * @return 控制按钮的序号，如果不在，返回-1
     */
    if(figure) {
        int i;
        for(i = 0; i < figure->controlBtn.size(); i++) {
            int xMin = figure->controlBtn[i][9]->x(), xMax = figure->controlBtn[i][23]->x();
            int yMin = figure->controlBtn[i][9]->y(), yMax = figure->controlBtn[i][23]->y();
            if(point.x() >= xMin && point.x() <= xMax && point.y() >= yMin && point.y() <= yMax)
                return i;
        }
        if(i == figure->controlBtn.size())
            return -1;
    }
    return -1;
}

void MyWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, 800, 600, *pix);
}




