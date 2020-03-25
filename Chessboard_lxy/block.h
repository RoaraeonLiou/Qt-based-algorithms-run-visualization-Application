#ifndef BLOCK_H
#define BLOCK_H

///
/// \brief The Coord struct 用于记录方块坐标
///
typedef struct Coord
{
    Coord(int x=-1, int y=-1);
    bool operator== (const Coord& c);   // 重载==运算符

    int x;      //方块的x坐标
    int y;      //方块的y坐标
}Coord;


///
/// \brief The Tri_block class 用于记录L形骨牌三个方块的坐标，并记录在数组中
///
class Tri_block
{
public:
    Tri_block(Coord first, Coord second, Coord third);

    Coord blocks[3];    //记录三个方块的坐标数组
};


///
/// \brief The Sub_Chessboard class 用于记录棋盘分块后各棋盘的中心坐标，中心四个方块坐标的信息
///
class Sub_Chessboard
{
public:
    Sub_Chessboard(Coord left_top, Coord right_bottom);
    bool is_one_bolck();    //判断棋盘是否只有一个方块


    Coord center_blocks[4]; //记录中心四个方块的坐标数组
    Coord left_top;         //棋盘左上方块的坐标
    Coord right_bottom;     //棋盘右下方块的坐标
    double center_x;        //中心坐标x
    double center_y;        //中心坐标y
};


#endif // BLOCK_H
