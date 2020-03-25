#include "hanoi.h"


Hanoi::~Hanoi()
{
    this->delete_steps();
}


///
/// \brief Hanoi::init 初始化Hanoi对象
/// \param num 汉诺塔盘子的数目
///
void Hanoi::init(int num)
{
    // 清空步骤
    this->delete_steps();

    // 生成步骤
    generate_steps(num, 0,1,2);

    // 将生成的步骤放入未执行步骤中，使得步骤顺序为正序
    while(this->executed_steps.isEmpty()==false)
        this->unexecuted_steps.push(this->executed_steps.pop());

    // 将步骤总数保存到num_of_steps中
    this->num_of_steps = this->unexecuted_steps.size();
}


///
/// \brief Hanoi::get_next_step 获取下一步并返回
/// \return 下一步骤的指针，如果无，返回nullptr
///
Step * Hanoi::get_next_step()
{
    if(this->unexecuted_steps.isEmpty())
        return nullptr;
    Step *s = this->unexecuted_steps.pop();
    this->executed_steps.push(s);
    return s;
}


///
/// \brief Hanoi::get_last_step 获取上一步并返回
/// \return 上一步骤的指针，如果无，返回nullptr
///
Step * Hanoi::get_last_step()
{
    if(this->executed_steps.isEmpty())
        return nullptr;
    Step *s = this->executed_steps.pop();
    this->unexecuted_steps.push(s);
    return s;
}



///
/// \brief Hanoi::reset 重置Hanoi对象
///
void Hanoi::reset()
{
    // 清空步骤
    this->delete_steps();
    this->num_of_steps = 0;
}


///
/// \brief Hanoi::generate_steps 生成移动步骤
/// \param num 需要移动的盘子数目
/// \param a 起始柱子下标
/// \param b 中介柱子下标
/// \param c 目标柱子下标
///
void Hanoi::generate_steps(int num, int a, int b, int c)
{
    // 生成汉诺塔移动步骤，并将步骤先放入已执行步骤的栈中
    Step *s;
    if(num==1)
    {
        s = new Step(a,c);
        this->executed_steps.push(s);
    }
    else
    {
        this->generate_steps(num-1, a, c, b);
        s = new Step(a,c);
        this->executed_steps.push(s);
        this->generate_steps(num-1, b, a, c);
    }
}


///
/// \brief Hanoi::delete_steps 清空步骤
///
void Hanoi::delete_steps()
{
    // 将已执行步骤和未执行步骤栈全部清空
    Step * s;
    while(this->unexecuted_steps.isEmpty()==false)
    {
        Step *s = this->unexecuted_steps.pop();
        delete s;
    }
    while(this->executed_steps.isEmpty()==false)
    {
        s = this->executed_steps.pop();
        delete s;
    }
    s=nullptr;
}
