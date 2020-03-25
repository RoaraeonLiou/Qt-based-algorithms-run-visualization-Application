#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H


#include <QFrame>
#include "hanoi.h"
#include "plate.h"
#include "pillar.h"


///
/// \brief The DisplayWindow class 用于展示汉诺塔移动过程的控件
///
class DisplayWindow : public QFrame
{
    Q_OBJECT
public:
    explicit DisplayWindow(QWidget *parent = nullptr);
    ~DisplayWindow();
    void init(int num, QVector<Plate*> &plates);            // 初始化展示控件
    void generate_plates(int num, QVector<Plate*> &plates); // 生成Plate对象
    Step * do_step(bool anti=false);                        // 执行步骤
    void reset();                                           // 重置展示控件

    QVector<Pillar*> pillars;       // 存储Pillar对象
    Hanoi *hanoi;                   // Hanoi对象指针
    bool init_flag=false;           // 记录是否已经初始化

protected:
    void paintEvent(QPaintEvent *);

};


#endif // DISPLAYWINDOW_H
