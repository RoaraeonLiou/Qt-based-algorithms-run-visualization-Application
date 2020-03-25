#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QFrame>
#include <QPixmap>
#include <QVector>
#include "block.h"

///
/// \brief The Chessboard class 棋盘类
///     继承于QFrame，用于实现棋盘覆盖问题的算法展示，
///  可以根据传入k值进行初始化并绘制棋盘，在确认特殊方块
///  之后可以生成两种顺序的覆盖步骤，可以按照指定顺序覆盖
///  棋盘。
///
class Chessboard : public QFrame
{
    Q_OBJECT
public:
    explicit Chessboard(QWidget *parent = nullptr);
    ~Chessboard();
    void init(int k);                   // 初始化Chessboard对象
    bool generate_slution();            // 生成棋盘覆盖问题的解决步骤
    bool draw(bool recursion=false);    // 按照指定顺序覆盖棋盘
    void reset();                       // 重置Chessboard对象

    int now_step = 0;                   // 记录当前按照递归顺序执行到的步骤数
    int now_level=0;                    // 记录当前按照分治顺序执行到的步骤数
    bool is_not_init=true;              // 记录是否已经初始化
    bool confirm=false;                 // 记录是否确认选择的特殊块
    int block_num;                      // 记录一行或一列中方块的数目
    int block_size;                     // 记录单个方块的边长
    QPixmap background = QPixmap(768,768);          // 棋盘的绘制设备
    Coord special_block={-1,-1};                    // 记录特殊块的坐标
    QVector<QVector<Tri_block * > * > solution_1;   // 记录分治顺序的覆盖步骤
    QVector<Tri_block * > solution_2;               // 记录递归顺序的覆盖步骤

protected:
    void paintEvent(QPaintEvent *);                 // 重写绘图事件
    void mousePressEvent(QMouseEvent* event);       // 重写鼠标点击事件

private:
    void solve(Coord sp,Sub_Chessboard board,int level);    // 解决棋盘覆盖问题的递归函数
    void draw_tri_block(Tri_block* tri_block);              // 绘制L形骨牌
    void clean();                                           // 清空步骤
};

#endif // CHESSBOARD_H
