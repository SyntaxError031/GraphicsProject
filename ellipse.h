#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "simplefigure.h"

class Ellipse : public SimpleFigure {
public:
    void draw();
    ~Ellipse() {}
    void drawBorder();
    void generateControlBtn();
};

#endif // ELLIPSE_H
