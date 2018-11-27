#include "rectangle.h"

void Rectangle::draw() {
    xMin = qMin(startPoint.x(), endPoint.x());
    xMax = qMax(startPoint.x(), endPoint.x());
    yMax = qMax(startPoint.y(), endPoint.y());
    yMin = qMin(startPoint.y(), endPoint.y());

    for(int i = xMin; i <= xMax; i++) {
        buffer.push_back(new QPoint(i, yMin));
        buffer.push_back(new QPoint(i, yMax));
    }
    for(int i = yMin; i < yMax; i++) {
        buffer.push_back(new QPoint(xMin, i));
        buffer.push_back(new QPoint(xMax, i));
    }
}

void Rectangle::generateBorder() {
    border = {xMin, yMin, xMax, yMax};
}

void Rectangle::generateControlBtn() {
    vector<QPoint*> btn;
    btn = {new QPoint(xMin-4, yMin-2), new QPoint(xMin, yMin+2)};
    controlBtn.push_back(btn);
    btn = {new QPoint(xMin-4, yMax-2), new QPoint(xMin, yMax+2)};
    controlBtn.push_back(btn);
    btn = {new QPoint(xMax, yMin-2), new QPoint(xMax+4, yMin+2)};
    controlBtn.push_back(btn);
    btn = {new QPoint(xMax, yMax-2), new QPoint(xMax+4, yMax+2)};
    controlBtn.push_back(btn);
}
