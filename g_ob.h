#ifndef G_OB_H
#define G_OB_H


class g_ob
{
public:
    g_ob();
    g_ob(int cx, int cy);
    int x;
    int y;
    int rx=0; //điểm neo x-rx=vị trí top left của ảnh
    int ry=0;
    int wi=0; //Chiều rộng/dài
    int he=0;
};

#endif // G_OB_H
