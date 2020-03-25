#include "huffman_window.h"
#include "ui_huffman_window.h"
#include "huffman.h"
#include <QDebug>
#include <QMap>
#include <QTimer>
#include <QFileDialog>
#include <QStandardPaths>
#include <QFile>
#include <QClipboard>
#include <QMessageBox>
#include <QStyleFactory>


Huffman_Window::Huffman_Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Huffman_Window)
{
    ui->setupUi(this);

    //创建对象
    huff = new Huffman;
    timer = new QTimer(this);
    clipboard = QApplication::clipboard();
    about = new QAction("关于",this);
    back = new QAction("返回",this);

    //设置信息
    this->basic_set();
    this->generate_connect();
}


Huffman_Window::~Huffman_Window()
{
    delete huff;
    huff = nullptr;
    delete ui;
}


///
/// \brief Huffman_Window::basic_set 窗口基本设置
///
void Huffman_Window::basic_set()
{
    // 窗口设置
    this->resize(1200,800);
    this->setWindowIcon(QPixmap(":/images/huff_1.png"));
    this->setWindowTitle("哈夫曼编码");
    this->ui->menubar->addAction(this->back);
    this->ui->menubar->addAction(this->about);

    // 树结构窗口设置
    this->ui->treeWidget->setHeaderLabels(QStringList() << "标识" << "字符" << "频率"<<"编码");
    this->ui->treeWidget->setColumnWidth(0, 300);
    this->ui->treeWidget->setColumnWidth(1, 50);
    this->ui->treeWidget->setColumnWidth(2, 100);
    this->ui->treeWidget->setColumnWidth(3, 100);

    // 树结构设置线
    this->ui->treeWidget->setStyle(QStyleFactory::create("Windows"));

    // 编码详情表窗口设置
    this->ui->tableWidget->setColumnCount(4);
    this->ui->tableWidget->setRowCount(0);
    this->ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"字符"<<"频率"<<"ASCII编码"<<"编码");

    // 单选框设置
    this->ui->choose_encode->setChecked(true);
    this->ui->punct_true->setChecked(true);
    this->ui->space_true->setChecked(true);
    this->ui->case_true->setChecked(true);

    // 信息展示框设置
    this->ui->output_window->setReadOnly(true);
    this->ui->input_file->setReadOnly(true);
    this->ui->code_file->setReadOnly(true);

    // 按钮图标设置
    this->ui->make->setIcon(QPixmap(":/images/auto.png"));
    this->ui->begin_btn->setIcon(QPixmap(":/images/begin.png"));
    this->ui->make_by_step->setIcon(QPixmap(":/images/next.png"));
    this->ui->reset_btn->setIcon(QPixmap(":/images/reset.png"));
}


///
/// \brief Huffman_Window::generate_connect 生成各组件的功能连接
///
void Huffman_Window::generate_connect()
{
    // 菜单栏功能链接
    connect(this->back, &QAction::triggered,[=](){ emit this->backSingal(); });
    connect(this->about, &QAction::triggered,[=](){ this->about_window(); });

    // 按钮功能链接
    connect(this->ui->begin_btn, &QPushButton::clicked,[=](){ this->begin(); });
    connect(this->ui->make, &QPushButton::clicked, [=](){ timer->start(300); });
    connect(this->ui->make_by_step, &QPushButton::clicked, [=](){ this->make_code_by_step(); });
    connect(this->ui->reset_btn, &QPushButton::clicked,[=](){ this->reset(); });
    connect(this->ui->work_btn, &QPushButton::clicked, [=](){ this->output(); });
    connect(this->ui->code_choose, &QPushButton::clicked, [=](){ QString path=this->choose_file(); this->ui->code_file->setText(path); this->ui->input_file->setText(path); });
    connect(this->ui->input_choose, &QPushButton::clicked, [=](){ QString path=this->choose_file(); this->ui->input_file->setText(path); });
    connect(this->ui->to_file_btn, &QPushButton::clicked, [=](){ this->output_to_file(); });
    connect(this->ui->copy_btn, &QPushButton::clicked,[=]{ this->copy(); });
    connect(this->ui->change_sort_btn, &QPushButton::clicked,[=](){ this->change_table_order(); });


    // 单选框功能链接
    connect(this->ui->punct_true,&QRadioButton::clicked,[=](){ this->huff->code_punctuation=true; });
    connect(this->ui->punct_false,&QRadioButton::clicked,[=](){ this->huff->code_punctuation=false; });
    connect(this->ui->space_true, &QRadioButton::clicked,[=](){ this->huff->code_space=true; });
    connect(this->ui->space_false, &QRadioButton::clicked,[=](){ this->huff->code_space=false; });
    connect(this->ui->case_true,&QRadioButton::clicked,[=](){ this->huff->match_case=true; });
    connect(this->ui->case_false,&QRadioButton::clicked,[=](){ this->huff->match_case=false; });
    connect(this->ui->choose_encode,&QRadioButton::clicked,[=](){this->ui->work_btn->setText("编码");});
    connect(this->ui->choose_decode,&QRadioButton::clicked,[=](){this->ui->work_btn->setText("解码");});

    // 定时器功能链接
    connect(this->timer, &QTimer::timeout,[=](){ this->make_code(); });
}



///
/// \brief Huffman_Window::begin 开始构造哈夫曼树
///
void Huffman_Window::begin()
{
    // 通过tabWidget的index来确定初始化方式
    int index = this->ui->tabWidget->currentIndex();
    if(index==0)
        this->init_flag = this->mode_1();
    else
        this->init_flag = this->mode_2();
    if(this->init_flag)
        this->ui->begin_btn->setDisabled(true);
}


///
/// \brief Huffman_Window::mode_1 利用文件初始化哈夫曼对象
/// \return 文件路径为空返回false，否则返回true
///
bool Huffman_Window::mode_1()
{
    // 获取文件路径
    QString code_path = this->ui->code_file->text();
    QString input_path = this->ui->input_file->text();

    // 判断文件路径是否为空
    if(code_path==""||input_path=="")
    {
        QMessageBox::information(this,"提示信息","请选择有效的路径");
        return false;
    }

    // 利用文件进行初始化
    this->huff->init_with_file(code_path,input_path);

    // 刷新窗口元素
    this->refresh_trees();
    this->init_table();
    this->forbid_set();

    return true;
}


///
/// \brief Huffman_Window::mode_2 利用字符串初始化哈夫曼对象
/// \return 字符串为空返回false，否则返回true
///
bool Huffman_Window::mode_2()
{
    // 获取输入框内容
    QString code_str = this->ui->code_str->text();
    QString input_str = this->ui->input_str->text();

    // 判断输入内容是否为空
    if(code_str=="")
    {
        QMessageBox::information(this,"提示信息","请输入生成编码字符串");
        return false;
    }
    if(input_str=="")
    {
        QMessageBox::information(this,"提示信息","被编码字符串");
        return false;
    }

    // 利用字符串方式初始化
    this->huff->init_with_string(code_str, input_str);

    // 刷新窗口元素
    this->refresh_trees();
    this->init_table();
    this->forbid_set();

    return true;
}


///
/// \brief Huffman_Window::make_code 自动构造哈夫曼树
///
void Huffman_Window::make_code()
{
    // 判断是否已经初始化
    if(this->init_flag==false)
    {
        this->timer->stop();
        QMessageBox::information(this,"提示信息","请先点击开始");
        return;
    }

    // 直接构造哈夫曼树，通过计时器事件调用
    if( this->huff->finish_flag )
    {
        this->timer->stop();
        QMessageBox::information(this,"提示信息","编码已经构造完成");
        return;
    }

    // 设置布尔型变量控制定时器的结束
    bool flag = this->huff->build_by_step();
    this->refresh_trees();
    if(flag)
        this->timer->stop();
}


///
/// \brief Huffman_Window::make_code_by_setp 单步构造哈夫曼树
///
void Huffman_Window::make_code_by_step()
{
    // 判断是否已经初始化
    if(this->init_flag==false)
    {
        this->timer->stop();
        QMessageBox::information(this,"提示信息","请先点击开始");
        return;
    }

    // 判断是否已经构造完成
    if( this->huff->finish_flag )
    {
        QMessageBox::information(this,"提示信息","编码已经构造完成");
        return;
    }

    // 单步构造哈夫曼树
    this->huff->build_by_step();
    this->refresh_trees();
}


///
/// \brief Huffman_Window::refresh_trees 刷新展示的树形结构
///
void Huffman_Window::refresh_trees()
{
    // 清空窗口
    this->ui->treeWidget->clear();

    // 遍历剩余结点列表
    for(int i=0;i<this->huff->remain_node;i++){
        // 对当前结点进行遍历，生成树
        QTreeWidgetItem * item = genetate_tree(this->huff->node_list[i]);

        // 设置背景颜色
        item->setBackground(2,Qt::cyan);

        //加入到treeWidget中
        this->ui->treeWidget->addTopLevelItem(item);
    }

    // 展开所有的树形结构
    this->ui->treeWidget->expandAll();
    if(this->huff->remain_node==1)
        this->refresh_table();
}


///
/// \brief Huffman_Window::genetate_tree 构造树状结构
/// \param p_node 结点
/// \param flag 结点类型，0:根节点 1:左孩子 2:右孩子
/// \return 返回生成的QTreeWidgetItem对象指针
///
QTreeWidgetItem * Huffman_Window::genetate_tree(Node *p_node, int flag)
{
    // 遍历Node结点,生成树状结构QTreeWidgetItem
    QStringList strList;
    QString str="";

    // 根据flag设置结点信息
    switch (flag)
    {
    case 0:
        str="root";
        break;
    case 1:
        str="0";
        break;
    case 2:
        str="1";
        break;
    default:
        break;
    }

    // 加入结点存储的字符，权值的信息
    strList << str << p_node->get_ch() << QString::number(p_node->get_power());

    // 创建QTreeWidgetItem对象
    QTreeWidgetItem *item = new QTreeWidgetItem(strList);

    // 判断当前结点是否位叶子结点
    if(p_node->get_ch()==QChar::Null)
    {
        // 不是叶子节点，分别对左右节点进行遍历
        QTreeWidgetItem *left = genetate_tree(p_node->leftChild, 1);
        item->addChild(left);
        QTreeWidgetItem *right = genetate_tree(p_node->rightChild, 2);
        item->addChild(right);
    }
    else
    {
        // 是叶子节点，设置背景
        item->setBackground(1,Qt::green);

        // 判断是否构造完成，如果构造完成，加入编码信息
        if(this->huff->remain_node==1)
            item->setText(3,this->huff->code_table[p_node->get_ch()].code);
    }
    return item;
}


///
/// \brief Huffman_Window::init_table 初始化编码表格
///
void Huffman_Window::init_table()
{
    // 清楚表格信息
    this->ui->tableWidget->clearContents();
    this->ui->tableWidget->setRowCount(this->huff->code_table.size());
    int i=0;

    // 遍历code_table将结点信息录入表格中
    for(QMap<QChar,Code>::iterator iter=this->huff->code_table.begin();iter!=this->huff->code_table.end();iter++)
    {
        // 字符
        this->ui->tableWidget->setItem(i,0, new QTableWidgetItem(iter.key()));
        // 频率
        this->ui->tableWidget->setItem(i,1, new QTableWidgetItem(QString::number(iter->f)));
        // ASCII编码
        int ascii = iter.key().toLatin1();
        this->ui->tableWidget->setItem(i,2, new QTableWidgetItem(QString::number(ascii)));
        i++;
    }
}


///
/// \brief Huffman_Window::refresh_table 刷新编码表格
///
void Huffman_Window::refresh_table()
{
    // 遍历code_table，加入每个字符的编码信息
    int i=0;
    for(QMap<QChar,Code>::iterator iter=this->huff->code_table.begin();iter!=this->huff->code_table.end();iter++,i++)
        this->ui->tableWidget->setItem(i,3, new QTableWidgetItem(iter->code));
}


///
/// \brief Huffman_Window::reset 重置哈夫曼对象及窗口
///
void Huffman_Window::reset()
{
    // 重置Huffman对象
    this->huff->reset();
    this->init_flag = false;
    // 激活单选框
    this->ui->choose_encode->setEnabled(true);
    this->ui->choose_decode->setEnabled(true);
    this->ui->punct_true->setEnabled(true);
    this->ui->space_true->setEnabled(true);
    this->ui->case_true->setEnabled(true);
    this->ui->punct_false->setEnabled(true);
    this->ui->space_false->setEnabled(true);
    this->ui->case_false->setEnabled(true);
    // 设置单选框默认选项
    this->ui->choose_encode->setChecked(true);
    this->ui->punct_true->setChecked(true);
    this->ui->space_true->setChecked(true);
    this->ui->case_true->setChecked(true);
    // 清空窗口内容
    this->ui->treeWidget->clear();
    this->ui->tableWidget->clearContents();
    this->ui->output_window->clear();
    this->output_content = "";
    // 激活tab窗口
    this->ui->tabWidget->setEnabled(true);
    // 激活开始按钮
    this->ui->begin_btn->setEnabled(true);
}


///
/// \brief Huffman_Window::output 输出编码/解码结果
///
void Huffman_Window::output()
{
    // 输出编码结果
    if(this->huff->finish_flag==false)
    {
        QMessageBox::information(this,"提示信息","编码尚未构造完成!");
        return ;
    }

    this->output_content="";

    // 输出编码结果
    if(this->ui->choose_encode->isChecked())
        this->huff->encode(this->output_content);

    // 输出解码结果
    if(this->ui->choose_decode->isChecked())
        this->huff->decode(this->output_content);

    // 展示信息
    this->ui->output_window->setPlainText(this->output_content);
}


///
/// \brief Huffman_Window::output_to_file 将编码/解码结果保存到文件
///
void Huffman_Window::output_to_file()
{
    // 获取文件路径并判断是否合法
    QString path = this->choose_file();
    if(path=="")
        return;

    // 打开文件并以追加形式写入
    QFile file(path);
    file.open(QIODevice::Append);
    file.write(this->output_content.toLatin1());
    file.close();

    // 提示保存信息
    QMessageBox::information(this,"提示信息","已保存到文件:"+path);
}


///
/// \brief Huffman_Window::choose_file 选择文件窗口
/// \return 返回选择文件的路径
///
QString Huffman_Window::choose_file()
{
    // 设置默认位置
    QString location = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    // 获取txt文件路径并返回
    QString path = QFileDialog::getOpenFileName(this, "打开文件",location,"*.txt");
    return path;
}


///
/// \brief Huffman_Window::copy 将编码/解码结果复制到剪切板
///
void Huffman_Window::copy()
{
    // 将输出内容复制到剪切板
    this->clipboard->setText(this->output_content);
    QMessageBox::information(this,"提示信息","编码结果已复制到剪切板");
}


///
/// \brief Huffman_Window::change_table_order 改变编码表格排序方式
///
void Huffman_Window::change_table_order()
{
    // 判断是否构造完成
    if(this->huff->finish_flag==false)
    {
        QMessageBox::information(this,"提示信息","编码尚未构造完成!");
        return;
    }

    // 改变信息表格中的排序方式
    if(this->ui->sort_label->text()=="字符编码")
    {
        this->ui->sort_label->setText("哈夫曼编码");
        this->ui->tableWidget->sortByColumn(3,Qt::SortOrder());
    }
    else
    {
        this->ui->sort_label->setText("字符编码");
        this->ui->tableWidget->sortByColumn(0,Qt::SortOrder());
    }
}


///
/// \brief Huffman_Window::forbid_set 禁用窗口部分控件
///
void Huffman_Window::forbid_set()
{
    //禁用单选框及tab窗口
    this->ui->tabWidget->setEnabled(false);
    this->ui->choose_encode->setDisabled(true);
    this->ui->choose_decode->setDisabled(true);
    this->ui->punct_true->setDisabled(true);
    this->ui->space_true->setDisabled(true);
    this->ui->case_true->setDisabled(true);
    this->ui->punct_false->setDisabled(true);
    this->ui->space_false->setDisabled(true);
    this->ui->case_false->setDisabled(true);
}


///
/// \brief Huffman_Window::about_window 关于窗口
///
void Huffman_Window::about_window()
{
    QString text= "Title: 哈夫曼编码\nDeveloped by 刘啸宇.\nTime: 2020.02.21";
    QMessageBox::about(this,"关于",text);
}
