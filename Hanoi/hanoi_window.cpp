#include "hanoi_window.h"
#include "ui_hanoi_window.h"
#include <QMessageBox>
#include <QTimer>
#include <QPropertyAnimation>
#include <QListWidgetItem>


Hanoi_Window::Hanoi_Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Hanoi_Window)
{
    ui->setupUi(this);

    about = new QAction("关于",this);
    back = new QAction("返回",this);

    // 窗口设置
    this->basic_set();

    // 生成定时器对象
    this->timer = new QTimer(this);

    // 生成链接
    this->generate_connect();
}


Hanoi_Window::~Hanoi_Window()
{
    delete ui;
}


///
/// \brief Hanoi_Window::basic_set 窗口基本设置
///
void Hanoi_Window::basic_set()
{
    // 窗口基本设置
    this->resize(1200,800);
    this->setWindowIcon(QPixmap(":/images/hanoi_2.png"));
    this->setWindowTitle("汉诺塔");
    this->ui->menubar->addAction(this->back);
    this->ui->menubar->addAction(this->about);


    this->ui->display_window->setFixedSize(900,630);
    this->ui->num_edit->setInputMethodHints(Qt::ImhFormattedNumbersOnly);
    this->ui->num_edit->setText("3");
    this->ui->display_window->setStyleSheet("QFrame{background:white;}");
    this->ui->message->setText("共需移动\t 0 \t步");
    this->ui->have_done->setText("当前已经执行\t 0 \t步");

    // 设置按钮图标
    this->ui->input->setIcon(QPixmap(":/images/begin.png"));
    this->ui->next->setIcon(QPixmap(":/images/next.png"));
    this->ui->back->setIcon(QPixmap(":/images/last.png"));
    this->ui->auto_next->setIcon(QPixmap(":/images/auto.png"));
    this->ui->reset->setIcon(QPixmap(":/images/reset.png"));
}


///
/// \brief Hanoi_Window::generate_connect 生成各组件的功能连接
///
void Hanoi_Window::generate_connect()
{
    // 菜单栏动作链接
    connect(this->back, &QAction::triggered,[=](){ emit this->backSingal(); });
    connect(this->about, &QAction::triggered,[=](){ this->about_window(); });

    // 操作按钮链接
    connect(this->ui->input,&QPushButton::clicked,[=](){ this->init(); });
    connect(this->ui->next,&QPushButton::clicked,[=](){ this->do_one_step(false);});
    connect(this->ui->back,&QPushButton::clicked,[=](){ this->do_one_step(true);});
    connect(this->ui->reset,&QPushButton::clicked,[=](){ this->reset();});
    connect(this->ui->auto_next, &QPushButton::clicked,[=](){ this->auto_next(); });

    // 定时器事件链接
    connect(this->timer, &QTimer::timeout,[=](){ if(this->do_one_step(false)==false) this->timer->stop(); });
}


///
/// \brief Hanoi_Window::init 初始化展示控件及窗口相关参数
///
void Hanoi_Window::init()
{
    // 判断是否已经初始化，防止重复初始化
    if(this->ui->display_window->init_flag==true)
    {
        QMessageBox::information(this,"提示信息","已经初始化过了");
        return ;
    }

    // 获取输入框内容，判断是否合法
    int num = this->ui->num_edit->text().toInt();
    if(num<=0||num>=15)
    {
        QMessageBox::information(this,"错误","请输入1到15之间的整数");
        this->ui->num_edit->clear();
        return;
    }

    // 初始化display_window
    this->ui->display_window->init(num, this->plates);

    // 显示共需移动的步数
    int num_of_step = this->ui->display_window->hanoi->num_of_steps;
    this->ui->message->setText(QString("共需移动\t %1 \t步").arg(num_of_step));

    // 连接所有盘子动画结束信号和窗口按钮启用函数
    for(QVector<Plate*>::iterator iter=this->plates.begin();iter!=this->plates.end();iter++)
        connect((*iter)->animation,&QPropertyAnimation::finished,this,&Hanoi_Window::enable_btn);
}


///
/// \brief Hanoi_Window::do_one_step 单步执行
/// \param anti 是否执行上一步的标志，默认为false
/// \return 如果还有可执行步骤，返回true，否则返回false
///
bool Hanoi_Window::do_one_step(bool anti)
{
    Step *step = this->ui->display_window->do_step(anti);

    // 判断步骤是否为空，如果是则禁用按钮并更新步骤列表，否则提示信息并启用按钮
    if(step!=nullptr)
    {
        // 更新步骤列表
        if(anti==false)
            this->add_step(step);
        else
            this->delete_step();

        this->disable_btn();
        return true;
    }
    else
    {
        QMessageBox::information(this,"提示信息","已无可执行步骤");
        this->enable_btn();
        return false;
    }
}


///
/// \brief Hanoi_Window::auto_next 自动执行下一步
///
void Hanoi_Window::auto_next()
{
    // 禁用按钮，并启动定时器
    this->disable_btn();
    this->timer->start(320);
}


///
/// \brief Hanoi_Window::reset 重置展示窗口函数及窗口相关参数
///
void Hanoi_Window::reset()
{
    // 启用执行步骤按钮
    this->enable_btn();

    // 断开所有盘子动画结束信号和按钮启用函数的连接
    for(QVector<Plate*>::iterator iter=this->plates.begin();iter!=this->plates.end();iter++)
        disconnect((*iter)->animation,&QPropertyAnimation::finished,this,&Hanoi_Window::enable_btn);

    // 清空盘子记录
    this->plates.clear();

    // 重置display_window
    this->ui->display_window->reset();

    // 重置步数信息
    this->ui->message->setText("共需移动\t 0 \t步");
    this->ui->have_done->setText("当前已经执行\t 0 \t步");
    this->ui->step_list->clear();
}


///
/// \brief Hanoi_Window::enable_btn 启用执行步骤按钮
///
void Hanoi_Window::enable_btn()
{
    this->ui->next->setEnabled(true);
    this->ui->back->setEnabled(true);
    this->ui->auto_next->setEnabled(true);
}


///
/// \brief Hanoi_Window::disable_btn 禁用执行步骤按钮
///
void Hanoi_Window::disable_btn()
{
    this->ui->next->setDisabled(true);
    this->ui->back->setDisabled(true);
    this->ui->auto_next->setDisabled(true);
}


///
/// \brief Hanoi_Window::add_step 在列表中添加下一步
/// \param step 下一步的步骤对象
///
void Hanoi_Window::add_step(Step *step)
{
    QString str = QString("从 %1 移动到 %2").arg(QChar('A'+step->init)).arg(QChar('A'+step->aim));
    QListWidgetItem *item = new QListWidgetItem(str);
    this->ui->step_list->addItem(item);
    this->ui->have_done->setText(QString("当前已经执行\t %1 \t步").arg(this->ui->step_list->count()));
}


///
/// \brief Hanoi_Window::delete_step 在列表中移除上一步
///
void Hanoi_Window::delete_step()
{
    QListWidgetItem *item = this->ui->step_list->takeItem(this->ui->step_list->count()-1);
    this->ui->step_list->removeItemWidget(item);
    delete item;
    this->ui->have_done->setText(QString("当前已经执行\t %1 \t步").arg(this->ui->step_list->count()));
}


///
/// \brief Hanoi_Window::about_window 关于窗口
///
void Hanoi_Window::about_window()
{
    QString text= "Title: 汉诺塔\nDeveloped by 刘啸宇.\nTime: 2020.02.24";
    QMessageBox::about(this,"关于",text);
}
