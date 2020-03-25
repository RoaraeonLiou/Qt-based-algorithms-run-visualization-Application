#include "sort_window.h"
#include "ui_sort_window.h"
#include <QPainter>
#include <QRandomGenerator>
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QTimer>
#include <QMessageBox>
#include <QAction>
#include <QMenuBar>


Sort_Window::Sort_Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sort_Window)
{
    ui->setupUi(this);

    // 创建对象
    menubar = new QMenuBar(this);
    timer = new QTimer;
    about = new QAction("关于",this);
    back = new QAction("返回",this);

    // 添加菜单栏动作
    this->menubar->addAction(this->back);
    this->menubar->addAction(this->about);


    // 基础设置
    this->basic_set();
    this->generate_connect();
}

Sort_Window::~Sort_Window()
{
    delete ui;
}


///
/// \brief Sort_Window::basic_set 窗口基本设置
///
void Sort_Window::basic_set()
{
    // 窗口基本设置
    this->background.fill(Qt::white);
    this->setWindowIcon(QPixmap(":/images/bubble_2.png"));
    this->setWindowTitle("冒泡排序");
    this->setFixedSize(1200,800);

    // 设置按钮图标
    this->ui->begin->setIcon(QPixmap(":/images/begin.png"));
    this->ui->next_step->setIcon(QPixmap(":/images/next.png"));
    this->ui->auto_2->setIcon(QPixmap(":/images/auto.png"));

}


///
/// \brief Sort_Window::generate_connect 生成各组件的功能连接
///
void Sort_Window::generate_connect()
{
    connect(this->back, &QAction::triggered,[=](){ emit this->backSingal(); });
    connect(this->about, &QAction::triggered,[=](){ this->about_window(); });

    connect(this->ui->begin,&QPushButton::clicked,[=](){ this->begin(); });
    connect(this->ui->next_step, &QPushButton::clicked,[=](){ this->draw(); });
    connect(this->ui->auto_2, &QPushButton::clicked,[=](){ this->timer->start(250); });
    connect(this->timer, &QTimer::timeout, [=](){ this->auto_draw(); });
}


///
/// \brief Sort_Window::paintEvent 重写绘图事件
///
void Sort_Window::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(QPoint(0,0),this->background);
}


///
/// \brief Sort_Window::begin 点击开始按钮
///
void Sort_Window::begin()
{
    this->background.fill(Qt::white);

    //读取用户输入的数据
    int num = this->ui->lineEdit->text().toInt();

    //若数据不合规范则弹出对话框
    if(num<5 || num > 20)
    {
        QMessageBox::information(this, "提示信息","请输入5到20之间的整数");
        return ;
    }
    this->now = 0;
    QVector<int> array;

    //获得用户所需个数的随机数
    for(int i=0;i<num;i++)
    {
        array.push_back(QRandomGenerator::global()->bounded(0,100));
    }

    //设置初始化的画家
    QPainter painter(&this->background);
    painter.setPen(QPen(Qt::black,3));
    painter.setBrush(QBrush(Qt::black));
    painter.setFont(QFont("Arial",10));

    //数据间隔由数组个数决定
    this->span = this->background.width()/(array.length()+2);

    //画出原始的数据图像
    for(int i=0;i<array.size();i++)
    {
        painter.drawRect(QRect(span+i*span,40,50,array.at(i)*per_height));
        painter.drawText(QPoint(span+i*span+20,20),QString::number(array.at(i)));
    }
    this->repaint();

    //进行排序
    this->bubble.init(array);
}


///
/// \brief Sort_Window::draw 绘制每一步的排序步骤
/// \return 全部排序是否结束
///
bool Sort_Window::draw()
{
    this->background.fill(Qt::white);

    //设置画家
    QPainter painter(&this->background);
    painter.setPen(QPen(Qt::black,3));
    painter.setFont(QFont("Arial",10));

    //判断是否排序结束
    if(this->now == this->bubble.steps.size())
    {
        painter.setBrush(QBrush(Qt::green));
        for(int i=0;i<this->bubble.array.size();i++)
        {
            painter.drawRect(QRect(span+i*span,40,50,this->bubble.array.at(i)*per_height));
            painter.drawText(QPoint(span+i*span+20,20),QString::number(this->bubble.array.at(i)));
        }
        this->repaint();
        return false;
    }
    else if (this->now > this->bubble.steps.size())
    {
        return false;
    }
    for(int i=0;i<this->bubble.steps[this->now].array.size();i++)
    {
        painter.setBrush(QBrush(Qt::black));

        if(i==this->bubble.steps[this->now].i || i==this->bubble.steps[this->now].j)
        {
            //显示正在比较的数字
            painter.setBrush(QBrush(Qt::red));
        }
        else if ( i>this->bubble.steps[this->now].flag )
        {
            //显示排序完成的数字
            painter.setBrush(QBrush(Qt::green));
        }
        painter.drawRect(QRect(span+i*span,40,50,this->bubble.steps[this->now].array.at(i)*per_height));
        painter.drawText(QPoint(span+i*span+20,20),QString::number(this->bubble.steps[this->now].array.at(i)));
    }
    this->repaint();
    this->now++;
    return true;
}


///
/// \brief Sort_Window::auto_draw 自动执行下一步
///
void Sort_Window::auto_draw()
{
    if(draw()==false)
        this->timer->stop();
}


///
/// \brief Sort_Window::about_window 关于窗口
///
void Sort_Window::about_window()
{
    QString text= "Title: 冒泡排序\nDeveloped by 吕聪昊.\nTime: 2020.02.23";
    QMessageBox::about(this,"关于",text);
}
