#include "bubble_sort.h"


///
/// \brief Bubble_Sort::init 初始化
/// \param array 记录数据的数组
///
void Bubble_Sort::init(QVector<int> array)
{
    this->steps.clear();
    this->array = array;
    this->sort();
}


///
/// \brief Bubble_Sort::sort 冒泡排序算法
///
void Bubble_Sort::sort()
{
    //若发生交换则为真，未发生则为假
    bool change_flag;
    for(int i=0;i<this->array.size()-1;i++)
    {
        change_flag=false;

        for(int j=0;j<this->array.size()-i-1;j++)
        {
            //保存每一步到步骤
            this->steps.push_back(Sort_Step(j,j+1,this->array.size()-i-1,this->array));
            if(this->array[j]>this->array[j+1])
            {
                int temp = this->array[j];
                this->array[j] = this->array[j+1];
                this->array[j+1] = temp;
                change_flag = true;
            }
        }
        //一次冒泡未交换数据，则说明排序完成
        if(change_flag==false)
            break;
    }
}
