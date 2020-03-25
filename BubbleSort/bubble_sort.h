#ifndef BUBBLE_SORT_H
#define BUBBLE_SORT_H

#include <QVector>


///
/// \brief Sort_Step 用于记录冒泡排序步骤
///
typedef struct Sort_Step
{
    Sort_Step(int i,int j, int flag, QVector<int> &array)
    {
        this->i = i;
        this->j = j;
        this->flag = flag;
        this->array = array;
    }
    int i;
    int j;          //记录比较的数据下标
    int flag;       //记录已经排好序的下标起始位置
    QVector<int> array;
}Sort_Step;


///
/// \brief The Bubble_Sort class 用于实现冒泡排序并记录过程
///
class Bubble_Sort
{
public:
    void init(QVector<int> array);  //初始化
    void sort();                    //冒泡排序算法
    QVector<Sort_Step> steps;       //存储排序的步骤
    QVector<int> array;             //存储数据
};

#endif // BUBBLE_SORT_H
