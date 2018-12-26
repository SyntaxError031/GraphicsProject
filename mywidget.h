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
#include "polygon.h"
#include "curve.h"
#include <QRubberBand>
#include <stack>
#include <set>
#include <queue>
#include <string>


namespace Ui {
class MyWidget;
}

enum {NOTHING, PENCIL, LINE, CIRCLE, RECTANGLE, ELLIPSE, FILL, POLYGON, CURVE};
enum Status {READY, DRAWING, MOVE, EDITING, EDITABLE, CUT, CUT_READY, CUT_DRAWING, CUT_EDITABLE, CUT_MOVE, CUT_EDITING};

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = nullptr);
    ~MyWidget();

    void drawBuffer();
    void drawPolygon();
    void drawCurve();
    void drawControlBtn();

    int isInControlBtn(QPoint point);
    bool isInBorder(QPoint point);
    bool isOnLine(QPoint point);

    void fill(QPoint seed);
    void clearAll();
    void moveFigure(QPoint point);
    void movePolygon(QPoint point);
    void moveCurve(QPoint point);
    void rotate(int value);
    void zoomIn();
    void zoomOut();
    void changeColor(QColor color);
    void changeWidth(int width);
    void undo();
    void redo();
    void cutLine();
    void newBoard();
    void save(QString path);
    void open(QString path);

    int mode;
    int lineWidth;
    QColor color;
    Status status;
    bool isSaved;
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
    QRubberBand *rubberBand;
    bool isInWidget(int x, int y);
    int button;  // 编辑时按了哪个controlBtn
    int polygonBtn;
    QPoint moveStart;
    Polygon *polygon;
    Curve *curve;
    vector<QPixmap> pixmaps;     // 保存画布、
    int currentPix;     // 当前的画布
    void pushPix(QPixmap pixmap);
    QRect cutArea;
    QPoint cutStart;
    QPoint cutEnd;
    void drawCutArea();
    int isInCutBtn(QPoint point);
    bool isInCutArea(QPoint point);
};



#endif // MYWIDGET_H
