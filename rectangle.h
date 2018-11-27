#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "simplefigure.h"

class Rectangle : public SimpleFigure {
public:
    void draw();
    ~Rectangle() {}
    void drawBorder();
    void generateControlBtn();

private:
    int xMin, yMin, xMax, yMax;
};

#endif // RECTANGLE_H
