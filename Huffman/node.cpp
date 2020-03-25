#include "node.h"


Node::Node(QChar c,int p):
    leftChild(nullptr),rightChild(nullptr),parent(nullptr),
    ch(c),power(p)
{
    // empty
}


Node::~Node()
{
    // 递归释放左右孩子结点
    this->recursive_delete(this->leftChild);
    this->recursive_delete(this->rightChild);
}


///
/// \brief Node::recursive_delete 递归释放一棵树的结点
/// \param node 根节点指针
///
void Node::recursive_delete(Node *node)
{
    if(node==nullptr)
        return ;
    recursive_delete(node->leftChild);
    recursive_delete(node->rightChild);
    delete node;
    node = nullptr;
}


///
/// \brief Node::set_leftChild 设置左孩子结点
/// \param p_node 结点指针
///
void Node::set_leftChild(Node *p_node)
{
    // 设置左孩子结点
    this->leftChild = p_node;
    this->power += p_node->power;
    p_node->set_parent(this);
}


///
/// \brief Node::set_rightChild 设置右孩子结点
/// \param p_node 右孩子结点指针
///
void Node::set_rightChild(Node *p_node)
{
    // 设置右孩子结点
    this->rightChild = p_node;
    this->power += p_node->power;
    p_node->set_parent(this);
}


///
/// \brief Node::set_parent 设置父结点
/// \param p_node 父结点指针
///
void Node::set_parent(Node *p_node)
{
    // 设置父结点
    this->parent = p_node;
}


///
/// \brief Node::get_ch 获取存储的字符
/// \return 返回节结点中存储的字符
///
QChar Node::get_ch()
{
    // 获取结点存放的字符
    return this->ch;
}


///
/// \brief Node::get_power 获取权值
/// \return 返回结点的权值
///
int Node::get_power()
{
    // 获取结点的权值
    return this->power;
}

