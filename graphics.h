#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QWidget>
#include <QVector>
#include <QTextStream>
#include "g_tilelist.h"
#include "g_anh.h"
#include "khoikeo.h"
class graphics : public QWidget
{
    Q_OBJECT
public:
    explicit graphics(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);
    void mousePressEvent(QMouseEvent *ev);
    QPoint getChuot();
    static int randomNum(int f,int t);
    static int chuanBang(int num);
    static int getIDvec(int x,int y);
    int wi=0;//Dài rong cua man hinh
    int he=0;
    /**** Các biến game *****/
    QList<g_tilelist> listAnh;
    g_anh testanh;
    int gameTT=1; //0:Vừa vào, 1: menu screen, 2: Choi
    bool gameTT_c=true; //Vừa thay đổi trạng thái?
    int DiemCao=0;
    void DoiTT(int t); //Chuyển sang trạng thái khác
    /* Màn hình menu */
    QList<g_anh> listMenu;
    QList<g_anh> listBanh;
    /* Màn hình chơi */
    QList<khoikeo> listKhoi; //Danh sách các khối
    QList<g_anh> listHieu; //Danh sách hiệu ứng
    QPoint khoiChon; //x: khối chọn đầu, y khối chọn sau , -1 nếu chưa chọn.
    g_ob gameOverB; //Vị trí bảng gameover
    g_anh UI_menu; //Nút quay về menu
    int gameMod=1; //1: thời gian
    int gameEnd=false; //Cho biết trò chơi đã kết thúc chưa?
    int choChon; //==2 cho người dùng tương tác
    int quetHang=0; //Hàng hiện đang quét tìm khối có thẻ swap để lấy điểm -1:đã tìm được.
    int KQquet=0; //Số thứ tự mảng của 1 khối có thể swap được. -1:khống có kết quả
    int GoiY=0; //Thời gian hiện gợi ý
    int KhoiDoi=-1; //Khối cần thay đổi để mảng swap được
    int soluot=0; //Số lượt đã di chuyển
    int sailuot=false; //nước đi sai? (không tính lượt vừa qua)
    g_ob DiemSo; //x: điểm hiện tại, y: quá khứ
    int DiemNo=0; //số khối nổ 1 lượt di chuyển gây ra
    //Chế đọ thời gian
    int timeMod=0;
    //tìm các khối giống nhau ở cạnh khối x,y
    QList<int> findSame(int x, int y, int &eff, int fakeID=-1, int wside=-1);
    //Cho biết khối (x,y) có swap được không.
    int find1move(int x, int y, int id);

    khoikeo taokeo(int x,int y,int db=-1);
    g_anh taoNo(int x,int y);
    void swapKeo(int x,int y);
    void luuGame(int DiemSo,int mod=1,QString ten="minh");
    void loadGame();
    void xoaMang(QList<int> &L, int A);
    //Thời gian
    //int timerId;
    //int bien;
    //Sự kiện người dùng.
    int chuotL=false;
signals:

public slots:
};

#endif // GRAPHICS_H
