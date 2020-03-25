#ifndef HANOI_H
#define HANOI_H

#include <QStack>

///
/// \brief The Step struct 用于记录汉诺塔移动步骤
///
typedef struct Step
{
    Step(int start, int end):init(start),aim(end){}
    int init;   // 起始柱子下标
    int aim;    // 目标柱子下标
}Step;


///
/// \brief The Hanoi class 用于求解并存储汉诺塔移动步骤
///
class Hanoi
{
public:
    ~Hanoi();
    void init(int num);     // 初始化Hanoi对象
    Step * get_next_step(); // 获取下一步并返回
    Step * get_last_step(); // 获取上一步并返回
    void reset();           // 重置Hanoi对象

    int num_of_steps;               // 记录总步数
    QStack<Step*> unexecuted_steps; // 存储未执行的步骤
    QStack<Step*> executed_steps;   // 存储已执行的步骤

private:
    void generate_steps(int num,int a, int b, int c);   // 生成移动步骤
    void delete_steps();                                // 清空步骤
};

#endif // HANOI_H
