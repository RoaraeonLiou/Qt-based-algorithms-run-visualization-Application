#ifndef HUFFMAN_H
#define HUFFMAN_H

#include<QChar>
#include<QString>
#include<QVector>
#include<QMap>
#include "node.h"

///
/// \brief the Code struct 用于记录编码和频率
///
typedef struct Code
{
    int f;          // 频率
    QString code;   // 编码
}Code;


///
/// \brief The Huffman class 实现哈夫曼编码的构造，编码，解码等功能
///
class Huffman
{
public:
    Huffman();
    ~Huffman();
    void init_with_string(const QString &code_str, const QString &input_str);   // 利用字符串初始化
    void init_with_file(const QString &codePath,const QString &inputPath);      // 利用文件初始化
    bool build_by_step();           // 单步构造哈夫曼树
    void encode(QString &res);      // 编码
    void decode(QString &res);      // 解码
    void reset();                   // 重置Huffman对象

    bool finish_flag=false;         // 是否构造结束的标志
    int init_mode=0;                // 记录初始化模式，0:错误值，1:用文件初始化，2:用字符串初始化
    int remain_node=0;              // 记录剩余结点数
    QVector<Node *> node_list;      // 存储所有根节点，用于构造哈夫曼树
    QMap<QChar,Code> code_table;    // 记录编码结果

    bool code_punctuation=true;     // 是否编码标点
    bool code_space=true;           // 是否编码空格
    bool match_case=true;           // 是否区分大小写


private:
    QChar scan(QChar c);                        // 按照设置筛选字符
    void set_code(Node *p_node,QString &code);  // 设置编码
    void generate_nodes();                      // 生成结点
    void genetare_anti_code();                  // 生成解码对照表
    QString code_path;                          // 记录生成编码文件路径
    QString input_path;                         // 记录被编码文件路径

    QString source;                 // 记录被编码字符串
    int max_code_length;            // 记录反码最大长度
    int min_code_length;            // 记录反码最短长度
    QMap<QString,QChar> anti_code;  // 记录反码信息
};


#endif // HUFFMAN_H
