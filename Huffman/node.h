#ifndef NODE_H
#define NODE_H

#include <QChar>


///
/// \brief The Node class 用于作为哈夫曼树的结点
///
class Node
{
    friend class Huffman;
    friend bool cmp_power(Node *a, Node *b);
public:
    Node(QChar c,int p);
    ~Node();
    void set_leftChild(Node *p_node);   // 设置左孩子结点
    void set_rightChild(Node *p_node);  // 设置右孩子结点
    void set_parent(Node *p_node);      // 设置父结点
    QChar get_ch();                     // 获取存储的字符
    int get_power();                    // 获取权值

    Node *leftChild;    // 指向左孩子的指针
    Node *rightChild;   // 指向右孩子的指针
    Node *parent;       // 指向父结点的指针

private:
    QChar ch;           // 记录存储的字符
    int power;          // 记录字符的权值

    void recursive_delete(Node*node);   // 递归释放一棵树的结点
};


#endif // NODE_H
