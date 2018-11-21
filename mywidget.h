#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include "simplefigure.h"
#include "line.h"
#include "circle.h"
#include "ellipse.h"
#include "rectangle.h"

namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = nullptr);
    ~MyWidget();
    void drawLine();
    void drawCircle();
    void pencilDraw();
    void drawEllipse();
    void drawRectangle();
    void drawBuffer();
    void clearBuffer();
    int mode;
    int lineWidth;
    QColor color;

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::MyWidget *ui;
    QPixmap *pix;
    QPoint startPoint;
    QPoint endPoint;
    SimpleFigure *figure;
    vector<QPoint> exists;      // 保存所有在画布上的点
};

#endif // MYWIDGET_H
