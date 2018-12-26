#include "threedwidget.h"
#include "ui_threedwidget.h"

ThreeDWidget::ThreeDWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    ui(new Ui::ThreeDWidget)
{
    ui->setupUi(this);
    rotationX = 0;
    rotationY = 0;
    rotationZ = 0;

    errorDlg = new QErrorMessage(this);

}

ThreeDWidget::~ThreeDWidget()
{
    delete ui;
}

void ThreeDWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

//    glEnable(GL_LIGHT0);
//    glEnable(GL_LIGHTING);
//    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
//    glEnable(GL_COLOR_MATERIAL);

}
void ThreeDWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw();
}

void ThreeDWidget::resizeGL(int width, int height)
{
//    glViewport(0,0,width,height);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glOrtho(-width/2,width/2,-height/2,height/2,-1,1);
//    glMatrixMode(GL_MODELVIEW);
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat x = GLfloat(width) / height;
    glFrustum(-x,+x,-1.0,+1.0,8.0,30.0);
    glMatrixMode(GL_MODELVIEW);
}

void ThreeDWidget::readFile(QString path) {
    if(!faces.empty() || !vertexes.empty()) {
        faces.clear();
        vertexes.clear();
    }
    ifstream in_file(path.toStdString(), ios::in);
    if(!in_file) {
        errorDlg->setWindowTitle(tr("错误"));
        errorDlg->showMessage(tr("无法打开文件"));
    }
    string flag;
    in_file >> flag;
    int ignore;
    in_file >> numVertices >> numFaces >> ignore;
    //qDebug() << numVertices << numFaces;
    float x, y, z;
    for(int i = 0; i < numVertices; i++) {
        in_file >> x >> y >> z;
        vertexes.push_back(Vertex(x, y, z));
    }
    int numVPerFace;
    for(int i = 0; i < numFaces; i++) {
        in_file >> numVPerFace;
        int index;
        vector<int> vertices;
        for(int j = 0; j < numVPerFace; j++) {
            in_file >> index;
            vertices.push_back(index);
        }
        faces.push_back(vertices);
        vertices.clear();
    }
    in_file.close();
}

void ThreeDWidget::draw() {

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationZ, 0.0, 0.0, 1.0);
    int color;
    for(int i = 0; i < numFaces; ++i)
    {
        color = i % 10;
        switch (color) {
        case 0:
            glColor3f(0.5f, 0.5f, 0.5f); break;
        case 1:
            glColor3f(0.55f, 0.55f, 0.55f); break;
        case 2:
            glColor3f(0.6f, 0.6f, 0.6f); break;
        case 3:
            glColor3f(0.65f, 0.65f, 0.65f); break;
        case 4:
            glColor3f(0.7f, 0.7f, 0.7f); break;
        case 5:
            glColor3f(0.75f, 0.75f, 0.75f); break;
        case 6:
            glColor3f(0.8f, 0.8f, 0.8f); break;
        case 7:
            glColor3f(0.85f, 0.85f, 0.85f); break;
        case 8:
            glColor3f(0.9f, 0.9f, 0.9f); break;
        case 9:
            glColor3f(0.95f, 0.95f, 0.95f); break;
        }
        glLoadName(i);
        glBegin(GL_TRIANGLES);
        for(int j = 0; j < faces[i].size(); ++j)
        {
            int index = faces[i][j];
            glVertex3f(vertexes[index].x, vertexes[index].y, vertexes[index].z);
        }
        glEnd();
    }

}

void ThreeDWidget::mousePressEvent(QMouseEvent *event) {
    lastPos = event->pos();
}

void ThreeDWidget::mouseMoveEvent(QMouseEvent *event)
{
    GLfloat dx = GLfloat(event->x() - lastPos.x()) / width();
    GLfloat dy = GLfloat(event->y() - lastPos.y()) / height();
    if(event->buttons() & Qt::LeftButton){
        rotationX += 180 * dy;
        rotationY += 180 * dx;
        update();
    }
    else if(event->buttons() & Qt::RightButton){
        rotationX += 180 * dy;
        rotationZ += 180 * dx;
        update();
    }
    lastPos = event->pos();
}

