/* 异常处理初识 */
#include <iostream>
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
#include <typeinfo>
using std::bad_cast; //
#include <new>
using std::bad_alloc; //
#include <stdexcept>
using std::domain_error;     //逻辑错误：参数对应结果值不存在
using std::exception;        //最常见的问题
using std::invalid_argument; //逻辑错误：无效参数
using std::length_error;     //逻辑错误：试图创建一个超出该类型长度的对象
using std::logic_error;      //程序逻辑错误：
using std::out_of_range;     //逻辑错误：使用一个超出有效范围的值
using std::overflow_error;   //运行时错误：计算上溢
using std::range_error;      //运行时错误：生成的结果超出了有意义的值域范围
using std::runtime_error;    //只有在运行时才能检测出的问题
using std::terminate;        //无匹配的catch块时，将调用标准库默认的异常处理函数
using std::underflow_error;  //运行时错误：计算下溢
int main(void)
{
    int x, y;
    try
    {
        cin >> x >> y;
        if (!y)
        {
            auto p = invalid_argument("The divisor cannot be zero");
            cout << p.what() << endl;
            throw p;
            // throw invalid_argument("y isn't zero!"); //以上两三步与此句等价
        }
        cout << x / y;
    }
    catch (runtime_error)
    {
    }
    catch (invalid_argument)
    {
        cout << "please" << endl;
    }
    cout << "hello" << endl;
    return 0;
}