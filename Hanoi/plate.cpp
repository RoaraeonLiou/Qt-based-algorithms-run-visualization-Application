#include "plate.h"
#include <QRandomGenerator>


Plate::Plate(QWidget *parent) : QLabel(parent)
{
    // 初始化动画对象
    this->animation = new QPropertyAnimation(this, "geometry");
    this->show();
}


///
/// \brief Plate::set_color 给Plate对象设置样式和随机颜色
///
void Plate::set_color()
{
    // 生成随机颜色
    int r = QRandomGenerator::global()->bounded(100,255);
    int g = QRandomGenerator::global()->bounded(100,255);
    int b = QRandomGenerator::global()->bounded(100,255);

    // 设置样式
    QString style_sheet = "Plate{ "
                            "background:rgb(%1,%2,%3);"
                            "border-radius:8px;padding:2px; "
                          "}";
    this->setStyleSheet(style_sheet.arg(r).arg(g).arg(b));
}


///
/// \brief Plate::move_pos 实现盘子的移动动画
/// \param x 目标位置的x坐标
/// \param y 目标位置的y坐标
///
void Plate::move_pos(int x, int y)
{
    // 设置动画时长
    this->animation->setDuration(300);

    // 设置起始位置，结束位置以及关键帧
    animation->setStartValue( QRect(this->x(), this->y(), this->width(), this->height()) );
    animation->setKeyValueAt(0.3,QRect(this->x(), 100, this->width(),this->height()));
    animation->setKeyValueAt(0.7,QRect(x, 100, this->width(),this->height()));
    animation->setEndValue(QRect(x,y,this->width(),this->height()));
    animation->setEasingCurve(QEasingCurve::InQuad);

    // 启动动画
    animation->start();
}
