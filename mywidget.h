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
#include <stack>
#include <set>
#include <queue>

struct Point {
    int x, y;
    Point(int px, int py) { x = px, y = py; }
    bool operator < (const Point& p) const {
        if(x == p.x)
            return (y < p.y);
        else
            return (x < p.x);
    }
};

namespace Ui {
class MyWidget;
}

enum {NOTHING, PENCIL, LINE, CIRCLE, RECT, ELLIPSE, FILL};
enum Status {READY, DRAWING, MOVE, EDITING, EDITABLE};

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
    bool isInBorder(QPoint point);
    bool isOnLine(QPoint point);
    void fill(QPoint seed);
    void clearAll();
    void moveFigure(QPoint point);
    void rotate(int value);
    //void zoomIn();

    int mode;
    int lineWidth;
    QColor color;
    Status status;
    //int ratio;

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::MyWidget *ui;
    QPixmap pix;
    QPixmap tmp;
    QPixmap withoutBtn;
    QPoint origin;
    SimpleFigure *figure;
    vector<QColor> preBuffer;      // 保存编辑状态的点原来的颜色
    QRubberBand *rubberBand;
    vector<vector<QColor>> preControlBtn;    // 存储绘制为控制按钮点以前的颜色
    bool isInWidget(int x, int y);
    int button;  // 编辑时按了哪个controlBtn
    QPoint moveStart;
};



#endif // MYWIDGET_H
