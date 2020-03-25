#include "algorithm.h"
#include <cmath>


algorithm::algorithm(int k, int cpiecesx, int cpiecesy)
{
    //初始化变量
    orderflag = 1;
    cartoonOrder=0;
    ck=k;
    this->cpiecesx=cpiecesx;
    this->cpiecesy=cpiecesy;
    csize=int(exp2(k));

    //定义动态二维数组，表示棋盘
    for(int i =0;i<csize;i++)
    {
        board[i]=new int[csize];
        order[i]= new int[csize];
    }

    //初始化二维数组
    for(int i=0;i<csize;i++)
    {
        for(int j=0;j<csize;j++)
        {
            board[i][j]=0;
            order[i][j]=0;
        }
    }
}

algorithm::~algorithm()
{
    //释放动态数组申请的空间
    for(int i=0;i<csize;i++)
    {
        delete [] board[i];
        delete [] order[i];
    }

    delete [] board;
    delete [] order;
}


///
/// \brief algorithm::coverfunction 棋盘覆盖求解
/// \param cstartx  开始x坐标
/// \param cstarty  开始y坐标
/// \param cpiecesx 特殊块x坐标
/// \param cpiecesy 特殊块y坐标
/// \param csize    棋盘大小
///
void algorithm::coverfunction(int cstartx, int cstarty, int cpiecesx, int cpiecesy, int csize)
{
    //递归到1*1方格时结束
    if(csize==1)
        return;

    int orderflaguse =  orderflag++;    //L型方格顺序
    int csizeuse = csize/2;             //将棋盘分为4部分递归

    //覆盖左上角
    if(cpiecesx<cstartx+csizeuse && cpiecesy<cstarty+csizeuse)
    {
        //如果特殊方格在左上角，就继续递归
        coverfunction(cstartx,cstarty,cpiecesx,cpiecesy,csizeuse);
    }
    else
    {
        //特殊方格不在左上角就在右下方格标记
        board[cstartx+csizeuse-1][cstarty+csizeuse-1]=orderflaguse;//记录L型方格顺序，实现分步绘制时使用
        cartoonOrder++;
        order[cstartx+csizeuse-1][cstarty+csizeuse-1]=cartoonOrder;//记录每个方格被标记的顺序，实现算法追踪时使用
        //标记之后标记的方格作为特殊方格继续递归
        coverfunction(cstartx,cstarty,cstartx+csizeuse-1,cstarty+csizeuse-1,csizeuse);
    }

    //覆盖右上角
    if(cpiecesx<cstartx+csizeuse && cpiecesy>=cstarty+csizeuse)
    {
        //如果特殊方格在右上角，就继续递归
        coverfunction(cstartx,cstarty+csizeuse,cpiecesx,cpiecesy,csizeuse);
    }
    else
    {
        //特殊方格不在右上角就在左下方格标记，标记后递归
        board[cstartx+csizeuse-1][cstarty+csizeuse] = orderflaguse;
        cartoonOrder++;
        order[cstartx+csizeuse-1][cstarty+csizeuse] = cartoonOrder;
        coverfunction(cstartx,cstarty+csizeuse,cstartx+csizeuse-1,cstarty+csizeuse,csizeuse);
    }

    //覆盖左下角
    if(cpiecesx>=cstartx+csizeuse && cpiecesy<cstarty+csizeuse)
    {
        //如果特殊方格在左下角，就继续递归
        coverfunction(cstartx+csizeuse,cstarty,cpiecesx,cpiecesy,csizeuse);
    }
    else
    {
        //特殊方格不在左下角就在右上方格标记，标记后递归
        board[cstartx+csizeuse][cstarty+csizeuse-1]=orderflaguse;
        cartoonOrder++;
        order[cstartx+csizeuse][cstarty+csizeuse-1]=cartoonOrder;
        coverfunction(cstartx+csizeuse,cstarty,cstartx+csizeuse,cstarty+csizeuse-1,csizeuse);
    }

    //覆盖右下角
    if(cpiecesx>=cstartx+csizeuse && cpiecesy>=cstarty+csizeuse)
    {
        //如果特殊方格在右下角，就继续递归
        coverfunction(cstartx+csizeuse,cstarty+csizeuse,cpiecesx,cpiecesy,csizeuse);
    }
    else
    {
        //特殊方格不在右下角就在左上方格标记，标记后递归
        board[cstartx+csizeuse][cstarty+csizeuse]=orderflaguse;
        cartoonOrder++;
        order[cstartx+csizeuse][cstarty+csizeuse]=cartoonOrder;
        coverfunction(cstartx+csizeuse,cstarty+csizeuse,cstartx+csizeuse,cstarty+csizeuse,csizeuse);
    }
}
