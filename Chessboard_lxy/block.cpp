#include "block.h"


///
/// \brief Coord::Coord Coord构造函数
/// \param x 用于初始化Coord对象的x坐标
/// \param y 用于初始化Coord对象的y坐标
///
Coord::Coord(int x, int y)
{
    this->x = x;
    this->y = y;
}


///
/// \brief Coord::operator == 重载==运算符
/// \param c 被比较的Coord对象
/// \return 返回两个Coord对象的x和y是否均相等，相等返回true，否则返回false
///
bool Coord::operator== (const Coord& c)
{
    return this->x==c.x && this->y==c.y;
}


///
/// \brief Tri_block::Tri_block Tri_block构造函数
/// \param first    第一个方块的坐标
/// \param second   第二个方块的坐标
/// \param third    第三个方块的坐标
///
Tri_block::Tri_block(Coord first,Coord second, Coord third)
{
    this->blocks[0] = first;
    this->blocks[1] = second;
    this->blocks[2] = third;
}


///
/// \brief Sub_Chessboard::Sub_Chessboard Sub_Chessboard构造函数
/// \param left_top     左上方块坐标
/// \param right_bottom 右下方块坐标
///
Sub_Chessboard::Sub_Chessboard(Coord left_top, Coord right_bottom)
{
    //记录左上和右下方块坐标
    this->left_top = left_top;
    this->right_bottom = right_bottom;
    //计算中心坐标
    this->center_x = (static_cast<double>(left_top.x)+static_cast<double>(right_bottom.x))/2;
    this->center_y = (static_cast<double>(left_top.y)+static_cast<double>(right_bottom.y))/2;
    //计算中心四个方块的坐标
    this->center_blocks[0] = Coord(static_cast<int>(center_x-0.5),static_cast<int>(center_y-0.5));
    this->center_blocks[1] = Coord(static_cast<int>(center_x+0.5),static_cast<int>(center_y-0.5));
    this->center_blocks[2] = Coord(static_cast<int>(center_x-0.5),static_cast<int>(center_y+0.5));
    this->center_blocks[3] = Coord(static_cast<int>(center_x+0.5),static_cast<int>(center_y+0.5));
}


///
/// \brief Sub_Chessboard::is_one_bolck 判断Sub_Chessboard所示区域是否只有一个方块
/// \return 如果Sub_Chessboard所示区域内只有一个方块，返回true，否则返回false
///
bool Sub_Chessboard::is_one_bolck()
{
    return this->left_top==this->right_bottom;
}
