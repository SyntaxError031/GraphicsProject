#include "curve.h"

Curve::Curve() {
    cnt = 0;
}

void Curve::draw() {
    double u;
    int x, y;
//    controlPoints.push_back(QPoint(100,100));
//    controlPoints.push_back(QPoint(200,400));
//    controlPoints.push_back(QPoint(300,100));
//    controlPoints.push_back(QPoint(400,100));

    int xMin = 800, yMin = 600, xMax = -1, yMax = -1;
    for(int i = 0; i < controlPoints.size(); i++) {
        if(controlPoints[i].x() < xMin)
            xMin = controlPoints[i].x();
        if(controlPoints[i].x() > xMax)
            xMax = controlPoints[i].x();
        if(controlPoints[i].y() < yMin)
            yMin = controlPoints[i].y();
        if(controlPoints[i].y() > yMax)
            yMax = controlPoints[i].y();
    }
    border = {xMin, yMin, xMax, yMax};

    int precision = qMax(xMax - xMin, yMax - xMax) * 2;

    for(int k = 0; k <= precision; k++) {
        u = (double)k / (double)precision;
        if(controlPoints.size() == 4) {
            x = (int)(controlPoints[0].x()*(1-u)*(1-u)*(1-u) + controlPoints[1].x()*3*u*(1-u)*(1-u) + \
                    controlPoints[2].x()*3*u*u*(1-u) + controlPoints[3].x()*u*u*u);
            y = (int)(controlPoints[0].y()*(1-u)*(1-u)*(1-u) + controlPoints[1].y()*3*u*(1-u)*(1-u) + \
                    controlPoints[2].y()*3*u*u*(1-u) + controlPoints[3].y()*u*u*u);
        }
        else if(controlPoints.size() == 3) {
            x = (int)(controlPoints[0].x()*(1-u)*(1-u) + controlPoints[1].x()*2*u*(1-u) + \
                    controlPoints[2].x()*u*u);
            y = (int)(controlPoints[0].y()*(1-u)*(1-u) + controlPoints[1].y()*2*u*(1-u) + \
                    controlPoints[2].y()*u*u);
        }
        brezierPoints.push_back(Point(x, y));
    }
}

QPoint Curve::getCenter() {
    return QPoint((border[0]+border[2])/2, (border[1]+border[3])/2);
}
