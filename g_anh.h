#ifndef G_ANH_H
#define G_ANH_H
#include "g_ob.h"
#include "g_tilelist.h"
#include <QPixmap>
#include <QRect>
#include <QPainter>
class g_anh : public g_ob
{
public:
    g_anh();
    explicit g_anh(int cx, int cy, g_tilelist Tile, int cFrame=0);
    //
    g_tilelist Tileset;
    int nowFrame=0;
    int idhoat=-1; //id hoạt hình đang phát, -1: ảnh tĩnh
    int frameCon=0; //số khung còn lại trước khi chuyển ảnh mới
    float zoom=1; //>1 phóng */to,<1 thu nhỏ so với kích thức pix
    QPointF tarzoom; //x: Mức zoom cần đạt (-1:không zoom),y: tốc độ zoom
//    int A=0;
    //Event
    bool Hasclick=true; //Có sự kiện?
    bool Hasclick_Pix=true; //Có tính chính xác pixel khi chuột click?
    //hành động
//    void catAnh(int y);
    void draw(QPainter &PT);
    int changeFrame(int Frame);
    int Tick(QPainter &PT);
    int TestChuot(QPoint Cur);
    void Click(QPainter &PT);
};

#endif // G_ANH_H
