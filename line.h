#ifndef LINE_H
#define LINE_H

#include "simplefigure.h"

class Line : public SimpleFigure {
public:
    ~Line() {}
    void draw();
    void generateControlBtn();
    void drawBorder();
    QPoint getCenter();
};

#endif // LINE_H
