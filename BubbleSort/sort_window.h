#ifndef SORT_WINDOW_H
#define SORT_WINDOW_H

#include <QWidget>
#include <QPixmap>
#include <QTimer>
#include "bubble_sort.h"
#include <QMenuBar>

namespace Ui {
class Sort_Window;
}


///
/// \brief The Sort_Window class 排序窗口，负责展示冒泡排序的题目
///
class Sort_Window : public QWidget
{
    Q_OBJECT

public:
    explicit Sort_Window(QWidget *parent = nullptr);
    ~Sort_Window();

signals:
    void backSingal(); //返回主页信号

protected:
    void paintEvent(QPaintEvent *);

private:
    Ui::Sort_Window *ui;
    QPixmap background = QPixmap(1200,700);
    Bubble_Sort bubble;
    QTimer *timer;              //自动运行定时器

    void basic_set();           //窗口基本设置
    void generate_connect();    //生成各组件的功能链接
    void begin();               //初始化显示原始数据
    bool draw();                //单步执行
    void auto_draw();           //自动执行下一步
    void about_window();

    int now;                    //记录当前执行的步骤
    int span;                   //打印图形的间隔
    int per_height = 6;         //单位高度
    QMenuBar * menubar;         //菜单栏
    QAction *about;             //关于
    QAction *back;              //返回上一级
};

#endif // SORT_WINDOW_H
