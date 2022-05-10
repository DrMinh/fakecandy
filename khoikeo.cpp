#include "khoikeo.h"
#include "graphics.h"
#include <QDebug>
khoikeo::khoikeo()
{

}

khoikeo::khoikeo(int x, int y, g_tilelist T, int num){
    anhkeo=g_anh(graphics::chuanBang(x)+25,graphics::chuanBang(y)+25-375-(8-y)*15,T,num);
    anhkeo.rx=anhkeo.ry=-25;
    mt=g_ob(x,y);
    tangtoc=g_ob(0,1);
    hoandi=g_ob(0,0);
    diemVe=-1;
    keoID=num;
    if(num>5) khop=false;
    if(num==21) mau=2;
    else if(num==17) mau=3;
}

void khoikeo::Tick(QPainter &PT){
    int chuanY=graphics::chuanBang(mt.y)+25;
    int chuanX=graphics::chuanBang(mt.x)+25;
    int oy=anhkeo.y;
    //hoãn di chuyển
    if(hoandi.x>0){
        hoandi.x--;
        tangtoc.x=0;
    }
    if(hoandi.y>0){
        hoandi.y--;
        tangtoc.y=0;
    }
    //
    if(anhkeo.y!=chuanY){
        if(abs(chuanY-anhkeo.y)>abs(tangtoc.y)){
            anhkeo.y+=tangtoc.y;
            tangtoc.y+=(anhkeo.y<chuanY)? 1:-1;
//            if(tangtoc.y>10) tangtoc.y=10;
        }
        else{
            anhkeo.y=chuanY;
            tangtoc.y=0;
        }
    }
    if(anhkeo.x!=chuanX){
        if(abs(chuanX-anhkeo.x)>abs(tangtoc.x)){
            anhkeo.x+=tangtoc.x;
            tangtoc.x+=(anhkeo.x<chuanX)? 1:-1;
//            if(tangtoc.y>10) tangtoc.y=10;
        }
        else{
            anhkeo.x=chuanX;
            tangtoc.x=0;
        }
    }
    //Chậm lại khi vào ma trận
//    if(oy<graphics::chuanBang(0)+anhkeo.ry&&anhkeo.y>=graphics::chuanBang(0)+anhkeo.ry)
//        tangtoc.y=1;
    //
    if(anhkeo.y+anhkeo.ry<graphics::chuanBang(0)){
        int xety=graphics::chuanBang(0)-anhkeo.y-anhkeo.ry; //Khoảng cách của bảng và y >0
        if(xety>=anhkeo.Tileset.Rcut.height()) return; //Nếu cách quá xa thì thôi
        //int cutH=anhkeo.Tileset.Rcut.height();
        //Lưu vị trí cũ
        //int tH=anhkeo.Tileset.Rcut.y();
        int ty=anhkeo.y;
        //thay đổi vị trí gốc và độ lớn của ảnh
        anhkeo.Tileset.giamY=xety;
        anhkeo.y=graphics::chuanBang(0)-anhkeo.ry;
        //Vẽ ảnh đã bị cắt
        anhkeo.draw(PT);
        //phục hồi vị trí cũ
        anhkeo.Tileset.Rcut.setHeight(anhkeo.he);
        anhkeo.y=ty;
        //if(anhkeo.Tileset.Rcut.height()==-50) qDebug()<<graphics::randomNum(100,0)<<mt.y<<cutH<<tH;
//        qDebug()<<mt.y;
        return;
    }
//    anhkeo.Tileset.Rcut.setHeight(20);
    anhkeo.draw(PT);
}

bool khoikeo::isStop(){
    return tangtoc.x==0&&tangtoc.y==0&&anhkeo.x==graphics::chuanBang(mt.x)+25&&anhkeo.y==graphics::chuanBang(mt.y)+25;
}

void khoikeo::changeMat(int cx, int cy){
    mt.x=cx;
    mt.y=cy;
}

bool khoikeo::nhanSat(){
    if(keoID<6) return false;
    if(keoID==21){
        mau--;
        diemCheck=true;
        if(mau>=1) anhkeo.changeFrame(20);
        else anhkeo.changeFrame(34);
        return true;
    }
    return false;
}
