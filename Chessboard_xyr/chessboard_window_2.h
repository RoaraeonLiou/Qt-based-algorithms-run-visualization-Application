#ifndef CHESSBOARD_WINDOW_2_H
#define CHESSBOARD_WINDOW_2_H

#include <QWidget>
#include "algorithm.h"
#include <QMenuBar>

namespace Ui {
class Chessboard_Window_2;
}

class Chessboard_Window_2 : public QWidget
{
    Q_OBJECT

public:
    explicit Chessboard_Window_2(QWidget *parent = nullptr);
    ~Chessboard_Window_2();
    void generate_connect();
    void basic_set();

signals:
    void backSingal(); //返回主页信号

public slots:
    void OnPushButtonOK();      //确定棋盘级数按钮的槽函数
    void OnPushButtonDraw();    //结果绘制按钮的槽函数

protected:
    void paintEvent(QPaintEvent *);         //重写绘图事件
    void mousePressEvent(QMouseEvent *e);   //重写鼠标点击事件，实现鼠标选取特殊方格

private: 
    Ui::Chessboard_Window_2 *ui;
    int k;              //棋盘级数
    int size;           //棋盘大小
    int startx;         //棋盘开始横坐标
    int starty;         //棋盘开始纵坐标
    int chessw;         //期盘方格宽度
    int chessh;         //棋盘方格高度
    int piecesx;        //特殊方块横坐标
    int piecesy;        //特殊方块纵坐标
    int drawflag;       //结果绘制开始标志
    int fenbuflag;      //分步绘制动画开始标志
    int zhuizongflag;   //算法追踪动画开始标志
    int q;              //分步绘制动画定时器变化标志
    int o;              //算法追踪动画定时器变化标志
    QTimer *timer1;
    QTimer *timer2;
    algorithm *algo;            //算法类对象，与算法实现互动
    QMenuBar * menubar;         //菜单栏
    QAction *about;             //关于
    QAction *back;              //返回上一级

    void about_window();
};

#endif // CHESSBOARD_WINDOW_2_H
