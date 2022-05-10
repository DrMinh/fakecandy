#include "graphics.h"
#include <QPainter>
#include <QSound>
#include <qdebug.h>
#include <QMouseEvent>
#include <QTime>
#include <QFile>
#include <QTextStream>
#include "g_anh.h"
#include "g_tilelist.h"
#include "khoikeo.h"
//graphics::listAnhc=listAnh;
graphics::graphics(QWidget *parent) : QWidget(parent)
{
    //Cài đặt ngẫu nhiên
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    //
    setFocusPolicy(Qt::StrongFocus); //Bắt sự kiện bàn phím bất kỳ khi nào tab được chọn
    //Nạp ảnh
    listAnh<<g_tilelist(100,100,":anh/UI.png")
          <<g_tilelist(1,1,":anh/nennau.png")
         <<g_tilelist(1,1,":anh/logo.png")
          <<g_tilelist(200,200,":anh/Sweets.png")
         <<g_tilelist(50,50,":anh/assets_candy.png")
         <<g_tilelist(50,50,":anh/background_blur.png")
           <<g_tilelist(50,50,":anh/bar.png")
          <<g_tilelist(140,140,":anh/explotion.png")
         <<g_tilelist(551,423,":anh/gameover.png")
             <<g_tilelist(168,75,":anh/UI2.png");
    //Tạo hoạt hình
    QList<int> hoatno;
    hoatno<<0<<7<<1<<1; //Hoạt hình từ frame 0->7,mỗi frame 1 khung hình, 1:có lặp lại
    listAnh[7].hoathinh<<hoatno;
    //testanh=g_anh(100,100,listAnh[0],10);
    //testanh.rx=testanh.ry=-40;
    //Các biến
    khoiChon=QPoint(-1,-1);
    DiemSo=g_ob(0,0);
    gameOverB=g_ob(0,0);
    //
    UI_menu=g_anh(0,0,listAnh[9]);
    UI_menu.rx=-UI_menu.wi/2;
    UI_menu.ry=-UI_menu.he/2;
    //Cài đặt thời gian
    int timerId=startTimer(30);

    qDebug()<<"ASdasd"<<timerId;
}

void graphics::paintEvent(QPaintEvent *){
    QPainter PT(this);
    //PT.setRenderHint(QPainter::Antialiasing);
    if(gameTT==1){
        //Tính chuột
        QPoint localM=getChuot();
        //
        if(gameTT_c){
            wi=this->width();
            he=this->height();
            listMenu.clear();
            listMenu<<g_anh(110,500,listAnh[3],0)
                    <<g_anh(130,200,listAnh[3],1)
                    <<g_anh(500,400,listAnh[3],2)
                    <<g_anh(400,130,listAnh[3],4)
                    <<g_anh(wi/2,he/2+100,listAnh[0],10);
            //
            listMenu[listMenu.count()-1].rx=listMenu[listMenu.count()-1].ry=-50;
            listMenu[0].rx=listMenu[0].ry=-100;
            listMenu[1].rx=listMenu[1].ry=-100;
            listMenu[2].rx=listMenu[2].ry=-100;
            listMenu[3].rx=listMenu[3].ry=-100;
            gameTT_c=false;
            //qDebug()<<"sada";
            loadGame();
        }
        //Vẽ Background
        PT.drawPixmap(0,0,wi,he,listAnh[1].Pix);
        //Vẽ listMenu
        for(int i=0,l=listMenu.count();i<l;i++){
            listMenu[i].Tick(PT);
            if(i==4&&chuotL&&listMenu[i].TestChuot(localM)){
                listMenu[i].changeFrame(3);
                DoiTT(2);
            }
            if(listMenu[i].TestChuot(localM)){
                listMenu[i].zoom=1.1;
            }
            else listMenu[i].zoom=1;
        }
        //Vẽ logo
        PT.drawPixmap(QPoint(90,30),listAnh[2].Pix);
    }
    else if(gameTT==2){
        QPoint localM=getChuot();
        if(gameTT_c){
            //Renew matrix
            listKhoi.clear();
            for(int i=0;i<64;i++){
                int kx=i%8;
                int ky=i/8;
                listKhoi<<taokeo(kx,ky);
            }
            //
//            int khoidb=randomNum(65,0);
//            listKhoi[khoidb]=taokeo(khoidb%8,khoidb/8,21);
            //
            DiemSo.x=DiemSo.y=0;
            khoiChon.setX(-1);
            khoiChon.setY(-1);
            choChon=0;
            gameTT_c=false;
            gameEnd=false;
            soluot=0;
            //Cài đặt chế độ
            if(gameMod==1) timeMod=1000;
            //Bảng game over
            gameOverB.x=wi/2+35-listAnh[8].Rcut.width()/2;
            gameOverB.y=-500;
        }
        //Lưu lại số điểm hiện tại
        if(DiemSo.y<DiemSo.x) DiemSo.y=(DiemSo.y+5>DiemSo.x)? DiemSo.x:DiemSo.y+5;
        //Vẽ Background
        PT.drawPixmap(0,0,wi,he,listAnh[5].Pix);
        //Vẽ bảng
        for(int k=0,l=listKhoi.count();k<l;k++){
            listAnh[4].draw(PT,26,chuanBang(listKhoi[k].mt.x),chuanBang(listKhoi[k].mt.y));
        }
        //Vẽ khoi
        //Tạo mảng chứa khối cần xóa <<Xoa[b] xóa cột b từ hàng Xoa[b].x đến hàng Xoa[b].y>>
        QVector<g_ob> Xoa;
        for(int b=0;b<8;b++)
            Xoa<<g_ob(-1,-1);
        bool choChon_c=true; //=false nó có khối nào còn hoạt động
        int epCheck=-1; //vị trí của một khối bắt buộc phải check ngay cả khi chưa stop
        QVector<int> khoiDB; //Khối đặt biệt cần xử lý thêm
        for(int k=0,l=listKhoi.count();k<l;k++){
            //listAnh[4].draw(PT,17,chuanBang(listKhoi[k].mt.x),chuanBang(listKhoi[k].mt.y));
            listKhoi[k].Tick(PT);

            //Trường hợp cần thay đổi id
            if(KhoiDoi!=-1){
                if(KhoiDoi==k){
                    if(listKhoi[k].keoID<6) listKhoi[k]=taokeo(k%8,k/8);
                    KhoiDoi--;
                    //qDebug()<<"renew"<<k;
                    //if(k==0) break;
                }
                choChon_c=false;
                //KhoiDoi--;
                continue;
            }
            //Kiểm tra
            if(choChon==2&&quetHang!=-1&&k%8==quetHang){
                int CTmove=find1move(k%8,k/8,listKhoi[k].keoID);
                //qDebug()<<"run";
                if(CTmove){
                    quetHang=-1;
                    KQquet=k;
                    //qDebug()<<k;
                }
            }
            bool stopK=listKhoi[k].isStop();
            if(listKhoi[k].keoID==17){
                if(choChon==2){
                    if(listKhoi[k].luotM==-1) listKhoi[k].luotM=soluot;
                    else if(listKhoi[k].luotM!=soluot){
                        listKhoi[k].luotM=soluot;
                        if(listKhoi[k].mau>0) listKhoi[k].mau--;
                        else listKhoi[k].diemCheck=true; //Chuẩn bị gọi
                    }
                }
                //Vẽ "phụ kiện" boom
                if(listKhoi[k].mau>0)
                    listAnh[4].draw(PT,28+listKhoi[k].mau,listKhoi[k].anhkeo.x+listKhoi[k].anhkeo.rx,listKhoi[k].anhkeo.y+listKhoi[k].anhkeo.ry,listKhoi[k].anhkeo.zoom);
                else if(listKhoi[k].anhkeo.nowFrame!=35)
                    listKhoi[k].anhkeo.changeFrame(35);
            }

            //Kiểm tra nổ
            int eff=-1;
            if(stopK&&listKhoi[k].diemCheck&&(choChon||listKhoi[k].diemVe!=-1||epCheck==k)){
                //Nếu ô chọn trước sẽ bị hủy
                //if(listKhoi[k].diemVe!=-1&&listKhoi[listKhoi[k].diemVe].diemVe==-1) listKhoi[k].diemVe=-1;
                //choChon_c=false;
                QList<int> KQ=findSame(k%8,k/8,eff);
//                if(listKhoi[k].keoID==21&&listKhoi[k].mau<=0){
//                    listKhoi[k].huy=true;
//                }
                //qDebug()<<k;
                listKhoi[k].diemCheck=false;
                if(KQ.count()==0){
                    eff=0;
                }else{
                    eff=1; //Tạm
                    choChon_c=false;
                    //qDebug()<<"co no"<<k;
                    //listKhoi[listKhoi[k].diemVe]
                    if(listKhoi[k].diemVe!=-1){
                        listKhoi[listKhoi[k].diemVe].diemVe=-1;
                        epCheck=listKhoi[k].diemVe; //Cho biết khối sau bắt buộc phải check
                        listKhoi[k].diemVe=-1;
                        sailuot=false;
                        //qDebug()<<k<<listKhoi[k].diemVe<<"top";
                    }
                    if(khoiChon.y()==k||khoiChon.x()==k){
                        khoiChon.setX(-1);
                        khoiChon.setY(-1);
                        sailuot=false;
                        //qDebug()<<k<<listKhoi[k].diemVe<<"down";
                    }
                    //
                    int diemcong=0;
                    int DiemNoc=DiemNo;
                    int timeModc=timeMod;
                    g_anh timeplus;
                    for(int khoiXoa:KQ){

                        int cot=khoiXoa%8;
                        int hang=khoiXoa/8;
                        if(!listKhoi[khoiXoa].huy){
                            listKhoi[khoiXoa].huy=true;
                            diemcong++;
                            DiemNo++;
                            if(listKhoi[khoiXoa].keoID==21){
                                timeMod+=100;
                                if(timeMod>2000) timeMod=2000;
                                timeplus=g_anh(listKhoi[khoiXoa].anhkeo.x+listKhoi[khoiXoa].anhkeo.rx,listKhoi[khoiXoa].anhkeo.y+listKhoi[khoiXoa].anhkeo.ry,listAnh[9],4);
                                timeplus.rx=-84;timeplus.ry=-37;
                                timeplus.zoom=1;
                                timeplus.tarzoom.setX(1.1);
                                timeplus.tarzoom.setY(0.005);
                                //listHieu<<Khen;
                            }
                        }
                        else{
                            //
                        }
                        //thêm hiệu ứng nổ
                        if(listKhoi[k].keoID<6){
                            listKhoi[khoiXoa].anhkeo.tarzoom.setX(0.1);
                            listKhoi[khoiXoa].anhkeo.tarzoom.setY(-0.1);
                            listKhoi[khoiXoa].anhkeo.zoom=1.2;
                            listHieu<<listKhoi[khoiXoa].anhkeo;
                        }
                        else{
                            listHieu<<taoNo(listKhoi[khoiXoa].anhkeo.x,listKhoi[khoiXoa].anhkeo.y);
                            if(timeMod>timeModc){
                                listHieu<<timeplus;
                            }
                        }

                        //listHieu<<taoNo(100,100);
                        //qDebug()<<Xoa[cot].x<<Xoa[cot].y<<hang<<cot;
                        if(Xoa[cot].x==-1||Xoa[cot].x>hang) Xoa[cot].x=hang;
                        if(Xoa[cot].y==-1||Xoa[cot].y<hang) Xoa[cot].y=hang;
                        //qDebug()<<Xoa[cot].x<<Xoa[cot].y<<hang<<cot<<"eko";
                        //setUpdatesEnabled(false);
                    }
                    //Cộng điểm
                    if(diemcong<=3) DiemSo.x+=diemcong*3;
                    else{
                        //
                        DiemSo.x+=diemcong*(3+1);
                        //

                    }
                    //Khen
                    if(DiemNo>DiemNoc){
                        //QSound::play("xoakhoi.wav");
                    }
                    if(DiemNoc<=15&&DiemNo>15){
                        g_anh Khen=g_anh(listKhoi[k].anhkeo.x+listKhoi[k].anhkeo.rx,listKhoi[k].anhkeo.y+listKhoi[k].anhkeo.ry,listAnh[9],2);
                        Khen.rx=-84;Khen.ry=-37;
                        Khen.zoom=0.1;
                        Khen.tarzoom.setX(2);
                        Khen.tarzoom.setY(0.1);
                        listHieu<<Khen;
                    }
                    else if(DiemNoc<=4&&DiemNo>4){
                        g_anh Khen=g_anh(listKhoi[k].anhkeo.x+listKhoi[k].anhkeo.rx,listKhoi[k].anhkeo.y+listKhoi[k].anhkeo.ry,listAnh[9],1);
                        Khen.rx=-84;Khen.ry=-37;
                        Khen.zoom=0.1;
                        Khen.tarzoom.setX(2);
                        Khen.tarzoom.setY(0.1);
                        listHieu<<Khen;
                    }
                }
            }
            //
            if(listKhoi[k].diemVe!=-1&&stopK){
                //qDebug()<<"xoay!";
                if(listKhoi[listKhoi[k].diemVe].diemVe!=-1){
                    //qDebug()<<listKhoi[k].diemVe<<"sai";
                    khoiChon.setX(k);
                    khoiChon.setY(listKhoi[k].diemVe);
                    stopK=false;
                    choChon_c=false;
                    sailuot=true;
                    //qDebug()<<k<<listKhoi[k].diemVe<<"sai";
                }
                //Nếu khối kia đã hủy về, hủy luôn việc về của khối đang xét
                else{
                    listKhoi[k].diemVe=-1;
                    sailuot=false;
                }
            }
            //
            if(!stopK){
                choChon_c=false;
            }
            if(choChon==2&&stopK&&listKhoi[k].anhkeo.TestChuot(localM)){
                listKhoi[k].anhkeo.zoom=1.1;
                if(chuotL){
                    int oneW=abs(khoiChon.x()%8-k%8);
                    int oneH=abs(khoiChon.x()/8-k/8);
                    if(khoiChon.x()!=-1&&((oneW==1&&oneH==0)||(oneH==1&&oneW==0))) khoiChon.setY(k);
                    else if(khoiChon.x()==k){
                        khoiChon.setX(-1);
                        khoiChon.setY(-1);
                    }
                    else khoiChon.setX(k);
                }
            }
            else{
                listKhoi[k].anhkeo.zoom=1;
            }
        }
        //Cập nhật hàng cần quét
        if(choChon==2&&quetHang!=-1&&quetHang<8) quetHang++;

        //Cập nhật cho phép người dùng chọn?
        if(choChon_c&&choChon<2){
            if(choChon==1){
                if(sailuot){
                    sailuot=false;
                    qDebug()<<"nuoc sai";
                }
                else soluot++;
                quetHang=0; //cho biết cần quét lại
                GoiY=100;
                DiemNo=0;
            }
            choChon++;
        }
        else if(!choChon_c){
            choChon=0;
        }
        //qDebug()<<"end.";
        //Xác định out move
        if(choChon==2&&quetHang==8&&KhoiDoi==-1){
            KQquet=-1;
            KhoiDoi=8*8-1;
            quetHang=-1;
            qDebug()<<"out move";
        }

        //Duyệt xóa và sinh các khối mới
        for(int cot=0;cot<8;cot++){
        //            qDebug()<<"wtf";
            if(Xoa[cot].x==-1) continue;
            //
            int hangTrong=Xoa[cot].y;
            for(int hang=Xoa[cot].y;hang>=-1;hang--){
                //qDebug()<<hang;
                int nia=getIDvec(cot,hang);
                //Nếu đã dời xong, tạo thêm các khối mới để lấp phần còn lại
                if(hang==-1&&hangTrong>=0){
                    for(int lap=0;lap<=hangTrong;lap++){
                        int them=getIDvec(cot,lap);
                        listKhoi[them]=taokeo(cot,lap);
                        listKhoi[them].hoandi.y=5;
                        //hangTrong--;
                    }
                    break;
                }
                //Nếu đang duyệt khối cần xóa,bỏ qua
                if(listKhoi[nia].huy==true){
                    if(cot>0){
                        listKhoi[getIDvec(cot-1,hang)].nhanSat();
                    }
                    if(cot+1<8){
                        listKhoi[getIDvec(cot+1,hang)].nhanSat();
                    }
                    if(hang>0){
                        listKhoi[getIDvec(cot,hang-1)].nhanSat();
                    }
                    if(hang+1<8){
                        listKhoi[getIDvec(cot,hang+1)].nhanSat();
                    }
                    continue;
                }
                //Khối đang xét chưa bị hủy, dời nó đến vị trí trống dưới nhất
                int oLap=getIDvec(cot,hangTrong);
                listKhoi[nia].changeMat(cot,hangTrong);
                listKhoi[oLap]=listKhoi.at(nia);
                listKhoi[oLap].diemCheck=true;
                listKhoi[oLap].hoandi.y=5;
                hangTrong--;
            }
        }
//        qDebug()<<"end";

        //Vẽ các khung chọn
        if(khoiChon.x()!=-1){
            int chonX=khoiChon.x()%8;
            int chonY=khoiChon.x()/8;
            //int mau=randomNum(3,0)+22;
            //if(mau==2) qDebug()<<mau;
            listAnh[4].draw(PT,27,chuanBang(chonX),chuanBang(chonY));
            //
            if(khoiChon.y()!=-1){
                //qDebug()<<khoiChon.x()<<khoiChon.y();
                swapKeo(khoiChon.x(),khoiChon.y());
                //qDebug()<<khoiChon.x()<<khoiChon.y()<<"doine";
                //
                khoiChon.setX(-1);
                khoiChon.setY(-1);
            }
        }
        //Vẽ khung gợi ý
        if(KQquet!=-1&&choChon==2&&GoiY>0){
            //qDebug()<<"ve";
            int numG=(GoiY%20>10)? 1:0;
            listAnh[4].draw(PT,33-numG,chuanBang(KQquet%8),chuanBang(KQquet/8));
            GoiY--;
        }
        //if(listKhoi.count()!=64) qDebug()<<listKhoi.count();
        //Vẽ score bar
        PT.drawPixmap(0,-30,wi,100,listAnh[6].Pix);
        //Vẽ hiệu ứng
        for(int Hi=0,lHi=listHieu.count();Hi<lHi;Hi++){
//            listHieu[Hi].x++;
            bool Azoom=(listHieu[Hi].tarzoom.x()!=-1);
            listHieu[Hi].Tick(PT);
            bool huyhieu1=listHieu[Hi].idhoat!=-1&&listHieu[Hi].nowFrame==listHieu[Hi].Tileset.hoathinh[listHieu[Hi].idhoat][1]&&listHieu[Hi].frameCon==0;
            bool huyhieu2=(Azoom&&listHieu[Hi].tarzoom.x()==-1);
            if(huyhieu1||huyhieu2){
                listHieu.removeAt(Hi);
                lHi--;
                Hi--;
            }
            //listHieu[Hi].Tileset.draw(PT,0,100,100);
        }
        //qDebug()<<listHieu.count();
        //Vẽ Bảng điểm
        QFont font = PT.font();
        font.setPixelSize(20);
        font.setFamily("Helvetica [Cronyx]");
        PT.setFont(font);
        PT.setPen(Qt::white);
        PT.drawText(QRect(10,10,60,50),"Score:");
        //font=PT.font();
        font.setBold(true);
        int DiemLech=0;
        if(DiemSo.x==DiemSo.y) font.setPixelSize(25);
        else{
            DiemLech=3;
            font.setPixelSize(30);
        }
        PT.setFont(font);
        PT.drawText(QRect(90,10-DiemLech,100,50),QString::number(DiemSo.y));
        //Vẽ các đối tượng chế độ
        if(gameMod==1&&!gameEnd){
            //Thêm thời gian
            //if(DiemSo.x-DiemSo.y>9) timeMod+=(DiemSo.x-DiemSo.y)/3;
            //
            if(choChon==2){
                timeMod--;

            }
            if(timeMod<0){
                timeMod=0;
                gameEnd=true;
            }
            font.setPixelSize(50);
            PT.setFont(font);
            PT.drawText(QRect(wi/2-60,-20,100,100),Qt::AlignCenter,QString::number(timeMod*30/1000));
        }
        //Kết thúc trò chơi?
        if(gameEnd){
            choChon=0;
            if(gameOverB.y==-500) DiemSo.y=0;
            PT.drawPixmap(QPoint(gameOverB.x,gameOverB.y),listAnh[8].Pix);
            //Vẽ UI menu
            UI_menu.x=gameOverB.x+155-UI_menu.rx;
            UI_menu.y=gameOverB.y+300-UI_menu.ry;
            UI_menu.draw(PT);
            if(gameOverB.y>he/2+20-listAnh[8].Rcut.height()/2&&UI_menu.TestChuot(localM)){
                UI_menu.zoom=1.1;
                if(chuotL){
                    //qDebug()<<DiemSo.x<<DiemCao<<"??";
                    if(DiemSo.x>DiemCao){
                        luuGame(DiemSo.x);
                        //qDebug()<<DiemCao<<DiemSo.x;
                    };
                    DoiTT(1);
                }
            }
            else
                UI_menu.zoom=1;
            //
            if(gameOverB.y<he/2+20-listAnh[8].Rcut.height()/2) gameOverB.y+=30;
            else{
                font.setPixelSize(35);
                PT.setFont(font);
                PT.setPen(QColor("#543518"));
                //Vẽ điểm đạt được
                PT.drawText(QRect(gameOverB.x+220,gameOverB.y+170,100,50),QString::number(DiemSo.y));
                //Vẽ điểm cao
                PT.drawText(QRect(gameOverB.x+220,gameOverB.y+250,100,50),QString::number((DiemCao>DiemSo.y)? DiemCao:DiemSo.y));
                //
                if(DiemSo.y+10<DiemSo.x) DiemSo.y+=10;
                else DiemSo.y=DiemSo.x;
//                if(DiemCao+10<DiemSo.y) DiemCao+=10;
//                else if(DiemCao<DiemSo.y) DiemCao=DiemSo.y;
            }
        }
    }
}

void graphics::swapKeo(int x, int y){
    g_ob Sao=g_ob(listKhoi[x].mt.x,listKhoi[x].mt.y);
    listKhoi[x].changeMat(listKhoi[y].mt.x,listKhoi[y].mt.y);
    listKhoi[y].changeMat(Sao.x,Sao.y);
    //qDebug()<<listKhoi[x].mt.x<<listKhoi[y].mt.x;
    khoikeo K=listKhoi.at(x);
    listKhoi[x]=listKhoi.at(y);
    listKhoi[y]=K;
    listKhoi[x].diemCheck=true;
    listKhoi[y].diemCheck=true;
    if(listKhoi[x].diemVe!=-1||listKhoi[y].diemVe!=-1){
        listKhoi[x].diemVe=-1;
        listKhoi[y].diemVe=-1;
    }
    else{
        listKhoi[x].diemVe=y;
        listKhoi[y].diemVe=x;
        //qDebug()<<"swap";
    }
}
int graphics::find1move(int x, int y,int id){
    int eff=0;
    if(y-1>=0) findSame(x,y-1,eff,id,2);
    if(!eff&&y+1<8) findSame(x,y+1,eff,id,0);
    if(!eff&&x-1>=0) findSame(x-1,y,eff,id,1);
    if(!eff&&x+1<8) findSame(x+1,y,eff,id,3);
    //
    return eff;
}

//x,y: vị trí khối trong mat, eff:các hiệu ứng đặt biệt, fakeID: giả sử khối có id khác, wside: không dò theo hướng 0,1,2,3 top,right,bot,left
QList<int> graphics::findSame(int x,int y, int &eff,int fakeID,int wside){
    int arrnum=graphics::getIDvec(x,y);
    int idkeo=(fakeID==-1)? listKhoi[arrnum].keoID:fakeID;
    //Tìm ngang
    int left=x-1;
    int right=x+1;
    int top=y-1;
    int bot=y+1;
    int slNgang=0;
    int slDoc=0;
    QList<int> ketquaX;
    //QVector<int> ketquaY;
    //Các khối đặt biệt
    if(idkeo==21){ //Bánh quy
        if(listKhoi[arrnum].mau<=0){
            ketquaX<<arrnum;
            eff=1;
        }
        return ketquaX;
    }
    else if(idkeo==17){ //Boom
        if(listKhoi[arrnum].mau<=0){
            g_ob gx=g_ob(qMax(arrnum%8-1,0),qMin(arrnum%8+1,8-1));
            g_ob gy=g_ob(qMax(arrnum/8-1,0),qMin(arrnum/8+1,8-1));
            //
           int gW=gx.x;
           int gH=gy.x;
            while(gH<=gy.y){
                ketquaX<<getIDvec(gW,gH);
                gW++;
                if(gW>gx.y){
                    gW=gx.x;
                    gH++;
                }
            }
            //
            eff=1;
        }
        return ketquaX;
    }
    //duyệt ngang
    if(wside!=3)
    while(left>=0){
        //duyệt trái
        int idL=getIDvec(left,y);
        if(listKhoi[idL].keoID==idkeo&&listKhoi[idL].khop){
            ketquaX<<idL;
            left--;
        }
        else left=-1; //Hủy tìm left
    }
    if(wside!=1)
    while(right<8){
        //duyệt phải
        int idR=getIDvec(right,y);
        if(listKhoi[idR].keoID==idkeo&&listKhoi[idR].khop){
            ketquaX<<idR;
            right++;
        }
        else right=100; //Hủy tìm right
    }
    slNgang=ketquaX.count();
    if(slNgang<2){
        ketquaX.clear();
        slNgang=0;
    }
    //Duyệt dọc
    if(wside!=0)
    while(top>=0){
        //duyệt top
        int idT=getIDvec(x,top);
        if(listKhoi[idT].keoID==idkeo&&listKhoi[idT].khop){
            ketquaX<<idT;
            top--;
        }
        else top=-1; //Hủy tìm top
    }
    if(wside!=2)
    while(bot<8){
        //duyệt bot
        int idB=getIDvec(x,bot);
        if(listKhoi[idB].keoID==idkeo&&listKhoi[idB].khop){
            ketquaX<<idB;
            bot++;
        }
        else bot=100; //Hủy tìm right
    }
    slDoc=ketquaX.count();
    if(slDoc-slNgang<2){
        //ketquaX.resize(slNgang);
        xoaMang(ketquaX,slNgang);
        //xoaMang(ketquaX,slNgang);
        slDoc=0;
    }
    //
    if(slDoc<2&&slNgang<2) ketquaX.clear();
    else{
        ketquaX<<arrnum;
        eff=1;
    }
    //Kết thúc
    return ketquaX;
}
khoikeo graphics::taokeo(int x, int y,int db){
    int khoiDB=randomNum(101,0);
    if(khoiDB>90) db=21;
    else if(khoiDB>89) db=17;
    return khoikeo(x,y,listAnh[4],(db==-1)? randomNum(6,0):db);
}
g_anh graphics::taoNo(int x,int y){
    g_anh A=g_anh(x,y,listAnh[7]);
    A.rx=A.ry=-70;
    //qDebug()<<A.x<<"eeee"<<chuanBang(x)+70;
    listAnh[7].dangkyHoat(A,0);
    return A;
}

void graphics::DoiTT(int t){
    gameTT=t;
    gameTT_c=true;
}

void graphics::timerEvent(QTimerEvent *){
//    QPoint localM=mapFromGlobal(QCursor::pos());
//    if(QCursor::f)qDebug()<<localM.y();
    repaint();
    chuotL=false;
}

void graphics::mousePressEvent(QMouseEvent *ev){
    if(ev->button()==Qt::LeftButton){
        chuotL=true;
    }
}

QPoint graphics::getChuot(){
    return mapFromGlobal(QCursor::pos());
}
//Random từ t>f
int graphics::randomNum(int f, int t){
    return qrand()%((t+1)-f)+t;
//    return qrand()%f+t;
}
int graphics::chuanBang(int num){
    return num*50+100;
}
int graphics::getIDvec(int x, int y){
    return y*8+x;
}
void graphics::luuGame(int DiemSo, int mod, QString ten){
    //
    QFile file("TaoNe.txt");
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << DiemSo << endl;
    }
    file.close();
}
void graphics::loadGame(){
    QFile file("TaoNe.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&file);
    QString line = in.readLine();
    DiemCao=line.toInt();
    file.close();
    //qDebug()<<DiemCao;
}

void graphics::xoaMang(QList<int> &L, int A){
    for(int i=L.count()-1;i>=A;i--){
        L.takeLast();
    }
}
