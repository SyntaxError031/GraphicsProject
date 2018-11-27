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
#include <QRubberBand>

namespace Ui {
class MyWidget;
}

enum {NOTHING, PENCIL, LINE, CIRCLE, RECT, ELLIPSE};
enum Status {READY, DRAWING, EDITING, EDITABLE};

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
    void drawControlBtn();
    void clearControlBtn();
    int isInControlBtn(QPoint point);
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
    QPoint origin;
    SimpleFigure *figure;
    vector<QColor> preBuffer;      // 保存编辑状态的点原来的颜色
    QRubberBand *rubberBand;
    vector<vector<QColor>> preControlBtn;    // 存储绘制为控制按钮点以前的颜色
    Status status;
};



#endif // MYWIDGET_H
