#ifndef CIRCLE_H
#define CIRCLE_H

#include "simplefigure.h"

class Circle : public SimpleFigure {
public:
    ~Circle() {}
    void draw();
    void drawBorder();
    void generateControlBtn();
private:
    int centerX, centerY, r;
};

#endif // CIRCLE_H
