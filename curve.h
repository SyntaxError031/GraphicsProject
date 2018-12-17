#ifndef CURVE_H
#define CURVE_H

#include <QPoint>
#include <vector>
using namespace std;

struct Point {
    int x, y;
    Point(int px, int py) { x = px, y = py; }
    bool operator < (const Point& p) const {
        if(x == p.x)
            return (y < p.y);
        else
            return (x < p.x);
    }
};

class Curve {
public:
    Curve();
    bool isFirst;   // 是否是第一条线
    vector<QPoint> controlPoints;   // 控制点
    void draw();
    vector<Point> brezierPoints;    // 计算出的brezier点
    vector<int> border;     // 边界
    int cnt;
    QPoint getCenter();
};

#endif // CURVE_H
