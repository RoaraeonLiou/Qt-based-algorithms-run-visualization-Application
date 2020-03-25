#ifndef HANOI_WINDOW_H
#define HANOI_WINDOW_H

#include <QMainWindow>
#include "hanoi.h"
#include "plate.h"
#include <QVector>

namespace Ui {
class Hanoi_Window;
}

///
/// \brief The Hanoi_Window class 汉诺塔窗口，负责展示汉诺塔题目
///
class Hanoi_Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Hanoi_Window(QWidget *parent = nullptr);
    ~Hanoi_Window();

signals:
    void backSingal();  //返回主页信号

private:
    Ui::Hanoi_Window *ui;
    QTimer *timer;          // 自动运行定时器
    QVector<Plate*> plates; // 盘子记录向量

    void basic_set();           // 窗口基本设置
    void generate_connect();    // 生成各组件的功能连接
    void init();                // 初始化展示控件及窗口相关参数
    bool do_one_step(bool anti);// 单步执行,anti为true时,执行上一步
    void auto_next();           // 自动执行下一步
    void reset();               // 重置展示窗口函数及窗口相关参数
    void enable_btn();          // 启用执行步骤按钮
    void disable_btn();         // 禁用执行步骤按钮
    void add_step(Step *step);  // 在列表中添加下一步
    void delete_step();         // 在列表中移除上一步
    void about_window();
    QAction *about;             // 关于
    QAction *back;              // 返回上一级
};

#endif // HANOI_WINDOW_H
