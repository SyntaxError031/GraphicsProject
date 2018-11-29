#include "ellipse.h"

void Ellipse::draw() {
    /* 中点椭圆生成算法
     * 鼠标左键按下并拖动形成的矩形的内切椭圆
     */

    rx = abs(startPoint.x()-endPoint.x())/2;
    ry = abs(startPoint.y()-endPoint.y())/2;
    centerX = (startPoint.x()+endPoint.x()) / 2;
    centerY = (startPoint.y()+endPoint.y()) / 2;
    int x = 0, y = ry, p = 4*ry*ry-4*rx*rx*ry-rx*rx;
    while(ry*ry*x < rx*rx*y) {     // 斜率绝对值小于1
        buffer.push_back(new QPoint(x+centerX, y+centerY));
        buffer.push_back(new QPoint(-x+centerX, y+centerY));
        buffer.push_back(new QPoint(-x+centerX, -y+centerY));
        buffer.push_back(new QPoint(x+centerX, -y+centerY));
        if(p >= 0) {
            p += (4*ry*ry*(2*x+3)+rx*rx*(8-8*y));
            y--;
        }
        else {
            p += 4*ry*ry*(2*x+3);
        }
        x++;
    }
    while(y >= 0) {
        buffer.push_back(new QPoint(x+centerX, y+centerY));
        buffer.push_back(new QPoint(-x+centerX, y+centerY));
        buffer.push_back(new QPoint(-x+centerX, -y+centerY));
        buffer.push_back(new QPoint(x+centerX, -y+centerY));
        if(p >= 0) {
            p += (4*rx*rx*(3-2*y));
        }
        else {
            p += (ry*ry*(8*x+8)+4*rx*rx*(3-2*y));
            x++;
        }
        y--;
    }
}

void Ellipse::generateBorder() {
    border = {centerX-rx, centerY-ry, centerX+rx, centerY+ry};
}

void Ellipse::generateControlBtn() {
    if(startPoint == endPoint)
        return ;
    vector<QPoint*> btn;
    btn = {new QPoint(border[0]-2, border[1]-2), new QPoint(border[0]+2, border[1]+2)};
    controlBtn.push_back(btn);
    btn = {new QPoint(border[0]-2, border[3]-2), new QPoint(border[0]+2, border[3]+2)};
    controlBtn.push_back(btn);
    btn = {new QPoint(border[2]-2, border[1]-2), new QPoint(border[2]+2, border[1]+2)};
    controlBtn.push_back(btn);
    btn = {new QPoint(border[2]-2, border[3]-2), new QPoint(border[2]+2, border[3]+2)};
    controlBtn.push_back(btn);

}
