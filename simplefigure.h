#ifndef SIMPLEFIGURE_H
#define SIMPLEFIGURE_H

#include <QPoint>
#include <vector>
#include <iostream>
#include <QImage>
using namespace std;

class SimpleFigure {
public:
    virtual void generateControlBtn() = 0;
    virtual void draw() = 0;
    void setStartPoint(QPoint point) { startPoint = point; }
    void setEndPoint(QPoint point) { endPoint = point; }
    vector<QPoint*> buffer;     // 存储处于编辑状态的点
    vector<vector<QPoint*>> controlBtn;     // 存储表示控制按钮的点
    virtual ~SimpleFigure() {}
protected:
    QPoint startPoint;
    QPoint endPoint;
};

#endif // SIMPLEFIGURE_H
