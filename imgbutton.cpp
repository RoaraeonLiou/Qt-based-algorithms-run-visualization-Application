#include "imgbutton.h"


ImgButton::ImgButton(QWidget *parent) : QPushButton(parent)
{}


///
/// \brief ImgButton::set_pic 设置图片
/// \param normalImg 普通状态显示的图片
/// \param pressImg  按下状态时显示的图片
///
void ImgButton::set_pic(QString normalImg, QString pressImg)
{
    this->normalImg = normalImg;
    this->pressImg = pressImg;
    if(!this->set_style(normalImg))
        return ;
}


///
/// \brief ImgButton::set_style 设置样式
/// \param path 要显示的图片
/// \return  返回是否设置成功
///
bool ImgButton::set_style(QString path)
{
    QPixmap pix;
    bool flag = pix.load(path);
    if(!flag)
        return false;
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(),pix.height()));
    return true;
}


///
/// \brief ImgButton::mousePressEvent 点击显示按下图片
/// \param e 鼠标点击事件
///
void ImgButton::mousePressEvent(QMouseEvent *e)
{
    if( this->pressImg!="" )
    {
        if(!this->set_style(this->pressImg)){
            return ;
        }
    }
    QPushButton::mousePressEvent(e);
}


///
/// \brief ImgButton::mouseReleaseEvent 释放显示普通图片
/// \param e 鼠标点击事件
///
void ImgButton::mouseReleaseEvent(QMouseEvent *e)
{
    if( this->pressImg!="" )
    {
        if(!this->set_style(this->normalImg))
            return ;
    }
    QPushButton::mouseReleaseEvent(e);
}
