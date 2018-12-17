#include "polygon.h"

Polygon::~Polygon() {
    controlBtn.clear();
    points.clear();
    border.clear();
}

void Polygon::generateControlBtn() {
    vector<QPoint*> btn;
    for(int i = 0; i < points.size(); i++) {
        btn = {new QPoint(points[i].x()-2, points[i].y()-2), new QPoint(points[i].x()+2, points[i].y()+2)};
        controlBtn.push_back(btn);
    }
}

void Polygon::generateBorder() {
    int xMin = 800, yMin = 600, xMax = -1, yMax = -1;
    for(int i = 0; i < points.size(); i++) {
        if(points[i].x() < xMin)
            xMin = points[i].x();
        if(points[i].x() > xMax)
            xMax = points[i].x();
        if(points[i].y() < yMin)
            yMin = points[i].y();
        if(points[i].y() > yMax)
            yMax = points[i].y();
    }
    border = {xMin, yMin, xMax, yMax};
}

QPoint Polygon::getCenter() {
    return QPoint((border[0]+border[2])/2, (border[1]+border[3])/2);
}
