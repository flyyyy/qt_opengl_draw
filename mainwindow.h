#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtOpenGL>
#include <QMouseEvent>
#include <QColorDialog>
#include <GL/glu.h>
#include <GL/gl.h>
#include <qgl.h>
#include <objread.h>
#include <cmath>


namespace Ui {
class MainWindow;
}

class MainWindow : public QGLWidget
{
    Q_OBJECT

protected:

    void initializeGL();

    void resizeGL(int width, int height);

    void paintGL();

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseDoubleClickEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent *event);

    point3f workfaceN(point3f a, point3f b, point3f c);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void draw();
    int faceAtPosition(const QPoint &pos);
    objread* myread = NULL;
    GLfloat rotationX;
    GLfloat rotationY;
    GLfloat rotationZ;
    point3f trans;
    GLfloat dis;
    QColor faceColors[4];
    QPoint lastPos;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
