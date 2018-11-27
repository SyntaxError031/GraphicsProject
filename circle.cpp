#include "circle.h"

void Circle::draw() {
    /* Bresenham 画圆算法
     * 鼠标左键按下并拖动形成的矩形的内切圆(圆心靠近起点)
     */

    int x, y, p;
    r = qMin(abs(startPoint.x()-endPoint.x()), abs(startPoint.y()-endPoint.y())) / 2;
    centerX = (startPoint.x() < endPoint.x()) ? (startPoint.x() + r) : (startPoint.x() - r);
    centerY = (startPoint.y() < endPoint.y()) ? (startPoint.y() + r) : (startPoint.y() - r);

    x = 0; y = r;
    p = 3 - 2*r;
    for(; x <= y; x++) {
        buffer.push_back(new QPoint(x+centerX, y+centerY));
        buffer.push_back(new QPoint(y+centerX, x+centerY));
        buffer.push_back(new QPoint(y+centerX, -x+centerY));
        buffer.push_back(new QPoint(x+centerX, -y+centerY));
        buffer.push_back(new QPoint(-x+centerX, -y+centerY));
        buffer.push_back(new QPoint(-y+centerX, -x+centerY));
        buffer.push_back(new QPoint(-y+centerX, x+centerY));
        buffer.push_back(new QPoint(-x+centerX, y+centerY));
        if(p >= 0) {
            p += (4*(x-y) + 10);
            y--;
        } else
            p += (4*x + 6);
    }
}

void Circle::drawBorder() {

}

void Circle::generateControlBtn() {
    vector<QPoint*> btn;
    btn = {new QPoint(centerX-r-2, centerY-r-2), new QPoint(centerX-r+2, centerY-r+2)};
    controlBtn.push_back(btn);
    btn = {new QPoint(centerX-r-2, centerY+r-2), new QPoint(centerX-r+2, centerY+r+2)};
    controlBtn.push_back(btn);
    btn = {new QPoint(centerX+r-2, centerY-r-2), new QPoint(centerX+r+2, centerY-r+2)};
    controlBtn.push_back(btn);
    btn = {new QPoint(centerX+r-2, centerY+r-2), new QPoint(centerX+r+2, centerY+r+2)};
    controlBtn.push_back(btn);


}
