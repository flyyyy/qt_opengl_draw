#include <objread.h>

objread::objread()
{
    pointlist.clear();
    facelist.clear();
}

void objread::init()
{
    pointlist.clear();
    facelist.clear();
}

void objread::read(std::string filename)
{
    QFile objfile("chair.obj");
    if(!objfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "open fail...";
        return ;
    }
    qDebug() << "open success...";

    QTextStream objInput(&objfile);
    while(!objInput.atEnd()) {
        QString line = objInput.readLine();
//        qDebug() << line;
        QStringList linelist = line.split(" ", QString::SkipEmptyParts);
        if(linelist.size() == 0) continue;
        if(linelist[0] == 'v') {
            double x = linelist.at(1).toDouble();
            double y = linelist.at(2).toDouble();
            double z = linelist.at(3).toDouble();
            point3f* tpoint3f = new point3f(x, y, z);
            pointlist.push_back(tpoint3f);
//            qDebug() << x;
        }
        else if(linelist[0] == 'f') {
            int x = linelist.at(1).toInt();
            int y = linelist.at(2).toInt();
            int z = linelist.at(3).toInt();
            point3d* tpoint3d = new point3d(x, y, z);
            facelist.push_back(tpoint3d);
        }
        else continue;
    }
}

point3f objread::getTrans()
{
    double lx, ly, lz, rx, ry, rz;
    lx = ly = lz = 1e7;
    rx = ry = rz = -1e7;
    for(int i = 0; i < pointlist.size(); i++) {
        lx = std::min(lx, pointlist[i]->x);
        ly = std::min(ly, pointlist[i]->y);
        lz = std::min(lz, pointlist[i]->z);
        rx = std::max(rx, pointlist[i]->x);
        ry = std::max(ry, pointlist[i]->x);
        rz = std::max(rz, pointlist[i]->x);
    }
    return point3f((lx + rx) / 2, (ly + ry) / 2, (lz + rz) / 2);
}
