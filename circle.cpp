#include "circle.h"

void Circle::draw() {
    /* Bresenham 画圆算法
     * 鼠标按下时的点为起始点，松开时的点为终点，形成的直线即为圆的直径
     */

    int centerX, centerY, r;
    int x, y, p;
    centerX = (startPoint.x()+endPoint.x()) / 2;
    centerY = (startPoint.y()+endPoint.y()) / 2;
    r = (int)(sqrt((startPoint.x()-endPoint.x())*(startPoint.x()-endPoint.x()) + \
             (startPoint.y()-endPoint.y())*(startPoint.y()-endPoint.y()))/2.0);
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
