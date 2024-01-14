#pragma once
#include "Point.h"
#include "Line.h"
/* 双曲线————椭圆类
一般方程：ax^2+by^2+cx+dy+e=0
*构造椭圆的常规方法：
*1. 长轴 短轴
*2. 焦点 定长
*3. 焦点 准线 离心率*/
class Ellipse
{
private:
    // x2:x的平方项系数
    // y2:y的平方项系数
    // xy:xy项的系数
    // x1:x的一次项系数
    // y1:y的一次项系数
    // c:常数项
    double x2, y2, xy, x1, y1, c;

public:
    //无参构造
    Ellipse(void) = default;
    //拷贝构造
    Ellipse(const Ellipse &E);
    //长轴 短轴——构造
    Ellipse(const double &, const double &);
    //焦点 定长——构造
    Ellipse(const Point &, const double &);
    // 焦点 准线 离心率——构造
    Ellipse(const Point &, const Point &, const double &);
    ~Ellipse();

public: //重载运算符
    Ellipse &operator=(const Ellipse &E);
};