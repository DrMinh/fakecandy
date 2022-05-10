#ifndef KHOIKEO_H
#define KHOIKEO_H
#include "g_tilelist.h"
#include "g_anh.h"
#include "g_ob.h"
class khoikeo
{
public:
    khoikeo();
    explicit khoikeo(int x,int y,g_tilelist T,int num);
    //
    g_anh anhkeo;
    g_ob mt;
    g_ob tangtoc;
    g_ob hoandi;
    int luotM=-1; //Đánh dấu lượt di chuyển hiện tại (một số kẹo thay đổi qua các lượt)
    int diemVe; //Điểm để quay về khi lỗi -1 -1 nếu không có điểm về
    bool diemCheck=true; //Điểm cần check với các điểm còn lại
    bool khop=true; //Cho biết loại kẹo này có khớp 3 không (kẹo đb không nổ khi khớp 3)
    bool huy=false; //Cho biết khối kẹo sẽ bị hủy
    int mau=1; //Dùng để tính "giáp" của một số loại kẹo ngăn bị hủy
    int keoID; //ID của kẹo
    //
//    khoikeo makeCopy();
    void changeMat(int cx,int cy);
    bool isStop();
    bool nhanSat();
    void Tick(QPainter &PT);
};

#endif // KHOIKEO_H
