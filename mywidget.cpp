#include "mywidget.h"
#include "ui_mywidget.h"
#include <QtDebug>
#include "mainwindow.h"

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    pix = QPixmap(800, 600);
    pix.fill(Qt::white);
    mode = NOTHING;
    status = READY;
    lineWidth = 1;
    rubberBand = NULL;
    figure = NULL;
    polygon = NULL;
    this->setMouseTracking(true);
    button = -1;
    polygonBtn = -1;
    tmp = pix;
    withoutBtn = tmp;
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
drawing:
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
            else if(mode == FILL) {
                // 填充
                fill(event->pos());
            }
            else if(mode == POLYGON) {  // 画多边形
                status = DRAWING;
                if(!polygon) {   // 还没有多边形，画第一条边
                    polygon = new Polygon();
                    polygon->isFirst = true;
                    polygon->isEnd = false;
                    polygon->points.push_back(event->pos());
                    origin = event->pos();
                    if(!rubberBand) {
                        rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
                        rubberBand->setGeometry(QRect(origin, QSize()));
                        rubberBand->show();
                    }
                }
                else {

                }
            }
        }
        else if(status == EDITABLE) {

            if(mode != POLYGON && (button = isInControlBtn(event->pos())) != -1) {      // 在控制按钮上
                status = EDITING;
                // 进行编辑
                if(mode == CIRCLE || mode == ELLIPSE || mode == RECT) {
                    switch (button) {
                    case 0:
                        figure->setStartPoint(QPoint(figure->border[2], figure->border[3])); break;
                    case 1:
                        figure->setStartPoint(QPoint(figure->border[2], figure->border[1])); break;
                    case 2:
                        figure->setStartPoint(QPoint(figure->border[0], figure->border[3])); break;
                    case 3:
                        figure->setStartPoint(QPoint(figure->border[0], figure->border[1])); break;
                    }
                }
            }
            else if(mode != POLYGON && isInBorder(event->pos())) {     // 在图形区域内
                status = MOVE;
                // TODO: 进行平移
                moveStart = event->pos();

            }
            else if(mode == LINE && isOnLine(event->pos())) {
                status = MOVE;
                moveStart = event->pos();
            }
            else if(mode == POLYGON && (polygonBtn = isInPolygonControlBtn(event->pos())) != -1) {
                status = EDITING;
                // 进行编辑
            }
            else if(mode == POLYGON && isInPolygonBorder(event->pos())) {
                status = MOVE;
                // 进行平移
                moveStart = event->pos();
            }
            else {
                // TODO: buffer中的内容画定在画布上，清空buffer和辅助线
                status = READY;
                if(mode == POLYGON) {
                    polygon->controlBtn.clear();
                    polygon->border.clear();
                    polygon = NULL;
                }
                else {
                    figure->buffer.clear();
                //clearControlBtn();
                //preControlBtn.clear();
                    figure->controlBtn.clear();
                }
                //TODO: 清空辅助线
                //figure->border.clear();
                tmp = withoutBtn;
                pix = tmp;
                update();
                goto drawing;
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
                withoutBtn = tmp;
                figure->buffer.clear();
                figure->setStartPoint(event->pos());      // 终点变为下一次的起点
            }
            else if(mode == POLYGON) {
                if(polygon->isFirst) {  // 第一条线
                    rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
                }
            }
        }
        else if(status == EDITING) {
            // 在编辑
            if(mode == LINE) {
                if(button == 0) {
                    figure->setStartPoint(event->pos());
                }
                else if(button == 1) {
                    figure->setEndPoint(event->pos());
                }
            }
            else if(mode == RECT || mode == CIRCLE || mode == ELLIPSE) {
                if(button == 0) {
                    if(event->pos().x() < figure->getStartPoint().x() && event->pos().y() < figure->getStartPoint().y())
                        figure->setEndPoint(event->pos());
                }
                else if(button == 1) {
                    if(event->pos().x() < figure->getStartPoint().x() && event->pos().y() > figure->getStartPoint().y())
                        figure->setEndPoint(event->pos());
                }
                else if(button == 2) {
                    if(event->pos().x() > figure->getStartPoint().x() && event->pos().y() < figure->getStartPoint().y())
                        figure->setEndPoint(event->pos());
                }
                else if(button == 3) {
                    if(event->pos().x() > figure->getStartPoint().x() && event->pos().y() > figure->getStartPoint().y())
                        figure->setEndPoint(event->pos());
                }
            }
            else if(mode == POLYGON) {
                polygon->points[polygonBtn] = event->pos();
                tmp = pix;
                drawPolygon();
                withoutBtn = tmp;
                polygon->controlBtn.clear();
                polygon->generateControlBtn();
                drawPolygonControlBtn();
                polygon->generateBorder();
            }

            if(mode != POLYGON) {
                tmp = pix;
                figure->controlBtn.clear();
                //preControlBtn.clear();
                //clearBuffer();
                figure->buffer.clear();
                //preBuffer.clear();
                figure->draw();
                drawBuffer();
                withoutBtn = tmp;
                figure->generateBorder();
                figure->generateControlBtn();
                drawControlBtn();
            }

        }
        else if(status == MOVE) {
            // 在平移
            if(mode != POLYGON)
                moveFigure(event->pos());
            else
                movePolygon(event->pos());
        }
    }
    if(mode == LINE || mode == CIRCLE || mode == ELLIPSE || mode == RECT || mode == PENCIL || mode == FILL || mode == POLYGON) {
        if(status == EDITABLE) {
            if(mode == POLYGON) {
                if(isInPolygonControlBtn(event->pos()) != -1)
                    setCursor(Qt::SizeVerCursor);
                else if(isInPolygonBorder(event->pos()))
                    setCursor(Qt::SizeAllCursor);
                else
                    setCursor(Qt::CrossCursor);
            }
            else {
                if(isInControlBtn(event->pos()) != -1) {
                    // TODO: 根据button序号分配不同朝向的图标
                    setCursor(Qt::SizeVerCursor);
                }
                else if(mode == LINE && isOnLine(event->pos())) {
                    setCursor(Qt::SizeAllCursor);
                }
                else if(isInBorder(event->pos())) {     // 在编辑图形区域内
                    // 变成SizeAllCursor
                    setCursor(Qt::SizeAllCursor);
                }
                else
                    setCursor(Qt::CrossCursor);
            }
        }
        else if(status == READY) {
            if(figure) {
                figure->buffer.clear();
                //clearControlBtn();
                //preControlBtn.clear();
                figure->controlBtn.clear();
                //TODO: 清空辅助线
                figure->border.clear();
                tmp = withoutBtn;
                pix = tmp;
                tmp = pix;
                update();
            }
            else if(polygon) {
                polygon->isEnd = true;
                tmp = pix;
                drawPolygon();
                polygon->border.clear();
                polygon->points.clear();
                polygon = NULL;
                pix = tmp;
                withoutBtn = tmp;
                tmp = pix;
                update();
            }
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
                figure->setEndPoint(event->pos());
                if(figure->getStartPoint() == figure->getEndPoint()) {
                    status = READY;
                    return ;
                }
                tmp = pix;
                figure->draw();
                drawBuffer();
                withoutBtn = tmp;
                figure->generateBorder();
                figure->generateControlBtn();
                drawControlBtn();
                status = EDITABLE;

            }
            else if(mode == PENCIL) {
                status = READY;
                pix = tmp;
                withoutBtn = tmp;
            }
            else if(mode == POLYGON) {
                tmp = pix;
                if(polygon->isFirst && event->pos() != polygon->points[0]) {  // 画第一条线结束
                    polygon->isFirst = false;
                    rubberBand->hide();
                    rubberBand = NULL;
                    polygon->points.push_back(event->pos());
                    drawPolygon();
                }
                else if(event->pos() != polygon->points[0]){
                    // 闭合多边形
                    if(abs(event->pos().x()-polygon->points[0].x()) <= 5 && abs(event->pos().y()-polygon->points[0].y()) <= 5) {
                        polygon->isEnd = true;
                        drawPolygon();
                        withoutBtn = tmp;
                        // TODO: 画出控制点
                        polygon->generateControlBtn();
                        polygon->generateBorder();
                        drawPolygonControlBtn();
                        status = EDITABLE;
                    }
                    else {
                        polygon->points.push_back(event->pos());
                        drawPolygon();
                    }
                }
                else {
                    polygon->points.clear();
                    polygon = NULL;
                    status = READY;
                }
            }
        }
        else if(status == EDITING || status == MOVE) {
            status = EDITABLE;
            // 编辑完成
        }
    }
}

void MyWidget::moveFigure(QPoint point) {
    if(isInWidget(point.x(), point.y())) {
        int deltaX = point.x() - moveStart.x();
        int deltaY = point.y() - moveStart.y();
        //clearControlBtn();
        figure->controlBtn.clear();
        //preControlBtn.clear();
        //clearBuffer();
        //figure->buffer.clear();
        //preBuffer.clear();
        for(int i = 0; i < figure->buffer.size(); i++) {
            figure->buffer[i]->setX(figure->buffer[i]->x() + deltaX);
            figure->buffer[i]->setY(figure->buffer[i]->y() + deltaY);
        }
        figure->setStartPoint(QPoint(figure->getStartPoint().x() + deltaX, figure->getStartPoint().y() + deltaY));
        figure->setEndPoint(QPoint(figure->getEndPoint().x() + deltaX, figure->getEndPoint().y() + deltaY));
        //figure->draw();
        tmp = pix;
        drawBuffer();
        withoutBtn = tmp;
        if(!figure->border.empty()) {
            figure->border[0] += deltaX, figure->border[2] += deltaX;
            figure->border[1] += deltaY, figure->border[3] += deltaY;
        }
        figure->generateControlBtn();
        drawControlBtn();
        //figure->generateBorder();
        moveStart = point;
    }
}

void MyWidget::movePolygon(QPoint point) {
    if(isInWidget(point.x(), point.y())) {
        int deltaX = point.x() - moveStart.x();
        int deltaY = point.y() - moveStart.y();

        for(int i = 0; i < polygon->points.size(); i++) {
            polygon->points[i].setX(polygon->points[i].x() + deltaX);
            polygon->points[i].setY(polygon->points[i].y() + deltaY);
        }

        tmp = pix;
        drawPolygon();
        withoutBtn = tmp;
        polygon->controlBtn.clear();
        polygon->generateControlBtn();
        drawPolygonControlBtn();
        polygon->generateBorder();
        moveStart = point;
    }
}

void MyWidget::rotate(int value) {
    if(status == EDITABLE && (mode == RECT || mode == ELLIPSE || mode == LINE || mode == CIRCLE)) {
        //clearControlBtn();
        figure->controlBtn.clear();
        //preControlBtn.clear();

        if(value != 180 && mode != CIRCLE) {

            //clearBuffer();
            //preBuffer.clear();
            tmp = pix;
            int centerX = figure->getCenter().x(), centerY = figure->getCenter().y();
            if(value != 90 && value != 270) {
                double radian = (double)value*3.14159 / 180.0;
                for(int i = 0; i < figure->buffer.size(); i++) {
                    int x = figure->buffer[i]->x(), y = figure->buffer[i]->y();
                    figure->buffer[i]->setX(centerX+(x-centerX)*cos(radian) - (y-centerY)*sin(radian));
                    figure->buffer[i]->setY(centerY+(x-centerX)*sin(radian) + (y-centerY)*cos(radian));
                }
            }
            else {
                for(int i = 0; i < figure->buffer.size(); i++) {
                    int x = figure->buffer[i]->x(), y = figure->buffer[i]->y();
                    figure->buffer[i]->setX(centerX - y + centerY);
                    figure->buffer[i]->setY(centerY + x - centerX);
                }
            }
            drawBuffer();
        }

        withoutBtn = tmp;
        //preBuffer.clear();
        figure->buffer.clear();
        figure->border.clear();
        status = READY;
        pix = tmp;
    }
    if(status == EDITABLE && mode == POLYGON) {
        polygon->controlBtn.clear();
        tmp = pix;
        int centerX = polygon->getCenter().x(), centerY = polygon->getCenter().y();
        double radian = (double)value*3.14159 / 180.0;
        for(int i = 0; i < polygon->points.size(); i++) {
            int x = polygon->points[i].x(), y = polygon->points[i].y();
            polygon->points[i].setX(centerX+(x-centerX)*cos(radian) - (y-centerY)*sin(radian));
            polygon->points[i].setY(centerY+(x-centerX)*sin(radian) + (y-centerY)*cos(radian));
        }

        drawPolygon();
        withoutBtn = tmp;
        polygon->points.clear();
        polygon = NULL;
        status = READY;
        pix = tmp;
    }

}


void MyWidget::zoomIn() {
    if(status == EDITABLE && (mode == CIRCLE || mode == ELLIPSE || mode == RECT)) {
        tmp = pix;
        figure->controlBtn.clear();
        figure->buffer.clear();
        int centerX = figure->getCenter().x(), centerY = figure->getCenter().y();
        figure->setStartPoint(QPoint(figure->getStartPoint().x()*2-centerX, figure->getStartPoint().y()*2-centerY));
        figure->setEndPoint(QPoint(figure->getEndPoint().x()*2-centerX, figure->getEndPoint().y()*2-centerY));
        figure->draw();
        drawBuffer();
        withoutBtn = tmp;
        figure->generateBorder();
        figure->generateControlBtn();
        drawControlBtn();
        status = EDITABLE;
    }
    if(status == EDITABLE && mode == POLYGON) {
        tmp = pix;
        polygon->controlBtn.clear();
        int centerX = polygon->getCenter().x(), centerY = polygon->getCenter().y();
        for(int i = 0; i < polygon->points.size(); i++) {
            int x = polygon->points[i].x(), y = polygon->points[i].y();
            polygon->points[i].setX(x*2-centerX);
            polygon->points[i].setY(y*2-centerY);
        }
        drawPolygon();
        withoutBtn = tmp;
        polygon->generateControlBtn();
        drawPolygonControlBtn();
        polygon->generateBorder();
        status = EDITABLE;
    }
}

void MyWidget::zoomOut() {
    if(status == EDITABLE && (mode == CIRCLE || mode == ELLIPSE || mode == RECT)) {
        int centerX = figure->getCenter().x(), centerY = figure->getCenter().y();
        int startPointAfterX = figure->getStartPoint().x()/2+centerX/2, startPointAfterY = figure->getStartPoint().y()/2+centerY/2;
        int endPointAfterX = figure->getEndPoint().x()/2+centerX/2, endPointAfterY = figure->getEndPoint().y()/2+centerY/2;
        if(abs(startPointAfterX - endPointAfterX) >= 5 || abs(startPointAfterY - endPointAfterY) >= 5) {
            figure->setStartPoint(QPoint(startPointAfterX, startPointAfterY));
            figure->setEndPoint(QPoint(endPointAfterX, endPointAfterY));
            tmp = pix;
            figure->controlBtn.clear();
            figure->buffer.clear();
            figure->draw();
            drawBuffer();
            withoutBtn = tmp;
            figure->generateBorder();
            figure->generateControlBtn();
            drawControlBtn();
        }
        else
            return ;
    }
    if(status == EDITABLE && mode == POLYGON) {
        // 太小了不能再缩小了
        if((polygon->border[2]-polygon->border[0]) < 10 && (polygon->border[3]-polygon->border[1]) < 10)
            return ;
        tmp = pix;
        polygon->controlBtn.clear();
        int centerX = polygon->getCenter().x(), centerY = polygon->getCenter().y();
        for(int i = 0; i < polygon->points.size(); i++) {
            int x = polygon->points[i].x(), y = polygon->points[i].y();
            polygon->points[i].setX(x/2+centerX/2);
            polygon->points[i].setY(y/2+centerY/2);
        }
        drawPolygon();
        withoutBtn = tmp;
        polygon->generateControlBtn();
        drawPolygonControlBtn();
        polygon->generateBorder();
        status = EDITABLE;
    }
}


void MyWidget::fill(QPoint seed) {
    QPainter painter(&pix);
    painter.setPen(QPen(color));

    queue<Point> stk;
    set<Point> seen;
    QImage img = pix.toImage();
    QColor insideColor = img.pixelColor(seed);
    if(!isInWidget(seed.x(), seed.y()))
        return ;
    stk.push(Point(seed.x(), seed.y()));
    seen.insert(Point(seed.x(), seed.y()));
    while(!stk.empty()) {
        Point tmp = stk.front();
        stk.pop();
        painter.drawPoint(tmp.x, tmp.y);
        if(isInWidget(tmp.x-1, tmp.y) && seen.insert(Point(tmp.x-1,tmp.y)).second) {
            if(img.pixelColor(tmp.x-1, tmp.y) == insideColor)
                stk.push(Point(tmp.x-1, tmp.y));
        }
        if(isInWidget(tmp.x+1, tmp.y) && seen.insert(Point(tmp.x+1,tmp.y)).second) {
            if(img.pixelColor(tmp.x+1, tmp.y) == insideColor)
                stk.push(Point(tmp.x+1, tmp.y));
        }
        if(isInWidget(tmp.x, tmp.y-1) && seen.insert(Point(tmp.x,tmp.y-1)).second) {
            if(img.pixelColor(tmp.x, tmp.y-1) == insideColor)
                stk.push(Point(tmp.x, tmp.y-1));
        }
        if(isInWidget(tmp.x, tmp.y+1) && seen.insert(Point(tmp.x,tmp.y+1)).second) {
            if(img.pixelColor(tmp.x, tmp.y+1) == insideColor)
                stk.push(Point(tmp.x, tmp.y+1));
        }
    }
    seen.clear();
    tmp = pix;
    withoutBtn = tmp;
    update();
}

bool MyWidget::isInWidget(int x, int y) {
    return (x >= 0 && x < 800 && y >= 0 && y < 600);
}

void MyWidget::drawBuffer() {
    /* 保存buffer位置原来点的颜色 */
    /*
    QImage img = pix->toImage();
    preBuffer.resize(figure->buffer.size());
    for(int i = 0; i < figure->buffer.size(); i++) {
        preBuffer[i] = img.pixelColor(*figure->buffer[i]);
    }
    */

    QPainter painter(&tmp);
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

void MyWidget::drawPolygon() {
    QPainter painter(&tmp);
    QPen pen;
    pen.setWidth(lineWidth);
    pen.setColor(color);
    painter.setPen(pen);
    int size = polygon->points.size();
    for(int i = 1; i < size; i++) {
        painter.drawLine(polygon->points[i], polygon->points[i-1]);
    }
    if(polygon->isEnd)
        painter.drawLine(polygon->points[size-1], polygon->points[0]);
    update();
}

void MyWidget::drawControlBtn() {
    /* 保存控制按钮原来的颜色 */
    /*
    QImage img = pix->toImage();
    preControlBtn.resize(figure->controlBtn.size(), vector<QColor>(25));
    for(int i = 0; i < figure->controlBtn.size(); i++) {
//        for(int j = 0; j < figure->controlBtn[i].size(); j++)
//            preControlBtn[i][j] = img.pixelColor(*figure->controlBtn[i][j]);
        int cnt = 0;
        int xMin = figure->controlBtn[i][0]->x(), xMax =  figure->controlBtn[i][1]->x();
        int yMin = figure->controlBtn[i][0]->y(), yMax = figure->controlBtn[i][1]->y();
        for(int j = xMin; j <= xMax; j++) {
            for(int k = yMin; k <= yMax; k++)
                preControlBtn[i][cnt++] = img.pixelColor(j, k);
        }
    }
    */
    /*
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
    */
    QPainter painter(&tmp);
    painter.setPen(QPen(Qt::black));
    painter.setBrush(QBrush(Qt::white));
    for(int i = 0; i < figure->controlBtn.size(); i++) {
        int x = figure->controlBtn[i][0]->x(), y = figure->controlBtn[i][0]->y();
        painter.drawRect(x, y, 4, 4);
    }
    update();
}

void MyWidget::drawPolygonControlBtn() {
    QPainter painter(&tmp);
    painter.setPen(QPen(Qt::black));
    painter.setBrush(QBrush(Qt::white));
    for(int i = 0; i < polygon->controlBtn.size(); i++) {
        int x = polygon->controlBtn[i][0]->x(), y = polygon->controlBtn[i][0]->y();
        painter.drawRect(x, y, 4, 4);
    }
    update();
}

void MyWidget::clearControlBtn() {
 //   QPainter painter(pix);
 //   QPen pen;
    /* 将控制按钮恢复为原来的颜色
    for(int i = 0; i < preControlBtn.size(); i++) {
        int x = figure->controlBtn[i][0]->x(), y = figure->controlBtn[i][0]->y();
        for(int j = 0; j < preControlBtn[i].size(); j++) {
            pen.setColor(preControlBtn[i][j]);
            painter.setPen(pen);
            painter.drawPoint(QPoint(x+j/5, y+j%5));
        }
    }
    update();
    */
}


void MyWidget::clearBuffer() {
//    QPainter painter(pix);
//    QPen pen;
//    pen.setWidth(lineWidth);
    /* 将buffer位置的点恢复为原来的颜色
    for(int i = 0; i < figure->buffer.size(); i++) {
        pen.setColor(preBuffer[i]);
        painter.setPen(pen);
        painter.drawPoint(*figure->buffer[i]);
    }

    update();
    */
}

void MyWidget::clearAll() {
    pix.fill(Qt::white);
    if(figure) {
        figure->border.clear();
        figure->buffer.clear();
        figure->controlBtn.clear();
        figure->~SimpleFigure();
    }
    //preBuffer.clear();
    //preControlBtn.clear();
    mode = NOTHING;
    status = READY;
    tmp = pix;
    withoutBtn = tmp;
    update();
}


int MyWidget::isInControlBtn(QPoint point) {
    /* 点是否在控制按钮范围内
     * @return 控制按钮的序号，如果不在，返回-1
     */
    if(figure) {
    /*
        int i;
        for(i = 0; i < figure->controlBtn.size(); i++) {
            int xMin = figure->controlBtn[i][9]->x(), xMax = figure->controlBtn[i][23]->x();
            int yMin = figure->controlBtn[i][9]->y(), yMax = figure->controlBtn[i][23]->y();
            if(point.x() >= xMin && point.x() <= xMax && point.y() >= yMin && point.y() <= yMax)
                return i;
        }
        if(i == figure->controlBtn.size())
            return -1;
    */
        int i;
        for(i = 0; i < figure->controlBtn.size(); i++) {
            int xMin = figure->controlBtn[i][0]->x(), xMax = figure->controlBtn[i][1]->x();
            int yMin = figure->controlBtn[i][0]->y(), yMax = figure->controlBtn[i][1]->y();
            if(point.x() >= xMin && point.x() <= xMax && point.y() >= yMin && point.y() <= yMax)
                return i;
        }
        if(i == figure->controlBtn.size())
            return -1;
    }

    return -1;       
}

int MyWidget::isInPolygonControlBtn(QPoint point) {
    if(polygon) {
        int i;
        for(i = 0; i < polygon->controlBtn.size(); i++) {
            int xMin = polygon->controlBtn[i][0]->x(), xMax = polygon->controlBtn[i][1]->x();
            int yMin = polygon->controlBtn[i][0]->y(), yMax = polygon->controlBtn[i][1]->y();
            if(point.x() >= xMin && point.x() <= xMax && point.y() >= yMin && point.y() <= yMax)
                return i;
        }
        if(i == polygon->controlBtn.size())
            return -1;
    }
    return -1;
}

bool MyWidget::isInBorder(QPoint point) {
    /* 点是否在图形的外接矩形内
     * @return true or false
     */
    if(figure && !figure->border.empty()) {
        int xMin = figure->border[0], xMax = figure->border[2];
        int yMin = figure->border[1], yMax = figure->border[3];
        if(point.x() >= xMin && point.x() <= xMax && point.y() >= yMin && point.y() <= yMax)
            return true;
    }
    return false;
}

bool MyWidget::isInPolygonBorder(QPoint point) {
    if(polygon && !polygon->border.empty()) {
        int xMin = polygon->border[0], xMax = polygon->border[2];
        int yMin = polygon->border[1], yMax = polygon->border[3];
        if(point.x() >= xMin && point.x() <= xMax && point.y() >= yMin && point.y() <= yMax)
            return true;
    }
    return false;
}

bool MyWidget::isOnLine(QPoint point) {
    for(int i = 0; i < figure->buffer.size(); i++) {
        if(abs(point.x()-figure->buffer[i]->x())<=3 && abs(point.y()-figure->buffer[i]->y()) <= 3)
            return true;
    }
    return false;
}

void MyWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, 800, 600, tmp);
}




