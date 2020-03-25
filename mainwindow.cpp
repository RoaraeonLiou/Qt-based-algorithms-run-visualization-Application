#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imgbutton.h"
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 创建题目窗口对象
    hanoi_window = new Hanoi_Window;
    huffman_window = new Huffman_Window;
    chessboard_window = new Chessboard_Window;
    chessboard_window_2 = new Chessboard_Window_2;
    sort_window = new Sort_Window;

    // 设置窗口基本属性以及按钮样式
    this->setWindowIcon(QPixmap(":/images/visiable.png"));
    this->setWindowTitle("算法过程可视化系统");
    this->ui->hanoi->set_pic(":/images/hanoi_2.png", ":/images/hanoi_1.png");
    this->ui->huffman->set_pic(":/images/huff_1.png", ":/images/huff_2.png");
    this->ui->chessboard->set_pic(":/images/chessboard_3.png", ":/images/chessboard_4.png");
    this->ui->chessboard_2->set_pic(":/images/chessboard_2.png", ":/images/chessboard_1.png");
    this->ui->bubble_sort->set_pic(":/images/bubble_2.png", ":/images/bubble_2.png");

    // 生成连接
    this->generate_connect();

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::generate_connect()
{
    // 链接按钮与窗口显示函数
    connect(this->ui->hanoi, &QPushButton::clicked, [=](){ this->hide(); this->hanoi_window->show(); });
    connect(this->ui->huffman, &QPushButton::clicked, [=](){ this->hide(); this->huffman_window->show(); });
    connect(this->ui->chessboard, &QPushButton::clicked, [=](){ this->hide(); this->chessboard_window->show(); });
    connect(this->ui->chessboard_2, &QPushButton::clicked, [=](){ this->hide(); this->chessboard_window_2->show(); });
    connect(this->ui->bubble_sort, &QPushButton::clicked, [=](){ this->hide(); this->sort_window->show(); });

    // 链接窗口返回主页的信号与动作
    connect(hanoi_window, &Hanoi_Window::backSingal,[=](){ hanoi_window->hide(); this->show(); });
    connect(huffman_window, &Huffman_Window::backSingal,[=](){ huffman_window->hide(); this->show(); });
    connect(chessboard_window, &Chessboard_Window::backSingal,[=](){ chessboard_window->hide(); this->show(); });
    connect(chessboard_window_2, &Chessboard_Window_2::backSingal,[=](){ chessboard_window_2->hide(); this->show(); });
    connect(sort_window, &Sort_Window::backSingal,[=](){ sort_window->hide(); this->show(); });

}
