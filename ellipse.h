#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "simplefigure.h"

class Ellipse : public SimpleFigure {
public:
    void draw();
    ~Ellipse() {}
    void generateBorder();
    void generateControlBtn();
    QPoint getCenter();

private:
    int rx, ry, centerX, centerY;
};

#endif // ELLIPSE_H
