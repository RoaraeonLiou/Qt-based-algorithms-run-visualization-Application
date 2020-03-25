#include "Chessboard_lxy/chessboard_window.h"
#include "ui_chessboard_window.h"
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QIntValidator>

Chessboard_Window::Chessboard_Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Chessboard_Window)
{
    ui->setupUi(this);

    // 创建定时器对象
    timer = new QTimer(this);
    about = new QAction("关于",this);
    back = new QAction("返回",this);

    // 窗口基本设置
    this->basic_set();

    // 生成链接
    this->generate_connect();
}


Chessboard_Window::~Chessboard_Window()
{
    delete ui;
}


///
/// \brief Chessboard_Window::basic_set 窗口基本设置
///
void Chessboard_Window::basic_set()
{
    // 设置窗口信息
    this->resize(1200,800);
    this->setWindowIcon(QPixmap(":/images/chessboard_3.png"));
    this->setWindowTitle("棋盘翻转");
    this->ui->menubar->addAction(this->back);
    this->ui->menubar->addAction(this->about);

    // 设置单选框默认值，按钮默认状态，输入框输入限制
    this->ui->divide_choice->setChecked(true);
    this->ui->k_edit->setValidator(new QIntValidator(0,100,this));
    this->ui->do_one_step->setDisabled(true);
    this->ui->auto_do_step->setDisabled(true);

    // 设置按钮图标
    this->ui->begin->setIcon(QPixmap(":/images/begin.png"));
    this->ui->confirm->setIcon(QPixmap(":/images/confirm.png"));
    this->ui->do_one_step->setIcon(QPixmap(":/images/next.png"));
    this->ui->auto_do_step->setIcon(QPixmap(":/images/auto.png"));
    this->ui->reset->setIcon(QPixmap(":/images/reset.png"));
}


///
/// \brief Chessboard_Window::generate_connect 生成各组件的功能连接
///
void Chessboard_Window::generate_connect()
{
    // 链接菜单栏功能
    connect(this->back, &QAction::triggered,[=](){ emit this->backSingal(); });
    connect(this->about, &QAction::triggered,[=](){ this->about_window(); });

    // 链接按钮功能
    connect(this->ui->begin,&QPushButton::clicked,[=](){ this->draw_chessboard(); });
    connect(this->ui->confirm, &QPushButton::clicked,[=](){ this->confirm_special_block(); });
    connect(this->ui->do_one_step, &QPushButton::clicked,[=](){ this->cover_block(); });
    connect(this->ui->auto_do_step, &QPushButton::clicked,[=](){ this->timer->start(400); });
    connect(this->ui->reset, &QPushButton::clicked,[=](){ this->reset(); });

    //链接定时器功能
    connect(this->timer,&QTimer::timeout,[=](){ this->cover_block_auto();  });

    // 链接单选框功能
    connect(this->ui->divide_choice,&QRadioButton::clicked,[=](){ if(this->is_recursion) this->is_recursion=false; });
    connect(this->ui->recursion_choice,&QRadioButton::clicked,[=](){ if(this->is_recursion==false) this->is_recursion=true; });
}

///
/// \brief Chessboard_Window::draw_chessboard 绘制棋盘
///
void Chessboard_Window::draw_chessboard()
{
    // 绘制棋盘
    QString text = this->ui->k_edit->text();

    // 判断输入内容是否为空
    if(text.isEmpty())
    {
        QMessageBox::information(this,"提示信息","请输入k的值");
        return;
    }

    // 判断输入的k的值是否合法
    int k = text.toInt();
    if(k<0 || k>8)
    {
        QMessageBox::information(this,"提示信息","请确保k的值在0到8之间");
        return;
    }

    // 对chessboard进行初始化
    this->is_draw = true;
    this->ui->chessboard->init(k);
}


///
/// \brief Chessboard_Window::confirm_special_block 确认特殊块的位置
///
void Chessboard_Window::confirm_special_block()
{
    // 判断当前棋盘是都已经绘制
    if(this->is_draw == false)
    {
        QMessageBox::information(this,"提示信息","请先绘制棋盘");
        return;
    }

    // 判断是否已经确认特殊块
    this->is_confirm = this->ui->chessboard->generate_slution();
    if(this->is_confirm==false)
    {
        QMessageBox::information(this,"提示信息","请先选择特殊块");
        return;
    }

    // 设置按钮状态
    this->ui->begin->setDisabled(true);
    this->ui->confirm->setDisabled(true);
    this->ui->do_one_step->setEnabled(true);
    this->ui->auto_do_step->setEnabled(true);
}


///
/// \brief Chessboard_Window::cover_block 单步覆盖棋盘
/// \return 还有可执行步骤时返回true，否则返回false
///
bool Chessboard_Window::cover_block()
{
    // 判断是否已经求解完成
    if( this->is_done==true )
    {
        QMessageBox::information(this,"提示信息","已求解完成");
        return false;
    }

    // 判断是否已经绘制棋盘
    if( this->is_draw==false )
    {
        QMessageBox::information(this,"提示信息","请先绘制棋盘");
        return false;
    }

    // 判断是否已经确认选择的特殊块
    if( this->is_confirm==false )
    {
        QMessageBox::information(this,"提示信息","请确认特殊块");
        return false;
    }

    // 禁用演示覆盖模式的选项
    this->ui->divide_choice->setDisabled(true);
    this->ui->recursion_choice->setDisabled(true);

    // 进行棋盘覆盖，并在结束时禁用执行覆盖按钮
    if( this->ui->chessboard->draw(this->is_recursion)==false )
    {
        this->is_done = true;
        QMessageBox::information(this,"提示信息","已求解完成");
        this->ui->auto_do_step->setDisabled(true);
        this->ui->do_one_step->setDisabled(true);
        return false;
    }
    return true;
}


///
/// \brief Chessboard_Window::cover_block_auto 按照定时器时间间隔自动覆盖棋盘
///
void Chessboard_Window::cover_block_auto()
{
    // 自动执行覆盖动作，结束之后停止定时器
    if(this->cover_block()==false)
        this->timer->stop();
}


///
/// \brief Chessboard_Window::reset 重置棋盘与窗口记录信息
///
void Chessboard_Window::reset()
{
    // 重置主窗口记录信息
    this->is_draw = false;
    this->is_confirm = false;
    this->is_done = false;
    this->is_recursion = false;

    // 重置主窗口覆盖模式选项
    this->ui->divide_choice->setEnabled(true);
    this->ui->recursion_choice->setEnabled(true);
    this->ui->divide_choice->setChecked(true);

    // 重置主窗口按钮状态
    this->ui->begin->setEnabled(true);
    this->ui->confirm->setEnabled(true);
    this->ui->do_one_step->setDisabled(true);
    this->ui->auto_do_step->setDisabled(true);

    // 重置chessboard对象
    this->ui->chessboard->reset();
}


///
/// \brief Chessboard_Window::about_window 关于窗口
///
void Chessboard_Window::about_window()
{
    QString text= "Title: 棋盘覆盖\nDeveloped by 刘啸宇.\nTime: 2020.02.25";
    QMessageBox::about(this,"关于",text);
}
