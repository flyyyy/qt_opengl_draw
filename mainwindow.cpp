#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "objread.h"

MainWindow::MainWindow(QWidget *parent) :
    QGLWidget(parent),
    ui(new Ui::MainWindow)
{
    rotationX = 0.0;
    rotationY = 0.0;
    rotationZ = 0.0;
    dis = 3.0;
//    ui->setupUi(this);
}

MainWindow::~MainWindow()
{

}

void MainWindow::initializeGL()
{
    glClearColor(0, 0, 0, 1);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);

    GLfloat LightAmbient[] = {0.5f, 0.5f, 0.5f, 1.0f};  //环境光参数
    GLfloat LightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};  //漫散光参数
    GLfloat LightPosition[] = {0.0f, -1.0f, 0.0f, 0.0f}; //光源位置
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);     //设置环境光
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);     //设置漫射光
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);   //设置光源位置
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);

    myread = new objread();
    myread->read("ant.obj");
    trans = myread->getTrans();
}

void MainWindow::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat x = GLfloat(width) / height;
    gluPerspective(10, x, 1.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
}

void MainWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw();
}

void MainWindow::draw()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-trans.x, -trans.y, -trans.z - dis);
    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationZ, 0.0, 0.0, 1.0);
    for(int i = 0; i < myread->pointlist.size(); i++) {
//        glLoadName(i);
        glBegin(GL_POINTS);
        glColor3f(1, 0, 0);
        glVertex3f(myread->pointlist[i]->x, myread->pointlist[i]->y, myread->pointlist[i]->z);
        glEnd();
    }
    for(int i = 0; i < myread->facelist.size(); i++) {
//        glLoadName(i);
        glBegin(GL_TRIANGLES);
//      glColor3f(62.0/255, 62.0/255, 62.0/255);
        int idx = myread->facelist[i]->x - 1;
        int idy = myread->facelist[i]->y - 1;
        int idz = myread->facelist[i]->z - 1;
        point3f faceN = workfaceN(*(myread->pointlist[idx]), *(myread->pointlist[idy]), *(myread->pointlist[idz]));
//        qDebug() << faceN.x << " " << faceN.y << " " << faceN.z;
        glNormal3f(-faceN.x, -faceN.y, -faceN.z);
        glVertex3f(myread->pointlist[idx]->x, myread->pointlist[idx]->y, myread->pointlist[idx]->z);
        glVertex3f(myread->pointlist[idy]->x, myread->pointlist[idy]->y, myread->pointlist[idy]->z);
        glVertex3f(myread->pointlist[idz]->x, myread->pointlist[idz]->y, myread->pointlist[idz]->z);
        glEnd();
    }
    this->update();
}

point3f MainWindow::workfaceN(point3f a, point3f b, point3f c)
{
    point3f v1 = a - b;
    point3f v2 = a - c;
    v1 = v1.cross(v2);
    v1.normalize();
    return v1;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    qDebug() << "press..." << endl;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    GLfloat dx = GLfloat(event->x() - lastPos.x()) / width();
    GLfloat dy = GLfloat(event->y() - lastPos.y()) / height();
    if(event->buttons() & Qt::LeftButton){
        rotationX += 180 * dy;
        rotationY += 180 * dx;
        paintGL();
    }
    else if(event->buttons() & Qt::RightButton){
        trans.y += dy;
        trans.x += -dx;
        paintGL();
    }
    lastPos = event->pos();
}

void MainWindow::wheelEvent(QWheelEvent * event)
{
    if(event->delta() > 0)
        dis = dis * 0.8;
    else
        dis = dis * 1.2;
    dis = std::max(dis, GLfloat(1.0));
    qDebug() << "dis == " << dis;
    paintGL();
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    int face = faceAtPosition(event->pos());
    if(face != -1){
        QColor color = QColorDialog::getColor(faceColors[face], this);
        if(color.isValid()){
            faceColors[face] = color;
            paintGL();
        }
    }
}

int MainWindow::faceAtPosition(const QPoint &pos)
{
    const int MaxSize = 512;
    GLuint buffer[MaxSize];
    GLint viewport[4];
//    makeCurrent();
    glGetIntegerv(GL_VIEWPORT, viewport);
    glSelectBuffer(MaxSize, buffer);
    glRenderMode(GL_SELECT);

    glInitNames();
    glPushName(0);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    gluPickMatrix(GLdouble(pos.x()), GLdouble(viewport[3] - pos.y()), 5.0, 5.0, viewport);
    GLfloat x = GLfloat(width()) / height();
    glFrustum(-x, x, -1.0, 1.0, 4.0, 15.0);
    draw();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    if(!glRenderMode(GL_RENDER))
        return -1;
    return buffer[3];
}
