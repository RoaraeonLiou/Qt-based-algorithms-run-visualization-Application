#ifndef HUFFMAN_WINDOW_H
#define HUFFMAN_WINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTreeWidget>
#include "huffman.h"
#include <QClipboard>

namespace Ui {
class Huffman_Window;
}


///
/// \brief The Huffman_Window class 哈夫曼编码窗口，负责展示哈夫曼编码题目
///
class Huffman_Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Huffman_Window(QWidget *parent = nullptr);
    ~Huffman_Window();

signals:
    void backSingal(); //返回主页信号

private:
    Ui::Huffman_Window *ui;

    void basic_set();           // 窗口基本设置
    void generate_connect();    // 生成各组件的功能连接
    void begin();               // 开始构造哈夫曼树
    bool mode_1();              // 利用文件初始化哈夫曼对象
    bool mode_2();              // 利用字符串初始化哈夫曼对象
    void make_code();           // 自动构造哈夫曼树
    void make_code_by_step();   // 单步构造哈夫曼树
    void refresh_trees();       // 刷新展示的树形结构
    void init_table();          // 初始化编码表格
    void refresh_table();       // 刷新编码表格
    void output();              // 输出编码/解码结果
    void output_to_file();      // 将编码/解码结果保存到文件
    void copy();                // 将编码/解码结果复制到剪切板
    void change_table_order();  // 改变编码表格排序方式
    void forbid_set();          // 禁用窗口部分控件
    void reset();               // 重置哈夫曼对象及窗口
    QString choose_file();      // 选择文件窗口
    void about_window();        // 关于窗口
    QTreeWidgetItem * genetate_tree(Node *p_node,int flag=0);//构造树状结构

    bool init_flag=false;       // 记录是否已经初始化
    QString output_content;     // 记录编码/解码结果
    Huffman *huff;              // 哈夫曼对象指针
    QTimer *timer;              // 定时器指针
    QClipboard *clipboard;      // 剪切板指针
    QAction *about;             // 关于
    QAction *back;              // 返回上一级
};

#endif // HUFFMAN_WINDOW_H
