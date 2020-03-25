#include "pillar.h"

///
/// \brief Pillar::set_y 设置柱子下一个盘子应该放置的y坐标
/// \param y 下一个盘子应该放置的y坐标
///
void Pillar::set_y(int y)
{
    this->top_y = y;
}


///
/// \brief Pillar::isEmpty // 判断Pillar中是否有Plate对象
/// \return 有则返回true，否则返回false
///
bool Pillar::isEmpty()
{
    return this->plates.isEmpty();
}


///
/// \brief Pillar::move_out 将柱子上的盘子移除并返回
/// \return Pillar对象中栈顶的Plate指针，无可返回的指针时，返回nullptr
///
Plate* Pillar::move_out()
{
    // 判断是否没有盘子，如果没有，返回nullptr
    if(this->plates.isEmpty())
        return nullptr;
    // 更新top_y
    this->top_y+=HEIGHT;
    return this->plates.pop();
}

///
/// \brief Pillar::move_in 将传入的Plate指针压入栈中
/// \param p Plate对象指针
///
void Pillar::move_in(Plate* p)
{
    // 更新top_y
    this->top_y-=HEIGHT;
    this->plates.push(p);
    return;
}


///
/// \brief Pillar::reset 重置Pillar对象
///
void Pillar::reset()
{
    // 将所有的盘子释放
    while(this->plates.isEmpty()==false)
    {
        Plate *p = this->move_out();
        if(p)
            delete p;
    }
}
