#include "g_anh.h"
#include "g_ob.h"
#include "graphics.h"
#include "g_tilelist.h"
#include "QPoint"
#include <QCursor>
#include <QDebug>
#include <QImage>
#include <QRgb>
g_anh::g_anh()
{

}

g_anh::g_anh(int cx, int cy, g_tilelist Tile,int cFrame) :g_ob(cx,cy)
{
    nowFrame=cFrame;
    Tileset=Tile;
    wi=Tile.Rcut.width();
    he=Tile.Rcut.height();
    tarzoom=QPointF(-1,-1);
    //qDebug()<<he;
}
void g_anh::draw(QPainter &PT){
    //rx=ry=-50;
    if(zoom==1) Tileset.draw(PT,nowFrame,x+rx,y+ry);
    else Tileset.draw(PT,nowFrame,x+rx*zoom,y+ry*zoom,zoom);
    if(idhoat!=-1){
        Tileset.capnhatHoat(nowFrame,frameCon,idhoat);
        //qDebug()<<x+rx<<x<<rx;
    }
}
int g_anh::changeFrame(int Frame){
    nowFrame=Frame;
    return Frame;
}
int g_anh::Tick(QPainter &PT){
    //Vẽ ảnh
    draw(PT);
    //Xử lý zoom
    if(tarzoom.x()!=-1){
        //qDebug()<<qAbs(tarzoom.x()-zoom)<<abs(tarzoom.y());
        if(qAbs(tarzoom.x()-zoom)>qAbs(tarzoom.y())){
            zoom+=tarzoom.y();
            //qDebug()<<zoom;
        }
        else{
            zoom=tarzoom.x();
            tarzoom.setX(-1);
        }
    }
    //
    return 1;
}
int g_anh::TestChuot(QPoint Cur){
    //Nếu không có sự kiện
    if(!Hasclick) return 0;
    //
    bool hitR=QRect(x+rx,y+ry,wi,he).contains(Cur.x(),Cur.y());
    //nếu click ngoài vùng hoặc không cần test pixel
    if(!hitR||!Hasclick_Pix) return hitR;
    //Tìm alpha của pixel đang click
    int lx=Cur.x()-rx-x+Tileset.Rcut.x();
    int ly=Cur.y()-ry-y+Tileset.Rcut.y();
    //qDebug()<<qAlpha(Tileset.Img.pixel(lx,ly))<<lx<<ly;
    if(qAlpha(Tileset.Img.pixel(lx,ly))==0) return 0;
    return 1;
}
//void g_anh::catAnh(int y){
//    Tileset.catAnh(y);
//    qDebug()<<Tileset.Rcut.height()<<xety;
//}

//void g_anh::Click(QPainter &PT){
//    //int A=graphics::randomNum(0,9);
//    //changeFrame(A);
//}

