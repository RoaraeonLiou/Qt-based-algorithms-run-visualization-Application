#ifndef IMGBUTTON_H
#define IMGBUTTON_H

#include <QPushButton>

class ImgButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ImgButton(QWidget *parent = nullptr);

    QString normalImg;  //静默状态下图片路径
    QString pressImg;   //按下时的图片路径

    void set_pic(QString normalImg, QString pressImg);
    bool set_style(QString path);

    //重写按钮按下和释放事件
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

signals:

public slots:
};

#endif // IMGBUTTON_H
