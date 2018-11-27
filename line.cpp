#include "line.h"

void Line::draw() {

    /* 中点直线生成算法
     * 鼠标左键按下为起点，松开为终点形成的直线
     */

    int x1, x2, y1, y2;
    QPoint tmp;
    if(startPoint.x() <= endPoint.x()) {
        x1 = startPoint.x(); x2 = endPoint.x();
        y1 = startPoint.y(); y2 = endPoint.y();
    }
    else {
        x1 = endPoint.x(); x2 = startPoint.x();
        y1 = endPoint.y(); y2 = startPoint.y();
    }

    int a = y1 - y2, b = x2 - x1;
    int d, delta1, delta2;
    int x = x1, y = y1;
    buffer.push_back(new QPoint(x, y));
//    tmp.setX(x), tmp.setY(y);
//    buffer.push_back(tmp);

    // k does not exist
    if(x1 == x2) {
        for(y = qMin(y1, y2); y <= qMax(y1, y2); y++) {
//            tmp.setX(x), tmp.setY(y);
//            buffer.push_back(tmp);
            buffer.push_back(new QPoint(x, y));
        }
    }
    else {
        // 0 <= k <= 1
        if(y1 >= y2 && (y1-y2 <= x2-x1)) {
            d = 2*a - b;
            delta1 = 2*a; delta2 = 2*(a-b);
            while(x < x2) {
                if(d < 0) {
                    x++;
                    d += delta1;
                } else {
                    x++; y--;
                    d += delta2;
                }
//                tmp.setX(x), tmp.setY(y);
//                buffer.push_back(tmp);
                buffer.push_back(new QPoint(x, y));
            }
        }
        // k > 1
        else if(y1 > y2 && (y1-y2 > x2-x1)) {
            d = a - 2*b;
            delta1 = -2*b; delta2 = 2*(a-b);
            while(y > y2) {
                if(d < 0) {
                    y--; x++;
                    d += delta2;
                } else {
                    y--;
                    d += delta1;
                }
//                tmp.setX(x), tmp.setY(y);
//                buffer.push_back(tmp);
                buffer.push_back(new QPoint(x, y));
            }
        }
        // -1 <= k < 0
        else if(y1 < y2 && (y2-y1 <= x2-x1)) {
            d = 2*a + b;
            delta1 = 2*a; delta2 = 2*(a+b);
            while(x < x2) {
                if(d < 0) {
                    x++; y++;
                    d += delta2;
                } else {
                    x++;
                    d += delta1;
                }
//                tmp.setX(x), tmp.setY(y);
//                buffer.push_back(tmp);
                buffer.push_back(new QPoint(x, y));
            }
        }
        // k < -1
        else if(y1 < y2 && (y2-y1 > x2-x1)) {
            d = a + 2*b;
            delta1 = 2*b; delta2 = 2*(a+b);
            while(y < y2) {
                if(d < 0) {
                    y++;
                    d += delta1;
                } else {
                    y++; x++;
                    d += delta2;
                }
//                tmp.setX(x), tmp.setY(y);
//                buffer.push_back(tmp);
                buffer.push_back(new QPoint(x, y));
            }
        }
    }
}

void Line::generateControlBtn() {
    /* 生成起点和终点的编辑按钮
     */

    int sx, sy, ex, ey;
    if(startPoint.x() < endPoint.x()) {
        sx = startPoint.x(), sy = startPoint.y();
        ex = endPoint.x(), ey = endPoint.y();
    } else {
        sx = endPoint.x(), sy = endPoint.y();
        ex = startPoint.x(), ey = startPoint.y();
    }

    vector<QPoint*> btn; /*
    btn = {new QPoint(sx-3, sy-1), new QPoint(sx-3,sy), new QPoint(sx-3,sy+1), \
           new QPoint(sx-2, sy-1), new QPoint(sx-2,sy), new QPoint(sx-2,sy+1), \
           new QPoint(sx-1, sy-1), new QPoint(sx-1,sy), new QPoint(sx-1,sy+1), \
           new QPoint(sx-4,sy-2), new QPoint(sx-4,sy-1), new QPoint(sx-4, sy), new QPoint(sx-4, sy+1), new QPoint(sx-4,sy+2),\
           new QPoint(sx-3, sy-2), new QPoint(sx-3, sy+2), new QPoint(sx-2, sy-2), new QPoint(sx-2, sy+2), \
           new QPoint(sx-1, sy-2), new QPoint(sx-1, sy+2), \
           new QPoint(sx, sy-2), new QPoint(sx, sy-1), new QPoint(sx, sy+1), new QPoint(sx, sy+2)};
    controlBtn.push_back(btn);
    btn = {new QPoint(ex-3, ey-1), new QPoint(ex-3,ey), new QPoint(ex-3,ey+1), \
           new QPoint(ex-2, ey-1), new QPoint(ex-2,ey), new QPoint(ex-2,ey+1), \
           new QPoint(ex-1, ey-1), new QPoint(ex-1,ey), new QPoint(ex-1,ey+1), \
           new QPoint(ex-4,ey-2), new QPoint(ex-4,ey-1), new QPoint(ex-4, ey), new QPoint(ex-4, ey+1), new QPoint(ex-4,ey+2),\
           new QPoint(ex-3, ey-2), new QPoint(ex-3, ey+2), new QPoint(ex-2, ey-2), new QPoint(ex-2, ey+2), \
           new QPoint(ex-1, ey-2), new QPoint(ex-1, ey+2), \
           new QPoint(ex, ey-2), new QPoint(ex, ey-1), new QPoint(ex, ey+1), new QPoint(ex, ey+2)};
    controlBtn.push_back((btn));
    */
    btn = {new QPoint(sx-4, sy-2), new QPoint(sx, sy+2)};
    controlBtn.push_back(btn);
    btn = {new QPoint(ex-4, ey-2), new QPoint(ex, ey+2)};
    controlBtn.push_back(btn);
}
