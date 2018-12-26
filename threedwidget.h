#ifndef THREEDWIDGET_H
#define THREEDWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QGLWidget>
#include <QOpenGLFunctions>
#include <GL/glut.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <QDebug>
#include <QErrorMessage>
#include <QPoint>
#include <QMouseEvent>

using namespace std;

struct Vertex {
    float x, y, z;
    Vertex(float xx, float yy, float zz) { x = xx, y = yy, z = zz; }
};

namespace Ui {
class ThreeDWidget;
}

class ThreeDWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit ThreeDWidget(QWidget *parent = nullptr);
    ~ThreeDWidget();
    void readFile(QString path);
    void draw();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    Ui::ThreeDWidget *ui;
    GLfloat rotationX;
    GLfloat rotationY;
    GLfloat rotationZ;
    QPoint lastPos;
    vector<Vertex> vertexes;
    vector<vector<int>> faces;
    int numVertices;
    int numFaces;
    QErrorMessage *errorDlg;
};

#endif // THREEDWIDGET_H
