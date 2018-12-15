#ifndef CIRCLE_H
#define CIRCLE_H

#include "simplefigure.h"

class Circle : public SimpleFigure {
public:
    ~Circle() {}
    void draw();
    void generateBorder();
    void generateControlBtn();
    QPoint getCenter();
private:
    int centerX, centerY, r;
};

#endif // CIRCLE_H
