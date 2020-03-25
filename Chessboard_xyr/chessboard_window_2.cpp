#include "chessboard_window_2.h"
#include "ui_chessboard_window_2.h"
#include "algorithm.h"
#include <QString>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QSpinBox>
#include <QMouseEvent>
#include <QRect>
#include <QPixmap>
#include <QRandomGenerator>
#include <cmath>
#include <QTimer>
#include <QObject>
#include <QAction>
#include <QMessageBox>


Chessboard_Window_2::Chessboard_Window_2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chessboard_Window_2)
{
    ui->setupUi(this);

    //定义定时器对象
    timer1 = new QTimer(this);
    timer2 = new QTimer(this);

    // 窗口基本设置
    this->basic_set();

    // 生成连接
    this->generate_connect();
}

Chessboard_Window_2::~Chessboard_Window_2()
{
    delete algo;
    delete ui;
}


///
/// \brief Chessboard_Window_2::basic_set 窗口基本设置
///
void Chessboard_Window_2::basic_set()
{
    // 设置窗口基本信息
    this->setWindowIcon(QPixmap(":/images/chessboard_2.png"));
    this->setWindowTitle("棋盘覆盖");

    // 创建菜单栏及动作
    menubar = new QMenuBar(this);
    about = new QAction("关于",this);
    back = new QAction("返回",this);
    this->menubar->addAction(this->back);
    this->menubar->addAction(this->about);

    // 限制棋盘大小最大值为4，避免棋盘级数过大导致程序崩溃
    ui->spinBoxSize->setMaximum(4);

    // 初始化变量
    q=0;
    o=0;
    size=1;
    piecesx=-1;
    piecesy=-1;
    drawflag=-1;
    fenbuflag=-1;
    zhuizongflag=-1;

    // 设置绘制按钮为不可用
    ui->pushButtonDraw->setEnabled(false);
    ui->btnFenbu->setEnabled(false);
    ui->btnZhuizong->setEnabled(false);
    ui->btnStop->setEnabled(false);
    ui->btnContinue->setEnabled(false);

    // 设置按钮图标
    this->ui->pushButtonOK->setIcon(QPixmap(":/images/begin.png"));
    this->ui->btnPosOK->setIcon(QPixmap(":/images/confirm.png"));
    this->ui->btnStop->setIcon(QPixmap(":/images/stop.png"));
    this->ui->btnContinue->setIcon(QPixmap(":/images/continue.png"));
    this->ui->btnZhuizong->setIcon(QPixmap(":/images/choice.png"));
    this->ui->btnFenbu->setIcon(QPixmap(":/images/choice.png"));
    this->ui->pushButtonDraw->setIcon(QPixmap(":/images/choice.png"));

}


///
/// \brief Chessboard_Window_2::generate_connect 生成连接
///
void Chessboard_Window_2::generate_connect()
{
    // 菜单栏功能
    connect(this->back, &QAction::triggered,[=](){ emit this->backSingal(); });
    connect(this->about, &QAction::triggered,[=](){ this->about_window(); });


    //点击确定按钮确定棋盘大小
    connect(ui->pushButtonOK,SIGNAL(pressed()),this,SLOT(OnPushButtonOK()));

    //点击绘制结果按钮实现结果一次性绘制
    connect(ui->pushButtonDraw,SIGNAL(pressed()),this,SLOT(OnPushButtonDraw()));

    //数字框内数字改变特殊方格位置发生改变
    connect(ui->spinBoxForX, QOverload<int>::of(&QSpinBox::valueChanged),[=](int i){
        //特殊方块横坐标
        piecesx = i;

        //特殊方块坐标确定前绘制按钮不可用，绘图标志均为-1，不进行绘图
        ui->pushButtonDraw->setEnabled(false);
        ui->btnFenbu->setEnabled(false);
        ui->btnZhuizong->setEnabled(false);
        drawflag=-1;
        fenbuflag=-1;
        zhuizongflag=-1;

        //重绘特殊方格
        update();
    });

    connect(ui->spinBoxForY, QOverload<int>::of(&QSpinBox::valueChanged),[=](int j){
        //特殊方块纵坐标
        piecesy = j;

        //特殊方块坐标确定前绘制按钮不可用，绘图标志均为-1，不进行绘图
        ui->pushButtonDraw->setEnabled(false);
        ui->btnFenbu->setEnabled(false);
        ui->btnZhuizong->setEnabled(false);
        drawflag=-1;
        fenbuflag=-1;
        zhuizongflag=-1;

        //重绘特殊方格
        update();
    });

    //点击确定按钮确定特殊方块坐标
    connect(ui->btnPosOK,&QPushButton::pressed,[=](){
        //新建算法类对象，用于之后的绘图
        algo = new algorithm(k,piecesx,piecesy);

        //执行算法
        algo->coverfunction(0,0,piecesx,piecesy,size);

        //点击该确定按钮后，绘图和动画的按钮才可以使用
        ui->pushButtonDraw->setEnabled(true);
        ui->btnFenbu->setEnabled(true);
        ui->btnZhuizong->setEnabled(true);
    });

    //点击分步绘制按钮，开始分步绘制
    connect(ui->btnFenbu,&QPushButton::pressed,[=](){
        //设置结果绘制和算法追踪的绘图事件标志为-1，不进行这两项的绘制
        drawflag=-1;
        zhuizongflag=-1;

        //设置分步绘制开始标志
        fenbuflag=1;

        //初始化定时器变量为0
        q=0;

        //设置停止和继续按钮为可用
        ui->btnStop->setEnabled(true);
        ui->btnContinue->setEnabled(true);

        //启动定时器1
        timer1->start(500);
    });

    //定时器1启动后的操作
    connect(timer1,&QTimer::timeout,[=](){
        //判断条件保证绘制完之后界面停留在最终绘制的结果
        if(q<((size*size)-1)/3)
        {
            //q用来表示每一步绘制的L型方块的序号，根据q的变化重绘实现动画
            q++;
            update();
        }
        else
        {
            //不符合判断条件定时器停止
            timer1->stop();
        }
    });

    //点击算法追踪按钮，开始按算法每一步执行结果绘制
    connect(ui->btnZhuizong,&QPushButton::pressed,[=](){
        //设置结果绘制和分步绘制的绘图事件标志为-1，不进行这两项的绘制
        drawflag=-1;
        fenbuflag=-1;

        //设置算法追踪绘制开始标志
        zhuizongflag=1;

        //初始化定时器变量为0
        o=0;

        //设置停止和继续按钮为可用
        ui->btnStop->setEnabled(true);
        ui->btnContinue->setEnabled(true);

        //启动定时器2
        timer2->start(500);
    });

    //定时器2启动后的操作
    connect(timer2,&QTimer::timeout,[=](){
        //判断条件保证绘制完之后界面停留在最终绘制的结果
        if(o<(size*size)-1)
        {
            o++;        //用来表示每一步绘制的L型方块的序号，根据的变化重绘实现动画
            update();   //重绘界面
        }
        else
        {
            //不符合判断条件定时器停止
            timer2->stop();
        }
    });

    //点击停止按钮的操作
    connect(ui->btnStop,&QPushButton::pressed,[=](){
        //点击停止按钮定时器结束，暂停动画
        timer1->stop();
        timer2->stop();
    });

    //点击继续按钮的操作
    connect(ui->btnContinue,&QPushButton::pressed,[=](){
        //点击继续按钮定时器开始，继续动画
        timer1->start();
        timer2->start();
    });
}


///
/// \brief Chessboard_Window_2::OnPushButtonOK 点击确定按钮
///
void Chessboard_Window_2::OnPushButtonOK()
{
    //初始化变量，保证每次点击确定棋盘大小的确定按钮后其他操作都是未进行状态
    piecesx=-1;
    piecesy=-1;
    drawflag = -1;
    fenbuflag=-1;
    zhuizongflag=-1;
    q=0;
    o=0;
    size=1;

    //从数字框中获取用户输入的棋盘大小
    k = ui->spinBoxSize->value();

    //计算棋盘大小
    size=int(exp2(k));

    //根据棋盘大小设定特殊方块的最大坐标范围
    ui->spinBoxForX->setMaximum(size-1);
    ui->spinBoxForY->setMaximum(size-1);

    update();   //重绘界面
}

//点击绘制结果按钮
void Chessboard_Window_2::OnPushButtonDraw()
{
    //绘图事件开始的标志
    drawflag = 1;

    //重绘界面
    update();
}

///
/// \brief Chessboard_Window_2::mousePressEvent 通过鼠标选取特殊方块
/// \param e 鼠标点击事件
///
void Chessboard_Window_2::mousePressEvent(QMouseEvent *e)
{
    //初始化变量，保证在坐标选取之前绘图和动画不开始
    drawflag=-1;
    fenbuflag=-1;
    zhuizongflag=-1;
    q=0;
    o=0;

    //绘图和动画按钮不可用
    ui->pushButtonDraw->setEnabled(false);
    ui->btnFenbu->setEnabled(false);
    ui->btnZhuizong->setEnabled(false);

    //获取鼠标相对于窗口的位置
    int x =e->x();
    int y = e->y();

    //如果鼠标位置在棋盘内，则转换为棋盘内坐标
    if(x>startx && x<startx+size*chessw && y>starty && y<starty+size*chessh)
    {
        piecesy = (x-startx)/chessw;
        piecesx = (y-starty)/chessh;
    }

    //将鼠标选取的特殊方块的坐标显示在相应的数字框内
    ui->spinBoxForX->setValue(piecesx);
    ui->spinBoxForY->setValue(piecesy);

    update();   //重绘界面
}

///
/// \brief Chessboard_Window_2::paintEvent 重写绘图事件
///
void Chessboard_Window_2::paintEvent(QPaintEvent *)
{
    //设置棋盘起点坐标
    startx=240;
    starty=30;

    //设置棋盘方格大小
    chessw=(width()-250)/size;
    chessh = chessw;

    QPainter p(this);   //创建QPainter,设置窗口为绘图设备

    //创建画笔
    QPen pen1;
    pen1.setWidth(2);
    pen1.setColor(Qt::black);
    p.setPen(pen1);

    //绘制棋盘
    for(int i=0;i<=size;i++)
    {
        //画棋盘横线
        p.drawLine(startx,starty+i*chessh,startx+size*chessw,starty+i*chessh);

        //画棋盘竖线
        p.drawLine(startx+i*chessw,starty,startx+i*chessw,starty+size*chessh);
    }

    //绘制特殊方格
    if(piecesx!=-1&& piecesy!=-1)
    {
        p.drawPixmap(startx+piecesy*chessw,starty+piecesx*chessh,chessw,chessh,QPixmap(":/images/littlestart.png") );
    }

    //判断条件用于判断是否绘制结果
    if(drawflag!=-1)
    {
        //设置随机颜色
        int num =(size*size-1)/3+1; //num为L型棋盘数目
        QColor *colors= new QColor[num];
        for(int i=0;i<num;i++)
        {
            colors[i] = QColor(rand()%256,rand()%256,rand()%256);   //每个L型棋盘的序号对应同一颜色，但每次重新绘制颜色会改变
        }

        //遍历算法对象中表示棋盘的二维数组
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                //如果是特殊方块不重新绘制
                if(i==piecesx&&j==piecesy)
                {
                    continue;
                }

                // 填充颜色
                int temp = algo->board[i][j];
                p.setBrush(colors[temp]);
                p.drawRect(startx+j*chessw,starty+i*chessh,chessw,chessh);

                //填充数字
                p.drawText(QPoint(startx+j*chessw+chessw/2, starty+i*chessh+chessh/2), QString::number(temp));
            }
        }
        delete[] colors;    //释放动态申请的空间
        drawflag=-1;        //每次绘制完之后绘制标志改为-1再次点击才会重新绘制
    }

    //判断条件用于判断是否分步绘制
    if(fenbuflag!=-1)
    {
        //设置随机颜色
        int num = (size*size-1)/3+1;
        QColor *colors2= new QColor[num];
        for(int i=0;i<num;i++)
        {
            colors2[i] = QColor((50*i)%256,(i*i+200)%180,(i*(i+30)*3+100)%200); //每个L型棋盘的序号对应同一颜色，但每次重新绘制颜色不会改变
        }

        //遍历算法对象中表示棋盘的二维数组
        for(int i=0;i<size;i++)
        {
            for(int j=0;j<size;j++)
            {
                int temp2 = algo->board[i][j];  //获取方格是哪个L型棋盘

                //如果是序号小于q的L型棋盘就绘制
                if(temp2!=0&&temp2<=q)
                {
                    //根据q的不同每次重绘结果不同，实现动画效果
                    p.setBrush(colors2[temp2]);
                    p.drawRect(startx+j*chessw,starty+i*chessh,chessw,chessh);

                    //填充数字
                    p.drawText(QPoint(startx+j*chessw+chessw/2, starty+i*chessh+chessh/2), QString::number(temp2));
                }
            }
        }
        delete[] colors2;   //释放动态申请的空间
    }

    //判断条件用于判断是否追踪算法绘制
    if(zhuizongflag!=-1)
    {
        //设置随机颜色
        int num =(size*size-1)/3+1;
        QColor *colors3= new QColor[num];
        for(int i=0;i<num;i++)
        {
            colors3[i] = QColor((50*i)%256,(i*i+200)%180,(i*(i+30)*3+100)%200); //每个L型棋盘的序号对应同一颜色，但每次重新绘制颜色不会改变
        }

        //遍历算法对象中表示棋盘的二维数组
        for(int i=0;i<size;i++)
        {
            for(int j=0;j<size;j++)
            {
                int temp3 = algo->order[i][j];  //获取算法执行过程中棋盘方格被标记的顺序
                int ctemp=algo->board[i][j];    //获取该方格对应的L型方格的序号

                //判断按算法执行顺序绘制方格
                if(temp3>0&&temp3<=o)
                {
                    //根据o的不同每次重绘结果不同，实现动画效果
                    p.setBrush(colors3[ctemp]);
                    p.drawRect(startx+j*chessw,starty+i*chessh,chessw,chessh);

                    //填充数字
                    p.drawText(QPoint(startx+j*chessw+chessw/2, starty+i*chessh+chessh/2), QString::number(ctemp));
                }
            }
        }
        delete[] colors3;   //释放动态申请的空间
    }
}


///
/// \brief Chessboard_Window_2::about_window 关于窗口
///
void Chessboard_Window_2::about_window()
{
    QString text= "Title: 棋盘覆盖\nDeveloped by 许亚茹.\nTime: 2020.02.23";
    QMessageBox::about(this,"关于",text);
}


