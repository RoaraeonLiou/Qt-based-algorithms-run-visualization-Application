#ifndef PLATE_H
#define PLATE_H

#include <QLabel>
#include <QPropertyAnimation>

///
/// \brief The Plate class 实现盘子的显示与移动
///
class Plate : public QLabel
{
    Q_OBJECT
public:
    explicit Plate(QWidget *parent = nullptr);
    void move_pos(int x, int y);    // 实现盘子的移动动画
    void set_color();               // 给Plate对象设置样式和随机颜色

    QPropertyAnimation *animation;
};

#endif // PLATE_H
