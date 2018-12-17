#ifndef POLYGON_H
#define POLYGON_H

#include <QPoint>
#include <vector>
using namespace std;

class Polygon {
public:
    bool isFirst;   // 是否为第一条线
    bool isEnd;     // 是否为最后一条线
    vector<QPoint> points;  // 多边形每个顶点
    vector<vector<QPoint*>> controlBtn;
    vector<int> border;
    void generateControlBtn();
    void generateBorder();
    QPoint getCenter();
};

#endif // POLYGON_H
