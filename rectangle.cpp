#include "rectangle.h"

void Rectangle::draw() {
    int xMin = qMin(startPoint.x(), endPoint.x());
    int xMax = qMax(startPoint.x(), endPoint.x());
    int yMax = qMax(startPoint.y(), endPoint.y());
    int yMin = qMin(startPoint.y(), endPoint.y());

    for(int i = xMin; i <= xMax; i++) {
        buffer.push_back(new QPoint(i, yMin));
        buffer.push_back(new QPoint(i, yMax));
    }
    for(int i = yMin; i < yMax; i++) {
        buffer.push_back(new QPoint(xMin, i));
        buffer.push_back(new QPoint(xMax, i));
    }
}
