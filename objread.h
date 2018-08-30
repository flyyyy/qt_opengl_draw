#ifndef OBJREAD_H
#define OBJREAD_H

#include <vector>
#include <cstring>
#include <string>
#include <iostream>
#include <qfile.h>
#include <QFile>
#include <qdebug.h>
#include <QDebug>

class point3f
{

public:
    double x, y, z;
    point3f(double xx = 0, double yy = 0, double zz = 0) : x(xx), y(yy), z(zz){}

    point3f operator-(point3f b) {
        return point3f(x - b.x, y - b.y, z - b.z);
    }

    point3f cross(point3f b) {
        return point3f(y * b.z - z * b.y,
                       z * b.x - x * b.z,
                       x * b.y - y * b.x);
    }

    void normalize() {
        double len = sqrt(x * x + y * y + z * z);
        x = x / len;
        y = y / len;
        z = z / len;
    }
};

class point3d
{
public:
    int x, y, z;
    point3d(int xx = 0, int yy = 0, int zz = 0) : x(xx), y(yy), z(zz){}
};

class objread
{
public:
    std::vector<point3f*> pointlist;
    std::vector<point3d*> facelist;
    objread();
    void init();
    void read(std::string filename);
    point3f getTrans();
};


#endif // OBJREAD_H
