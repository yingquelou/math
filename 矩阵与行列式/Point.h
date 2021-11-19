#pragma once
//判断点/向量相同或平行时所允许的误差
#define allowableError (1e-15)
#include <cmath>
using std::abs;
using std::sqrt;
#include <iostream>
using std::cerr;
using std::endl;
using std::ostream;
#include <cmath>
using std::acos;
// using namespace std;
// 平面向量/点类(坐标表示)
class Point
{ //友元

    //以 (横坐标,纵坐标) 形式输出坐标/向量,例如：(1,1)
    friend ostream &operator<<(ostream &cout, const Point &P);
    //向量的数乘(数前置版) 滑动向量的产生
    friend Point operator*(const double &b, const Point &P);

private: //成员变量
    double x;
    double y;

public: //构造与析构
    //无参构造
    Point();
    //拷贝构造
    Point(const Point &point);
    //有参构造:以形参顺序构造横、纵坐标
    Point(const double &, const double &);
    //有参构造2
    Point(const Point &, const double &, const double &);
    ~Point(){};

public: //重载运算符
    //向量/坐标拷贝(赋值)
    Point &operator=(const Point &b);
    //向量的数量积(内积、点积)
    double operator*(const Point &b) const;
    //向量的数乘(数后置版) 滑动向量的产生
    Point operator*(const double &b) const;
    //当前向量的横坐标变为原来的P.x倍,纵坐标变为原来的P.y倍
    Point &operator*=(const Point &P);
    //向量的合成与分解——合成
    //点/向量相加即对应坐标相加
    //向量的加法满足平行四边形法则和三角形法则
    Point operator+(const Point &P) const;
    //点/向量相减即对应坐标相减
    //向量的减法满足三角形法则
    Point operator-(const Point &P) const;
    //返回当前向量的相反(负)向量
    Point operator-() const;
    //前置递减为横坐标递减
    Point &operator--();
    //后置递减为纵坐标递减
    Point &operator--(int);
    //前置递增为横坐标递增
    Point &operator++();
    //后置递增为纵坐标递增
    Point &operator++(int);
    //判断点/向量相同
    bool operator==(const Point &P) const;
    //向量间判断平行(共线)
    //当零向量参与平行的判断时,请谨慎使用!
    //因为此时亦会返回true
    bool operator||(const Point &P) const;

public: //成员函数
    //获取横坐标
    double Get_x() const { return x; };
    //获取纵坐标
    double Get_y() const { return y; };
    //向量的模
    double VectorMold() const;
    //求解两向量的夹角
    double AngleOfVector(const Point &P) const;
    //向量的合成与分解——分解
    //求当前向量在另一非零向量上的分量(投影)
    Point VectorComponent(const Point &P) const;
    //求当前向量在以向量ex和ey为基底的平面坐标系中的坐标表示
    Point CoordinateInBase(const Point &ex, const Point &ey) const;
};
using VectorOfMath = Point;