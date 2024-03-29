#include "mywidget.h"
#include "ui_mywidget.h"
#include <QtDebug>
#include "mainwindow.h"

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    pix = QPixmap(size());
    pix.fill(Qt::white);
    mode = NOTHING;
    status = READY;
    lineWidth = 1;
    rubberBand = NULL;
    figure = NULL;
    polygon = NULL;
    curve = NULL;
    this->setMouseTracking(true);
    button = -1;
    polygonBtn = -1;
    tmp = pix;
    withoutBtn = tmp;
    pixmaps.push_back(pix);
    currentPix = 0;
    isSaved = false;
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
drawing:
        if(status == READY) {
            if(mode == LINE || mode == CIRCLE || mode == ELLIPSE || mode == RECTANGLE) {
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
                    figure = new class Ellipse(); break;
                case RECTANGLE:
                    figure = new class Rectangle(); break;
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
                    polygon = new class Polygon();
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
            else if(mode == CURVE) {
                status = DRAWING;
                if(!curve) {    // 还没有曲线，画第一条边
                    curve = new Curve();
                    curve->isFirst = true;
                    curve->controlPoints.push_back(event->pos());
                    origin = event->pos();
                    if(!rubberBand) {
                        rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
                        rubberBand->setGeometry(QRect(origin, QSize()));
                        rubberBand->show();
                    }
                }
            }
        }
        else if(status == EDITABLE) {

            //if(mode != POLYGON && (button = isInControlBtn(event->pos())) != -1) {      // 在控制按钮上
            if((button = isInControlBtn(event->pos())) != -1) {
                status = EDITING;
                // 进行编辑
                if(mode == CIRCLE || mode == ELLIPSE || mode == RECTANGLE) {
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
            else if(isInBorder(event->pos())) {
            //else if(mode != POLYGON && isInBorder(event->pos())) {     // 在图形区域内
                status = MOVE;
                // TODO: 进行平移
                moveStart = event->pos();

            }
            else if(mode == LINE && isOnLine(event->pos())) {
                status = MOVE;
                moveStart = event->pos();
            }
            //else if(mode == POLYGON && (polygonBtn = isInPolygonControlBtn(event->pos())) != -1) {
              //  status = EDITING;
                // 进行编辑
            //}
            //else if(mode == POLYGON && isInPolygonBorder(event->pos())) {
              //  status = MOVE;
                // 进行平移
                //moveStart = event->pos();
           // }
            else {
                // TODO: buffer中的内容画定在画布上，清空buffer和辅助线
                status = READY;
                if(mode == POLYGON) {
                    //polygon->controlBtn.clear();
                    //polygon->border.clear();
                    polygon->~Polygon();
                    polygon = NULL;
                }
                else if(mode == CURVE) {
                    curve->~Curve();
                    curve = NULL;
                }
                else {
                 //   figure->buffer.clear();
                //clearControlBtn();
                //preControlBtn.clear();
                   // figure->controlBtn.clear();
                    figure->~SimpleFigure();
                    figure = NULL;
                }
                //TODO: 清空辅助线
                //figure->border.clear();
                tmp = withoutBtn;
                pix = tmp;
                update();
                pushPix(pix);
                goto drawing;
            }
        }
    }
    else if(event->button() == Qt::RightButton) {
        if(mode == LINE && status == EDITABLE) {    // 进入裁剪状态
            status = CUT;
            cutStart = event->pos();
            origin = event->pos();
            tmp = withoutBtn;   // 清除控制按钮
            if(!rubberBand) {
                rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
                rubberBand->setGeometry(QRect(origin, QSize()));
                rubberBand->show();
            }
        }
        else if(status == CUT) {
            if((button = isInCutBtn(event->pos())) != -1) {  // 在裁剪区域控制按钮上
                status = CUT_EDITING;
                switch (button) {
                case 0:
                    cutStart = cutArea.bottomRight(); break;
                case 1:
                    cutStart = cutArea.topRight(); break;
                case 2:
                    cutStart = cutArea.bottomLeft(); break;
                case 3:
                    cutStart = cutArea.topLeft(); break;
                default:
                    break;
                }
            }
            else if(isInCutArea(event->pos())) {     // 在裁剪区域内
                status = CUT_MOVE;
                moveStart = event->pos();
            }
            else { // 不在裁剪区域内
                // 确认裁剪
                // TODO: cut, 重画, 画出控制点
                cutLine();
                status = EDITABLE;
            }

        }
    }

}

void MyWidget::mouseMoveEvent(QMouseEvent *event) {
    if(event->buttons() & Qt::LeftButton) {
        if(status == DRAWING) {
            if(mode == LINE || mode == CIRCLE || mode == ELLIPSE || mode == RECTANGLE)
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
            else if(mode == CURVE) {
                if(curve->isFirst) {    // 第一条线
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
            else if(mode == RECTANGLE || mode == CIRCLE || mode == ELLIPSE) {
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
                polygon->points[button] = event->pos();
                tmp = pix;
                drawPolygon();
                withoutBtn = tmp;
                polygon->controlBtn.clear();
                polygon->generateControlBtn();
                drawControlBtn();
                polygon->generateBorder();
            }
            else if(mode == CURVE) {
                curve->controlPoints[button] = event->pos();
                tmp = pix;
                // 重画
                curve->brezierPoints.clear();
                curve->draw();
                drawCurve();
                withoutBtn = tmp;
                drawControlBtn();
            }

            if(mode != POLYGON && mode != CURVE) {
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
            if(mode != POLYGON && mode != CURVE)
                moveFigure(event->pos());
            else if(mode == POLYGON)
                movePolygon(event->pos());
            else
                moveCurve(event->pos());
        }
    }
    else if(event->buttons() & Qt::RightButton) {
        if(status == CUT) {
            rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
        }
        else if(status == CUT_EDITING) {
            tmp = withoutBtn;
            cutEnd = event->pos();
            drawCutArea();
        }
        else if(status == CUT_MOVE) {
            tmp = withoutBtn;
            int deltaX = event->pos().x() - moveStart.x(), deltaY = event->pos().y() - moveStart.y();
            cutStart.setX(cutStart.x() + deltaX);
            cutStart.setY(cutStart.y() + deltaY);
            cutEnd.setX(cutEnd.x() + deltaX);
            cutEnd.setY(cutEnd.y() + deltaY);
            drawCutArea();
            moveStart = event->pos();
        }
    }
    if(mode == LINE || mode == CIRCLE || mode == ELLIPSE || mode == RECTANGLE || mode == PENCIL || mode == FILL || mode == POLYGON || mode == CURVE) {
        if(status == EDITABLE) {
//            if(mode == POLYGON) {
//                if(isInPolygonControlBtn(event->pos()) != -1)
//                    setCursor(Qt::SizeVerCursor);
//                else if(isInPolygonBorder(event->pos()))
//                    setCursor(Qt::SizeAllCursor);
//                else
//                    setCursor(Qt::CrossCursor);
//            }
 //           else {
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
//            }
        }
        else if(status == READY) {
            if(figure) {
                //figure->buffer.clear();
                //clearControlBtn();
                //preControlBtn.clear();
                //figure->controlBtn.clear();
                //TODO: 清空辅助线
                //figure->border.clear();
                figure->~SimpleFigure();
                figure = NULL;
                tmp = withoutBtn;
                pix = tmp;
                tmp = pix;
                pushPix(pix);
                update();
            }
            else if(polygon) {
                polygon->isEnd = true;
                tmp = pix;
                drawPolygon();
                //polygon->border.clear();
                //polygon->points.clear();
                polygon->~Polygon();
                polygon = NULL;
                pix = tmp;
                withoutBtn = tmp;
                tmp = pix;
                update();
                pushPix(pix);
            }
            else if(curve) {
                curve->~Curve();
                curve = NULL;
                tmp = withoutBtn;
                pix = tmp;
                tmp = pix;
                update();
                pushPix(pix);
            }
            setCursor(Qt::CrossCursor);
        }
        else if(status == CUT) {
            if(isInCutBtn(event->pos()) != -1)
                setCursor(Qt::SizeVerCursor);
            else if(isInCutArea(event->pos()))
                setCursor(Qt::SizeAllCursor);
            else
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
            if(mode == LINE || mode == CIRCLE || mode == ELLIPSE || mode == RECTANGLE) {
                rubberBand->hide();
                rubberBand = NULL;
                figure->setEndPoint(event->pos());
                if(figure->getStartPoint() == figure->getEndPoint()) {
                    status = READY;
                    figure->~SimpleFigure();
                    figure = NULL;
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
                        drawControlBtn();
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
            else if(mode == CURVE) {
                tmp = pix;
                if(curve->isFirst && event->pos() != curve->controlPoints[0]) {  // 画第一条线结束
                    curve->isFirst = false;
                    rubberBand->hide();
                    rubberBand = NULL;
                    curve->controlPoints.push_back(event->pos());
                    drawCurve();
                    withoutBtn = tmp;
                }
                else if(event->pos() != curve->controlPoints[0]) {  // 不是第一条线且没画完
                    curve->cnt++;
                    if(curve->cnt < 2) {
                        curve->isFirst = false;
                        curve->controlPoints.insert(curve->controlPoints.end()-1, event->pos());
                        curve->draw();
                        drawCurve();
                        withoutBtn = tmp;
                    }
                    else if(curve->cnt == 2) {
                        curve->controlPoints.insert(curve->controlPoints.end()-1, event->pos());
                        curve->brezierPoints.clear();
                        curve->draw();
                        drawCurve();
                        // TODO: 画出控制点
                        withoutBtn = tmp;
                        drawControlBtn();
                        status = EDITABLE;
                    }

                }
                else {
                    curve->controlPoints.clear();
                    curve = NULL;
                    status = READY;
                }
            }
        }
        else if(status == EDITING || status == MOVE) {
            status = EDITABLE;
            // 编辑完成
        }
    }
    else if(event->button() == Qt::RightButton) {
        if(status == CUT) {
            rubberBand->hide();
            rubberBand = NULL;
            cutEnd = event->pos();
            drawCutArea();
        }
        else if(status == CUT_EDITING || status == CUT_MOVE)
            status = CUT;
    }
}

int test(double p, double q, double *u1, double *u2) {
    int flag = 1;
    double r;
    if(p < 0) {
        r = q / p;
        if(r > *u2)
            flag = 0;
        else if(r > *u1)
            *u1 = r;
    }
    else if(p > 0) {
        r = q / p;
        if(r < *u1)
            flag = 0;
        else if(r < *u2)
            *u2 = r;
    }
    else if(q < 0)
        flag = 0;
    return flag;
}

void MyWidget::cutLine() {
    int xMin = cutArea.left(), xMax = cutArea.right();
    int yMin = cutArea.top(), yMax = cutArea.bottom();
    int x1 = figure->getStartPoint().x(), y1 = figure->getStartPoint().y();
    int x2 = figure->getEndPoint().x(), y2 = figure->getEndPoint().y();
    double u1 = 0.0, u2 = 1.0, dx = x2 - x1, dy;
    if(test(-dx, x1-xMin, &u1, &u2))
        if(test(dx, xMax-x1, &u1, &u2)) {
            dy = y2 - y1;
            if(test(-dy, y1-yMin, &u1, &u2))
                if(test(dy, yMax-y1, &u1, &u2)) {
                    if(u2 < 1.0) {
                        x2 = (int)(x1 + u2*dx);
                        y2 = (int)(y1 + u2*dy);
                    }
                    if(u1 > 0) {
                        x1 = (int)(x1 + u1*dx);
                        y1 = (int)(y1 + u1*dy);
                    }
                }
        }
    tmp = pix;
    figure->buffer.clear();
    figure->controlBtn.clear();
    figure->setStartPoint(QPoint(x1, y1));
    figure->setEndPoint(QPoint(x2, y2));
    figure->draw();
    drawBuffer();
    //withoutBtn = tmp;
    figure->generateControlBtn();
    drawControlBtn();
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
        drawControlBtn();
        polygon->generateBorder();
        moveStart = point;
    }
}

void MyWidget::moveCurve(QPoint point) {
    if(isInWidget(point.x(), point.y())) {
        int deltaX = point.x() - moveStart.x();
        int deltaY = point.y() - moveStart.y();

        for(int i = 0; i < curve->controlPoints.size(); i++) {
            curve->controlPoints[i].setX(curve->controlPoints[i].x() + deltaX);
            curve->controlPoints[i].setY(curve->controlPoints[i].y() + deltaY);
        }

        tmp = pix;
        curve->brezierPoints.clear();
        curve->draw();
        drawCurve();
        withoutBtn = tmp;
        drawControlBtn();
        moveStart = point;
    }
}

void MyWidget::rotate(int value) {
    if(status == EDITABLE && (mode == RECTANGLE || mode == ELLIPSE || mode == LINE || mode == CIRCLE)) {
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
        //figure->buffer.clear();
        //figure->border.clear();
        figure->~SimpleFigure();
        figure = NULL;
        status = READY;
        pix = tmp;
        pushPix(pix);
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
        //polygon->points.clear();
        polygon->~Polygon();
        polygon = NULL;
        status = READY;
        pix = tmp;
        pushPix(pix);
    }
    if(status == EDITABLE && mode == CURVE) {
        curve->brezierPoints.clear();
        tmp = pix;
        int centerX = curve->getCenter().x(), centerY = curve->getCenter().y();
        double radian = (double)value*3.14159 / 180.0;
        for(int i = 0; i < curve->controlPoints.size(); i++) {
            int x = curve->controlPoints[i].x(), y = curve->controlPoints[i].y();
            curve->controlPoints[i].setX(centerX+(x-centerX)*cos(radian) - (y-centerY)*sin(radian));
            curve->controlPoints[i].setY(centerY+(x-centerX)*sin(radian) + (y-centerY)*cos(radian));
        }

        curve->draw();
        drawCurve();
        withoutBtn = tmp;

        curve->~Curve();
        curve = NULL;
        status = READY;
        pix = tmp;
        pushPix(pix);
    }

}


void MyWidget::zoomIn() {
    if(status == EDITABLE && (mode == CIRCLE || mode == ELLIPSE || mode == RECTANGLE)) {
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
        drawControlBtn();
        polygon->generateBorder();
        status = EDITABLE;
    }
}

void MyWidget::zoomOut() {
    if(status == EDITABLE && (mode == CIRCLE || mode == ELLIPSE || mode == RECTANGLE)) {
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
        drawControlBtn();
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
    pushPix(pix);
    update();
}

bool MyWidget::isInWidget(int x, int y) {
    return (x >= 0 && x < 800 && y >= 0 && y < 600);
}

void MyWidget::changeWidth(int width) {
    lineWidth = width;

    if(status == EDITABLE) {
        tmp = pix;
        if(figure) {
            drawBuffer();
        }
        else if(polygon) {
            drawPolygon();
        }
        else if(curve) {
            drawCurve();
        }
        withoutBtn = tmp;
        drawControlBtn();
    }
    else if(status == DRAWING) {
        tmp = pix;
        if(polygon)
            drawPolygon();
        else if(curve)
            drawCurve();
        withoutBtn = tmp;
    }
}

void MyWidget::changeColor(QColor c) {
    color = c;

    if(status == EDITABLE) {
        tmp = pix;
        if(figure) {
            drawBuffer();
        }
        else if(polygon) {
            drawPolygon();
        }
        else if(curve) {
            drawCurve();
        }
        withoutBtn = tmp;
        drawControlBtn();
    }
    else if(status == DRAWING) {
        tmp = pix;
        if(polygon)
            drawPolygon();
        else if(curve)
            drawCurve();
        withoutBtn = tmp;
    }
}

void MyWidget::pushPix(QPixmap pixmap) {
    int size = pixmaps.size();
    if(currentPix < size - 1) {
        for(int i = size - 1; i >= currentPix+1; i--) {
            pixmaps.erase(pixmaps.begin() + i);
        }
    }
    pixmaps.push_back(pixmap);
    currentPix++;
}

void MyWidget::undo() {
    if(status == EDITABLE) {
        pushPix(withoutBtn);
        status = READY;
    }
    else if(status == DRAWING) {
        // 曲线和多边形正在绘制

        if(polygon) {
            // 把图画完
            polygon->isEnd = true;
            tmp = pix;
            drawPolygon();
            pix = tmp;
            withoutBtn = tmp;
            tmp = pix;
            pushPix(pix);
            polygon->~Polygon();
            polygon = NULL;
        }
        else if(curve) {
            pushPix(withoutBtn);
            curve->~Curve();
            curve = NULL;
        }
        status = READY;
    }
    if(currentPix > 0) {
        currentPix--;
        tmp = pixmaps[currentPix];
        withoutBtn = tmp;
        pix = tmp;
        update();
    }
}

void MyWidget::redo() {
    if(currentPix < (int)pixmaps.size()-1) {
        currentPix++;
        tmp = pixmaps[currentPix];
        withoutBtn = tmp;
        pix = tmp;
        update();
    }
}

void MyWidget::drawBuffer() {

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

void MyWidget::drawCurve() {
    QPainter painter(&tmp);
    QPen pen;
    pen.setWidth(lineWidth);
    pen.setColor(color);
    painter.setPen(pen);

    if(curve->controlPoints.size() == 2) {
        painter.drawLine(curve->controlPoints[0], curve->controlPoints[1]);
    }
    else {
        for(int i = 1; i < curve->brezierPoints.size(); i++) {
            QPoint p1(curve->brezierPoints[i].x, curve->brezierPoints[i].y);
            QPoint p2(curve->brezierPoints[i-1].x, curve->brezierPoints[i-1].y);
            painter.drawLine(p1, p2);
        }
    }
    update();
}

void MyWidget::drawCutArea() {
    cutArea = QRect(cutStart, cutEnd).normalized();
    QPainter painter(&tmp);
    QPen pen;
    pen.setColor(Qt::blue);
    pen.setStyle(Qt::DashLine);
    painter.setPen(pen);
    painter.drawRect(cutArea);

    painter.setPen(QPen(Qt::black));
    painter.setBrush(QBrush(Qt::white));
    QPoint tmp = cutArea.topLeft();
    painter.drawRect(tmp.x()-2, tmp.y()-2, 4, 4);
    tmp = cutArea.topRight();
    painter.drawRect(tmp.x()-2, tmp.y()-2, 4, 4);
    tmp = cutArea.bottomLeft();
    painter.drawRect(tmp.x()-2, tmp.y()-2, 4, 4);
    tmp = cutArea.bottomRight();
    painter.drawRect(tmp.x()-2, tmp.y()-2, 4, 4);

    update();
}

void MyWidget::drawControlBtn() {

    QPainter painter(&tmp);
    painter.setPen(QPen(Qt::black));
    painter.setBrush(QBrush(Qt::white));
    if(figure) {
        for(int i = 0; i < figure->controlBtn.size(); i++) {
            int x = figure->controlBtn[i][0]->x(), y = figure->controlBtn[i][0]->y();
            painter.drawRect(x, y, 4, 4);
        }
    }
    else if(polygon) {
        for(int i = 0; i < polygon->controlBtn.size(); i++) {
            int x = polygon->controlBtn[i][0]->x(), y = polygon->controlBtn[i][0]->y();
            painter.drawRect(x, y, 4, 4);
        }
    }
    else if(curve) {
        for(int i = 0; i < curve->controlPoints.size(); i++) {
            int x = curve->controlPoints[i].x() - 2, y = curve->controlPoints[i].y() - 2;
            painter.drawRect(x, y, 4, 4);
        }
    }
    update();
}


void MyWidget::newBoard() {
    clearAll();
    pixmaps.clear();
    pixmaps.push_back(pix);
    currentPix = 0;
    lineWidth = 1;
    button = -1;
    isSaved = false;
}

void MyWidget::save(QString path) {
    isSaved = true;
    if(status == EDITABLE) {
        pushPix(withoutBtn);
        status = READY;
    }
    else if(status == DRAWING) {
        // 曲线和多边形正在绘制

        if(polygon) {
            // 把图画完
            polygon->isEnd = true;
            tmp = pix;
            drawPolygon();
            pix = tmp;
            withoutBtn = tmp;
            tmp = pix;
            pushPix(pix);
            polygon->~Polygon();
            polygon = NULL;
        }
        else if(curve) {
            pushPix(withoutBtn);
            curve->~Curve();
            curve = NULL;
        }
        status = READY;
    }
    tmp = withoutBtn;
    pix = tmp;
    pix.save(path, "JPG");
}

void MyWidget::open(QString path) {
    newBoard();
    QPixmap img;
    img.load(path);
    //tmp.load(path);
    tmp = img;
    pixmaps.clear();
    pix = tmp;
    withoutBtn = tmp;
    pixmaps.push_back(pix);
    currentPix = 0;
    update();
}


void MyWidget::clearAll() {
    pix.fill(Qt::white);
    if(figure) {
        figure->~SimpleFigure();
        figure = NULL;
    }
    if(curve) {
        curve->~Curve();
        curve = NULL;
    }
    if(polygon) {
        polygon->~Polygon();
        polygon = NULL;
    }
    //preBuffer.clear();
    //preControlBtn.clear();
    mode = NOTHING;
    status = READY;
    tmp = pix;
    withoutBtn = tmp;
    pushPix(pix);
    update();
}


int MyWidget::isInControlBtn(QPoint point) {
    /* 点是否在控制按钮范围内
     * @return 控制按钮的序号，如果不在，返回-1
     */
    int i;
    if(figure) {
        for(i = 0; i < figure->controlBtn.size(); i++) {
            int xMin = figure->controlBtn[i][0]->x(), xMax = figure->controlBtn[i][1]->x();
            int yMin = figure->controlBtn[i][0]->y(), yMax = figure->controlBtn[i][1]->y();
            if(point.x() >= xMin && point.x() <= xMax && point.y() >= yMin && point.y() <= yMax)
                return i;
        }
        if(i == figure->controlBtn.size())
            return -1;
    }
    else if(polygon) {
        //int i;
        for(i = 0; i < polygon->controlBtn.size(); i++) {
            int xMin = polygon->controlBtn[i][0]->x(), xMax = polygon->controlBtn[i][1]->x();
            int yMin = polygon->controlBtn[i][0]->y(), yMax = polygon->controlBtn[i][1]->y();
            if(point.x() >= xMin && point.x() <= xMax && point.y() >= yMin && point.y() <= yMax)
                return i;
        }
        if(i == polygon->controlBtn.size())
            return -1;
    }
    else if(curve) {
        for(int i = 0; i < curve->controlPoints.size(); i++) {
            int xMin = curve->controlPoints[i].x() - 2, xMax = curve->controlPoints[i].x() + 2;
            int yMin = curve->controlPoints[i].y() - 2, yMax = curve->controlPoints[i].y() + 2;
            if(point.x() >= xMin && point.x() <= xMax && point.y() >= yMin && point.y() <= yMax)
                return i;
        }
        if(i == curve->controlPoints.size())
            return -1;
    }

    return -1;       
}

int MyWidget::isInCutBtn(QPoint point) {
    if(abs(point.x() - cutArea.topLeft().x()) <= 2 && abs(point.y() - cutArea.topLeft().y()) <= 2)
        return 0;
    if(abs(point.x() - cutArea.bottomLeft().x()) <= 2 && abs(point.y() - cutArea.bottomLeft().y()) <= 2)
        return 1;
    if(abs(point.x() - cutArea.topRight().x()) <= 2 && abs(point.y() - cutArea.topRight().y()) <= 2)
        return 2;
    if(abs(point.x() - cutArea.bottomRight().x()) <= 2 && abs(point.y() - cutArea.bottomRight().y()) <= 2)
        return 3;
    return -1;
}

bool MyWidget::isInCutArea(QPoint point) {
    if(point.x() >= cutArea.left() && point.x() <= cutArea.right() &&
            point.y() >= cutArea.top() && point.y() <= cutArea.bottom())
        return true;
    return false;
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
    if(polygon && !polygon->border.empty()) {
        int xMin = polygon->border[0], xMax = polygon->border[2];
        int yMin = polygon->border[1], yMax = polygon->border[3];
        if(point.x() >= xMin && point.x() <= xMax && point.y() >= yMin && point.y() <= yMax)
            return true;
    }
    if(curve && !curve->border.empty()) {
        int xMin = curve->border[0], xMax = curve->border[2];
        int yMin = curve->border[1], yMax = curve->border[3];
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
    painter.drawPixmap(QPoint(0,0), tmp);
}




