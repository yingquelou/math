#include <algorithm> //大多数泛型算法算法都定义在头文件algorithm中。
#include <numeric>   //标准库还在头文件numeric中定义了一组数值泛型算法。
#include <stack>     //容器适配器
#include <queue>     //容器适配器
#include <iterator>  //迭代器
#include <iostream>
using namespace std;
using Elem = double;
bool My_compare(const Elem &e1, const Elem &e2)
{
    return e1 < e2;
}
int main(void)
{
    deque<int> iq1{1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    deque<double> iq2{1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    /* 只读算法find、count、accumulate、equal等 */
    // 在范围内查找第三个参数，找到了返回其位置(迭代器)
    auto result1 = find(iq1.cbegin(), iq1.cend(), 9);
    // 返回范围内第三个参数出现的次数
    auto result2 = count(iq1.cbegin(), iq1.cend(), 9);
    // 返回范围内的元素与第三个参数的累和
    // 如果运算符重载了,累加的效果是不同的，例如:若元素类型是string时,效果是拼接串
    auto result3 = accumulate(iq1.cbegin(), iq1.cend(), 9);
    // 判断两个序列中的元素是否对应相等
    auto result4 = equal(iq1.cbegin(), iq1.cend(), iq2.cbegin());
    cout << *result1 << endl
         << result2 << endl
         << result3 << endl
         << result4 << endl;
    /* 在非空容器内批量修改元素,不会改变容器的大小 */
    fill(iq1.begin(), iq1.end(), 1);
    fill_n(iq2.end() - 2, 2, 3);
    /* 插入迭代器 */
    auto it = back_inserter(iq1);
    /* back_inserter接受一个指向容器的引用，返回一个与该容器绑定的插入迭代器。
    当我们通过此迭代器赋值时，赋值运算符会调用push_back将一个具有给定值的元素添加到容器中 */
    fill_n(it, 5, 6);
    *it = 1;
    it = 2;
    // 替换指定的值，原序列被改变
    replace(iq1.begin(), iq1.end(), 6, 8);
    // 替换指定的值，原序列保留
    deque<int> tmp;
    replace_copy(iq1.begin(), iq1.end(), back_inserter(tmp), 8, 5);
    deque<double> iq3{1, 3, 2, 2, 3, 4, 5, 3, 6, 7, 8, 9, 0};
    // 序列元素有重载<,即sort内部是用重载的<进行元素间的比较，则可用sort排序
    // 同时
    sort(iq3.begin(), iq3.end());

    sort(iq3.begin(), iq3.end(), My_compare);
    // 在sort后为保证序列前部元素的唯一性,序列前部每个重复多余的元素都将被之后与其相异的元素覆盖，最终unique返回指向序列前部尾元素之后的迭代器
    auto need_erase_pos = unique(iq3.begin(), iq3.end());
    // 删除多余的元素
    iq3.erase(need_erase_pos, iq3.end());
    stable_sort(iq3.begin(), iq3.end(), My_compare);
    // 接受一个谓词将序列划分(重新排序)成前后两个逻辑部分
    // partition();
    // 在经过sort(如果需要排序)后返回第一个满足谓词的迭代器
    // find_if();

    // lambda表达式可以定义一个匿名"函数"
    // [capture list] (parameter list) -> return type { function body }
    // [捕获列表] (参数列表) -> return 类型 {函数体}
    // 参数列表、返回类型、函数体的情况与一般函数类似
    // 可以忽略参数列表和返回类型，但必须永远包含捕获列表和函数体
    auto f1 = []
    { return 42; }; //只存在return语句，才可以auto
    cout << f1() << endl;
    // 捕获列表只用于局部非static变量，lambda可以直接使用局部static变量和在它所在函数之外声明的名字。
    // 捕获列表若非空,则主要分为显示捕获和隐式捕获以及显隐混合
    // 显示捕获————值捕获
    auto f2 = [iq1]{ return iq1.size(); };
    // 显示捕获————引用捕获
    auto f3 = [&iq1]{ return iq1.size(); };
    // 隐式捕获：让编译器推断哪些变量需要按照指定的方式捕获
    // 隐式捕获————"=":=告诉编译器默认采用值捕获方式
    auto f4 = [=]{ return iq1.size(); };
    // 隐式捕获————"&":&告诉编译器默认采用引用捕获方式
    auto f5 = [&]{ return iq1.size(); };
    // 当我们使用显隐混合时，捕获列表中的第一个元素必须是一个&或=。此符号指定了默认捕获方式为引用或值。
    // 显隐混合————除iq3外均采用引用捕获
    auto f6 = [&,iq3]{ return iq1.size()+iq3.size(); };
    // 显隐混合————除iq2外均采用值捕获
    auto f7 = [=,&iq2]{ return iq1.size()+iq3.size(); };

    return 0;
}