#ifndef PILLAR_H
#define PILLAR_H

#include <QStack>
#include "plate.h"

const int HEIGHT=25;    //Plate对象的高度

///
/// \brief The Pillar class 存储Plate对象，并配合Hanoi对象实现移动步骤的展示
///
class Pillar
{
public:
    void set_y(int y);      // 设置下一个盘子应该放置的y坐标
    bool isEmpty();         // 判断Pillar中的栈是否为空
    Plate* move_out();      // 移出栈顶Platet指针，并返回指针
    void move_in(Plate *p); // 将传入的Plate指针入栈
    void reset();           // 重置Pillar对象

    int top_y;              // 记录下一个盘子应该放置的y坐标

private:
    QStack<Plate*> plates;  // 存储Plate对象指针的栈
};


#endif // PILLAR_H
