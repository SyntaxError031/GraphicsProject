#ifndef CIRCLE_H
#define CIRCLE_H

#include "simplefigure.h"

class Circle : public SimpleFigure {
public:
    ~Circle() {}
    void draw();
    void drawBorder();
    void generateControlBtn();
};

#endif // CIRCLE_H
