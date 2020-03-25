#ifndef ALGORITHM_H
#define ALGORITHM_H

class algorithm
{
public:
    algorithm(int k, int cpiecesx,int cpiecesy);
    ~algorithm();
    void coverfunction(int cstartx, int cstarty,int cpiecesx,int cpiecesy, int csize);

    int ck;         //棋盘级数
    int csize;      //棋盘大小
    int cpiecesx;   //特殊方块横坐标
    int cpiecesy;   //特殊方块纵坐标
    int **board = new int *[csize]; //动态二位数组表示棋盘，记录L型方块顺序
    int **order = new int *[csize]; //内容记录每一步递归顺序

private:
    int orderflag;      //L型方块顺序
    int cartoonOrder;   //递归顺序
};

#endif // ALGORITHM_H
