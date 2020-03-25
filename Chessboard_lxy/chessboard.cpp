#include "chessboard.h"
#include <QtMath>
#include <QPainter>
#include <QPen>
#include <QMouseEvent>
#include <QVector>
#include <QRandomGenerator>


Chessboard::Chessboard(QWidget *parent) : QFrame(parent)
{
    // 设置棋盘大小，并将背景覆盖为白色
    this->setFixedSize(769,769);
    this->background.fill(Qt::white);
}


Chessboard::~Chessboard()
{
    this->clean();
}


///
/// \brief Chessboard::init 初始化Chessboard对象
/// \param k 棋盘覆盖问题中的k值
///
void Chessboard::init(int k)
{
    // 将Chessboard中记录信息初始化
    this->is_not_init = false;
    this->confirm = false;
    this->now_step = 0;
    this->now_level = 0;

    // 计算单行方块数目和方块尺寸
    this->block_num = static_cast<int>(pow(2, k));
    this->block_size = this->width() / this->block_num;

    // 创建画家对象
    QPainter painter(&this->background);
    painter.setPen(QPen(Qt::black, 1));

    // 绘制棋盘
    this->background.fill(Qt::white);
    for (int i=0;i<=this->block_num;i++)
    {
        painter.drawLine(0, i * block_size, this->width(), i * block_size);
        painter.drawLine(i * block_size, 0, i * block_size, this->width());
    }
    this->repaint();
}


///
/// \brief Chessboard::generate_slution 生成覆盖步骤
/// \return // 判断是否已经选择了特殊块，求解完成后返回true，否则返回false
///
bool Chessboard::generate_slution()
{
    // 如果未选择特殊块，返回false
    if(this->special_block.x==-1 && this->special_block.y==-1)
        return false;

    // 将记录确定特殊块的condirm赋值为true
    this->confirm = true;

    // 求出分治方法共需的步数
    int num_of_step = 0;
    for(int num_of_block = (this->block_num*this->block_num-1)/3;num_of_block>0;num_of_step++)
        num_of_block-=static_cast<int>(pow(4,num_of_step));

    // 创建分治方法步骤向量
    for(int i=0;i<num_of_step;i++)
    {
        QVector<Tri_block * > * step = new QVector<Tri_block * >;
        this->solution_1.push_back(step);
    }

    // 求解棋盘覆盖问题
    solve(this->special_block, Sub_Chessboard(Coord(0,0), Coord(this->block_num-1,this->block_num-1)),0);
    return true;
}


///
/// \brief Chessboard::solve 求解棋盘覆盖步骤
/// \param sp    特殊块的坐标
/// \param board 传入的棋盘信息
/// \param level 按照分治方式，当前执行到的步骤数
///
void Chessboard::solve(Coord sp,Sub_Chessboard board,int level)
{
    // 如果棋盘范围中只有一个方块，结束递归
    if(board.is_one_bolck())
        return;

    int index=-1;
    Tri_block *tri_block=nullptr;

    // 将棋盘分成四份
    Sub_Chessboard left_top(board.left_top,board.center_blocks[0]);
    Sub_Chessboard right_top(Coord(board.center_blocks[1].x,board.left_top.y),Coord(board.right_bottom.x,board.center_blocks[1].y));
    Sub_Chessboard left_bottom(Coord(board.left_top.x,board.center_blocks[2].y),Coord(board.center_blocks[2].x,board.right_bottom.y));
    Sub_Chessboard right_bottom(board.center_blocks[3],board.right_bottom);
    Sub_Chessboard * boards[4] = { &left_top, &right_top, &left_bottom, &right_bottom };

    // 根据特殊块所在棋盘的位置，求解出需要绘制的tri_block，并加入到分治方法步骤向量(solution_1)中
    if(sp.x<board.center_x && sp.y<board.center_y)
    {
        tri_block = new Tri_block(board.center_blocks[1],board.center_blocks[2],board.center_blocks[3]);
        this->solution_1[level]->push_back(tri_block);
        index = 0;
    }
    else if(sp.x>board.center_x && sp.y<board.center_y)
    {
        tri_block = new Tri_block(board.center_blocks[0],board.center_blocks[2],board.center_blocks[3]);
        this->solution_1[level]->push_back(tri_block);
        index = 1;
    }
    else if(sp.x<board.center_x && sp.y>board.center_y)
    {
        tri_block = new Tri_block(board.center_blocks[0],board.center_blocks[1],board.center_blocks[3]);
        this->solution_1[level]->push_back(tri_block);
        index = 2;
    }
    else if(sp.x>board.center_x && sp.y>board.center_y)
    {
        tri_block = new Tri_block(board.center_blocks[0],board.center_blocks[1],board.center_blocks[2]);
        this->solution_1[level]->push_back(tri_block);
        index = 3;
    }

    // 将求出的tri_block加入递归方法步骤向量(solution_2)中
    this->solution_2.push_back(tri_block);

    // 执行递归
    for(int i=0;i<4;i++)
    {
        if(i==index)
            solve(sp,*boards[i],level+1);
        else
            solve(board.center_blocks[i],*boards[i],level+1);
    }
}


///
/// \brief Chessboard::draw 绘制覆盖步骤函数
/// \param recursion 是否采用递归模式选项，true为采用递归模式，false为采用分治方式，默认false
/// \return 如果当前有可执行步骤，返回true，否则返回false
///
bool Chessboard::draw(bool recursion)
{
    // 判断绘制模式
    if(recursion==true)
    {
        // 递归模式绘制棋盘
        if( now_step >= this->solution_2.size() )
            return false;

        this->draw_tri_block(this->solution_2[now_step]);

        this->now_step++;
        return true;
    }
    else
    {
        // 分治模式绘制棋盘
        if( this->now_level >= this->solution_1.size() )
            return false;

        for(int i=0;i< this->solution_1[now_level]->size();i++)
            this->draw_tri_block(this->solution_1[now_level]->at(i));

        this->now_level++;
        return true;
    }
}


///
/// \brief Chessboard::draw_tri_block 绘制随机颜色的L形骨牌
/// \param tri_block 被绘制的L形骨牌坐标
///
void Chessboard::draw_tri_block(Tri_block* tri_block)
{
    //设置画家以及随机颜色
    QPainter painter(&this->background);
    int r = QRandomGenerator::global()->bounded(100,255);
    int g = QRandomGenerator::global()->bounded(100,255);
    int b = QRandomGenerator::global()->bounded(100,255);
    painter.setBrush(QBrush(QColor(r,g,b)));

    //绘制L形骨牌
    for (int i=0;i<3;i++)
        painter.drawRect(tri_block->blocks[i].x * this->block_size, tri_block->blocks[i].y * this->block_size, this->block_size, this->block_size);

    this->repaint();
}


///
/// \brief Chessboard::clean 释放申请的Tri_block对象和QVector<Tri_block * >对象
///
void Chessboard::clean()
{
    //释放代码堆区内存
    Tri_block * tri;
    QVector<Tri_block * > * step;
    for(int j=0;j<this->solution_2.size();j++)
    {
        tri =  solution_2[j];
        delete tri;
    }
    this->solution_2.clear();
    for(int i=0;i<this->solution_1.size();i++)
    {
        step = this->solution_1[i];
        step->clear();
        delete step;
    }
    this->solution_1.clear();
}


///
/// \brief Chessboard::reset 重置Chessboard对象
///
void Chessboard::reset()
{
    // 释放步骤对象
    this->clean();

    // 重置Chessboard中的记录信息
    this->is_not_init=true;
    this->confirm=false;
    this->now_step = 0;
    this->now_level = 0;
    this->block_num = 0;
    this->block_size = 0;
    this->special_block={-1,-1};

    // 将背景重绘为白色
    this->background.fill(Qt::white);
    this->repaint();
}


///
/// \brief Chessboard::paintEvent 重写绘图事件，实现棋盘背景的绘制
///
void Chessboard::paintEvent(QPaintEvent *)
{
    // 将背景绘制到Chessboard上
    QPainter painter(this);
    painter.drawPixmap(QPoint(0,0),this->background);
}


///
/// \brief Chessboard::mousePressEvent 重写鼠标点击事件处理函数，实现对点击事件的控制
/// \param event 鼠标点击事件
///
void Chessboard::mousePressEvent(QMouseEvent* event)
{
    // 判断是否已经初始化或者确认了选择的特殊块，如果是，则不处理鼠标点击事件
    if(this->is_not_init || this->confirm)
        return;

    // 创建画家对象
    QPainter painter(&this->background);

    // 如果已经选择了特殊块，则将之前的特殊块涂白
    if (this->special_block.x!=-1)
    {
        painter.setBrush(QBrush(Qt::white));
        painter.drawRect(this->special_block.x*block_size, this->special_block.y*block_size, block_size, block_size);
    }

    // 计算新选择的特殊块的坐标
    this->special_block.x = event->x()/this->block_size;
    this->special_block.y = event->y()/this->block_size;

    // 绘制特殊块
    painter.setBrush(QBrush(Qt::black));
    painter.drawRect(this->special_block.x*block_size, this->special_block.y*block_size, block_size, block_size);
    this->repaint();
}
