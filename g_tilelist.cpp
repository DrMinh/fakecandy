#include "g_tilelist.h"
#include <QPainter>
#include <QDebug>
#include "graphics.h"
g_tilelist::g_tilelist()
{

}
g_tilelist::g_tilelist(int cw, int ch, QString cpath){
    Pix=QPixmap(cpath);
    Img=QImage(cpath);
    numW=Img.width()/cw;
    numH=Img.height()/ch-1;
    //qDebug()<<Img.hasAlphaChannel();
    Rcut=QRect(0,0,cw,ch);
}
void g_tilelist::draw(QPainter &PT, int num,int cx,int cy,float zoom,float rotate){
    //
//    if(Rcut.height()==-50) qDebug()<<"cc";
    int A=(num%numW)*Rcut.width();
    int B=(num/numW)*Rcut.height();
    Rcut.moveTo(A,B);
    //giamY=32;
    //Giảm kích thước (vẽ 1 phần ảnh)
    Rcut.setY(Rcut.y()+giamY);
    //Nếu có xoay
    if(rotate!=0){

        PT.translate(QPointF(cx+(Rcut.width()/2)*zoom,cy+(Rcut.height()/2)*zoom));
        PT.rotate(0);
        cx=-(Rcut.width()/2)*zoom;cy=-(Rcut.height()/2)*zoom;
    }
//    Rcut.setHeight(20);
    //
    //qDebug()<<Rcut.x()<<Rcut.y();
//    if(Rcut.height()>=200||Rcut.height()<=0) qDebug()<<Rcut.height()<<graphics::randomNum(100,0)<<giamY;
    if(zoom==1) PT.drawPixmap(QPoint(cx,cy),Pix,Rcut);
    else PT.drawPixmap(QRect(cx,cy,Rcut.width()*zoom,Rcut.height()*zoom),Pix,Rcut);

//    if(Rcut.height()==-50) qDebug()<<graphics::randomNum(99,0)<<Rcut.y()<<Rcut.width();
    giamX=giamY=0;
    if(rotate!=0) PT.resetTransform();
//    qDebug()<<Rcut.height();
}
void g_tilelist::catAnh(int y){
    Rcut.setHeight(y);
}
void g_tilelist::capnhatHoat(int &nowFrame, int &frameCon,int idHoat){
    //nowFrame=0; return;
    if(frameCon>0){
        frameCon--;
        return;
    }
    frameCon=hoathinh[idHoat][2];
    if(nowFrame==hoathinh[idHoat][1]) nowFrame=hoathinh[idHoat][0];
    else nowFrame++;
}

void g_tilelist::dangkyHoat(g_anh &Anh, int idHoat){
    Anh.nowFrame=hoathinh[idHoat][0];
    Anh.frameCon=hoathinh[idHoat][2];
    Anh.idhoat=idHoat;
    //qDebug()<<"ASDAS";
}
