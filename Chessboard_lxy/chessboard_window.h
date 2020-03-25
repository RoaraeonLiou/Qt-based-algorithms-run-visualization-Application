#ifndef CHESSBOARD_WINDOW_H
#define CHESSBOARD_WINDOW_H

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class Chessboard_Window;
}

///
/// \brief The Chessboard_Window class 棋盘覆盖窗口，负责展示棋盘覆盖题目
///
class Chessboard_Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Chessboard_Window(QWidget *parent = nullptr);
    ~Chessboard_Window();

signals:
    void backSingal();  // 返回主页信号

private:
    Ui::Chessboard_Window *ui;
    QTimer *timer;                  // 自动执行下一步定时器
    bool is_draw = false;           // 记录是否已经绘制棋盘
    bool is_confirm = false;        // 记录是否已经确认特殊块的位置
    bool is_done = false;           // 记录覆盖是否已经完成
    bool is_recursion = false;      // 记录覆盖顺序类型

    void basic_set();               // 窗口基本设置
    void generate_connect();        // 生成各组件的功能连接
    void draw_chessboard();         // 绘制棋盘
    void confirm_special_block();   // 确认特殊块的位置
    bool cover_block();             // 单步覆盖棋盘
    void cover_block_auto();        // 按照定时器时间间隔自动覆盖棋盘
    void reset();                   // 重置棋盘与窗口记录信息
    void about_window();

    QAction *about;
    QAction *back;
};

#endif // CHESSBOARD_WINDOW_H
