#include "huffman.h"
#include <QChar>
#include <QMap>
#include <qalgorithms.h>
#include <QDebug>
#include <QFile>


///
/// \brief cmp_power 权值比较函数
/// \param a 结点a
/// \param b 结点b
/// \return 返回结点a的权值是否小于b的权值
///
bool cmp_power(Node *a, Node *b)
{
    return a->power < b->power;
}


Huffman::Huffman():
    code_path(""),input_path("")
{
    //empty
}


Huffman::~Huffman(){
    // 释放所有处于node_list中的结点
    if(this->remain_node)
        for(int i=0;i<this->remain_node;i++)
        {
            delete this->node_list[i];
            this->node_list[i] = nullptr;
        }
}


///
/// \brief Huffman::init_with_file 利用文件初始化
/// \param codePath 生成编码文件路径
/// \param inputPath 被编码文件路径
///
void Huffman::init_with_file(const QString &codePath, const QString &inputPath)
{
    // 设置对象启动方式及相关属性
    this->code_path = codePath;
    this->input_path = inputPath;
    this->init_mode = 1;

    // 从文件中统计字符频率
    QChar c;
    QFile file(this->code_path);
    file.open(QIODevice::ReadOnly);
    QByteArray line;
    while( !file.atEnd()  )
    {
        line = file.readLine();
        for(int i=0;i<line.length();i++)
        {
            // 利用scan函数筛选字符
            c = scan(line.at(i));
            if(c!=QChar::Null)
            {
                if(code_table.count(c))
                    code_table[c].f++;
                else
                    code_table[c]={1,""};
            }
        }
    }
    file.close();

    // 根据统计结果生成结点
    for(QMap<QChar,Code>::iterator iter=code_table.begin();iter!=code_table.end();iter++)
    {
        Node * p_node = new Node(iter.key(),iter->f);
        node_list.push_back(p_node);
    }
    this->remain_node = node_list.length();

    // 给node_list排序
    std::sort(node_list.begin(),node_list.end(),cmp_power);
}


///
/// \brief Huffman::init_with_string 利用字符串初始化
/// \param code_str  生成编码字符串
/// \param input_str 被编码字符串
///
void Huffman::init_with_string(const QString &code_str, const QString &input_str)
{
    // 设置对象启动方式
    this->init_mode = 2;

    // 统计字符出现频率
    QChar c;
    for(int i=0;i<code_str.length();i++){
        // 利用scan函数筛选字符
        c = scan(code_str.at(i));
        if(c!=QChar::Null)
        {
            if(code_table.count(c))
                code_table[c].f++;
            else
                code_table[c]={1,""};
        }
    }

    // 将被编码字符串按照编码设置存储到source中
    this->source = input_str;

    // 根据统计结果生成结点，加入到node_list中
    this->generate_nodes();
}


///
/// \brief Huffman::generate_nodes 生成结点
///
void Huffman::generate_nodes()
{
    // 根据code_table中数据生成结点，加入到node_list中
    for(QMap<QChar,Code>::iterator iter=code_table.begin();iter!=code_table.end();iter++)
    {
        Node * p_node = new Node(iter.key(),iter->f);
        node_list.push_back(p_node);
    }
    this->remain_node = node_list.length();

    // 给node_list排序
    std::sort(node_list.begin(),node_list.end(),cmp_power);
}


///
/// \brief Huffman::code_by_setp 单步构造哈夫曼树
/// \return 全部构造完成返回true,否则返回false
///
bool Huffman::build_by_step()
{
    // 判断是否已经构造完成或者没有结点
    if(this->finish_flag==1 || this->code_table.size()==0)
        return true;

    // 选择权值最小的两个树，构建新的树
    Node * p_node = new Node(QChar::Null,0);
    p_node->set_leftChild(node_list[0]);
    p_node->set_rightChild(node_list[1]);

    // 从node_list中移除权值最小的两个树,并将构建的新树加入到node_list中
    node_list.erase(node_list.begin(),node_list.begin()+2);
    node_list.push_back(p_node);
    this->remain_node--;

    if(this->remain_node==1)
    {
        // 如果构建完成,对哈夫曼树设置编码,并将完成标志finish_flag设置为true
        QString code = "";
        this->set_code(node_list[0],code);
        this->finish_flag=1;
        this->genetare_anti_code();
        return true;
    }

    // 将node_list排序,为下一步构造做准备
    std::sort(node_list.begin(),node_list.end(),cmp_power);
    return false;
}


///
/// \brief Huffman::encode 编码
/// \param res 接收编码结果的字符串
///
void Huffman::encode(QString &res)
{
    // 判断错误情况
    if(this->code_table.size()==0)
        return;
    if(this->finish_flag==false)
        return;

    // 根据初始化方式输出编码结果
    QChar c;
    if(this->init_mode==1)
    {
        QFile file(this->input_path);
        file.open(QIODevice::ReadOnly);
        QByteArray line;
        while( !file.atEnd()  )
        {
            // 单行读取文件
            line = file.readLine();

            // 对一行的字符进行编码
            for(int i=0;i<line.length();i++)
            {
                // 利用scan函数筛选字符
                c = scan(line.at(i));
                if(c!=QChar::Null)
                {
                    // 判断是否存在编码，若存在，进行编码，否则结束，返回error
                    if( this->code_table.count(c) )
                        res.append(this->code_table[c].code);
                    else
                    {
                        res.clear();
                        res = "error";
                        file.close();
                        return;
                    }
                }
            }
        }
        file.close();
        return;
    }
    else if(this->init_mode == 2)
    {
        for(int i=0;i<this->source.length();i++)
        {
            // 利用scan函数筛选字符
            c = scan(this->source.at(i));
            if(c!=QChar::Null)
            {
                // 判断是否存在编码，若存在，进行编码，否则结束，返回error
                if(this->code_table.count(c))
                    res.append(this->code_table[c].code);
                else
                {
                    res.clear();
                    res = "error";
                    return;
                }
            }
        }
        return;
    }
}


///
/// \brief Huffman::decode 解码
/// \param res 接收解码结果的字符串
///
void Huffman::decode(QString &res)
{
    // 判断错误情况
    if(this->code_table.size()==0)
        return;
    if(this->finish_flag==false)
        return;

    QString code="";
    QString all_str;

    // 根据初始化方式读取待解码字符串
    if(this->init_mode==1)
    {
        QFile file(this->input_path);
        file.open(QIODevice::ReadOnly);
        QByteArray all = file.readAll();
        all_str = QByteArray(all);
        file.close();
    }
    else if(this->init_mode == 2)
    {
        all_str = this->source;
    }
    else
    {
        return;
    }

    // 遍历待解码字符串，进行解码
    for(int i=0;i<all_str.length();i++)
    {
        code.append(all_str.at(i));

        // 如果当前编码字符串大于最长的编码长度，返回error
        if(code.length()>this->max_code_length)
        {
            code.clear();
            res.clear();
            res = "error";
            return;
        }

        // 如果当前编码字符串不小于最短的编码长度，尝试进行解码
        if(code.length()>=this->min_code_length)
            if(this->anti_code.count(code))
            {
                res.append(this->anti_code[code]);
                code.clear();
            }
    }

}


///
/// \brief Huffman::reset 重置Huffman对象
///
void Huffman::reset()
{
    // 清空所有记录
    if(this->remain_node)
        for(int i=0;i<this->remain_node;i++)
        {
            delete this->node_list[i];
            this->node_list[i] = nullptr;
        }
    this->node_list.clear();
    this->code_table.clear();
    this->anti_code.clear();

    // 属性还原默认值
    this->remain_node = 0;
    this->finish_flag = false;
    this->code_path = "";
    this->input_path = "";
    this->source="";
    this->init_mode=0;

    // 设置还原默认值
    this->code_space = true;
    this->code_punctuation = true;
    this->match_case = true;
}


///
/// \brief Huffman::set_code 设置编码
/// \param p_node 哈夫曼树根节点
/// \param code 记录编码的字符串
///
void Huffman::set_code(Node *p_node, QString &code)
{
    // 由生成的哈夫曼树为出现的字符设置编码,保存到code_table中

    if(p_node==nullptr)
        return ;

    if( code_table.size()==1 )
    {
        // 如果统计结果只出现一种字符,直接设置编码为0
        code_table[p_node->ch].code = "0";
        return ;
    }

    // 分别从左右子树寻找叶子结点,设置编码
    if(p_node->leftChild)
    {
        code.append('0');
        set_code(p_node->leftChild, code);
    }
    if(p_node->rightChild)
    {
        code.append('1');
        set_code(p_node->rightChild, code);
    }

    // 当前结点是叶子结点,设置编码
    if( p_node->leftChild==nullptr && p_node->rightChild==nullptr )
        code_table[p_node->ch].code = code;

    // 重置编码,保证编码正确
    code.remove(code.length()-1,1);
}


///
/// \brief Huffman::scan 按照设置筛选字符
/// \param c 被筛选字符
/// \return 返回筛选后的字符
///
QChar Huffman::scan(QChar c)
{
    // 按照编码设置,对字符进行筛选,并返回符合编码结果的字符
    if(c.isPunct())
    {
        // 标点
        if(this->code_punctuation)
            return c;
        else
            return QChar::Null;
    }

    if(c==' ')
    {
        // 空格
        if(this->code_space)
            return c;
        else
            return QChar::Null;
    }

    if(c.isLetter())
    {
        // 大小写
        if(this->match_case)
            return c;
        else
            return c.toLower();
    }

    if(c.isDigit())
        return c;

    // 其余情况,返回空字符
    return QChar::Null;
}


///
/// \brief Huffman::genetare_anti_code 生成解码对照表
///
void Huffman::genetare_anti_code()
{
    // 判断是否编码结束
    if(this->finish_flag==false)
        return;

    this->max_code_length = 0;
    this->min_code_length = 999999;

    //遍历编码信息表，生成相应反码，并统计出最短编码长度和最长编码长度
    for(QMap<QChar,Code>::iterator iter=this->code_table.begin();iter!=this->code_table.end();iter++)
    {
        this->max_code_length = this->max_code_length > iter.value().code.length()?this->max_code_length:iter.value().code.length();
        this->min_code_length = this->min_code_length < iter.value().code.length()?this->min_code_length:iter.value().code.length();
        this->anti_code[iter.value().code] = iter.key();
    }
    return;
}

