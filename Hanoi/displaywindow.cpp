#include "displaywindow.h"
#include <QPainter>
#include <QPen>
#include <QFont>
#include <QMessageBox>


DisplayWindow::DisplayWindow(QWidget *parent) : QFrame(parent)
{
    // 创建Hanoi对象和Pillar对象
    this->hanoi = new Hanoi;

    Pillar *pillar;
    for(int i=0;i<3;i++)
    {
        pillar = new Pillar();
        this->pillars.push_back(pillar);
    }
}

DisplayWindow::~DisplayWindow()
{
    if (hanoi)
        delete hanoi;
    hanoi = nullptr;
}


///
/// \brief DisplayWindow::init 初始化展示控件
/// \param num    盘子的数目
/// \param plates 题目展示窗口中记录盘子的向量
///
void DisplayWindow::init(int num, QVector<Plate*> &plates)
{
    // 初始化hanoi对象
    this->hanoi->init(num);

    // 生成盘子
    this->generate_plates(num, plates);

    // 设置初始化标志为true
    this->init_flag = true;
}


///
/// \brief DisplayWindow::generate_plates 生成Plate对象
/// \param num    盘子的数目
/// \param plates 题目展示窗口中记录盘子的向量
///
void DisplayWindow::generate_plates(int num, QVector<Plate*> &plates)
{
    // 给所有柱子设置起始y坐标
    for(int i=0;i<3;i++)
        this->pillars[i]->set_y(this->height()-130);

    // 盘子变化间隔
    int delta = this->width()/4/num;

    // 生成盘子并放入第一个柱子，记录到主窗口的盘子记录向量中
    Plate *p;
    for(int i=0;i<num;i++)
    {
        p = new Plate(this);
        p->setFixedSize((this->width()/4-delta*i), HEIGHT);
        p->set_color();
        p->move(this->width()/24+delta/2*i,this->pillars[0]->top_y);
        this->pillars[0]->move_in(p);
        plates.push_back(p);
    }
}


///
/// \brief DisplayWindow::do_step 执行步骤
/// \param anti 是否执行上一步的标志，默认未false
/// \return 返回执行的步骤
///
Step * DisplayWindow::do_step(bool anti)
{

    Step *step;
    int start,end;

    // 判断anti的值，如果为true时，获取上一步，否则获取下一步
    if(anti)
        step = this->hanoi->get_last_step();
    else
        step = this->hanoi->get_next_step();

    // 判断步骤是否为空
    if(step==nullptr)
        return nullptr;

    // 根据anti的值设置起始柱子和目标柱子
    if(anti)
    {
        start = step->aim;
        end = step->init;
    }
    else
    {
        start = step->init;
        end = step->aim;
    }

    // 获取要移动的盘子
    Plate *plate = this->pillars[start]->move_out();

    // 计算终点坐标
    int x=plate->x() + (end - start) * this->width() / 3;
    int y=this->pillars[end]->top_y;

    // 移动盘子
    plate->move_pos(x, y);
    this->pillars[end]->move_in(plate);

    // 返回执行的步骤
    return step;
}


///
/// \brief DisplayWindow::reset 重置展示控件
///
void DisplayWindow::reset()
{
    // 重置hanoi对象
    this->hanoi->reset();

    // 重置pillar对象
    for(int i=0;i<3;i++)
        this->pillars[i]->reset();

    // 设置初始化标志为false
    this->init_flag = false;
}


///
/// \brief DisplayWindow::paintEvent 重写绘图事件，用于实现窗口背景的展示
///
void DisplayWindow::paintEvent(QPaintEvent *)
{
    // 创建QPainter、QPen、QFont对象
    QPainter painter(this);
    QPen pen(Qt::black,5,Qt::SolidLine);
    QFont font("Arial Black",40);

    // 设置画笔和字体
    painter.setPen(pen);
    painter.setFont(font);

    // 绘制展示窗口内容
    for(int i=0;i<3;i++)
    {
        painter.drawLine(QPoint((2*i+1)*this->width()/6,100),QPoint((2*i+1)*this->width()/6,this->height()-100));
        painter.drawText(QPoint((2*i+1)*this->width()/6-20,this->height()-100+50), QChar('A'+i));
    }
    painter.drawLine(QPoint(10,this->height()-100),QPoint(this->width()-10,this->height()-100));
}
