#ifndef G_TILELIST_H
#define G_TILELIST_H
#include <QPixmap>
#include <QImage>
#include <QRect>
#include "g_ob.h"
/*#include "g_anh.h"*/
class g_anh;
class g_tilelist
{
public:
    g_tilelist();
    g_tilelist(int cw, int ch, QString cpath);
    int numW;
    int numH;
    int giamY=0;
    int giamX=0;
    QPixmap Pix;
    QImage Img;
    QRect Rcut;
    QList<QList<int>> hoathinh;
    void dangkyHoat(g_anh &Anh, int idHoat);
    void capnhatHoat(int &nowFrame, int &frameCon,int idHoat);
    void draw(QPainter &PT, int num, int cx, int cy, float zoom=1,float rotate=0);
    void catAnh(int y);
};

#endif // G_TILELIST_H
