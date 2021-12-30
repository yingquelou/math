#include <iostream>
#include <vector>
using namespace std;
#define Mod 10;
using std::vector;
using MyDouble = double;
// 行/列矩阵
using Mat1 = vector<MyDouble>;
// 二维矩阵
using Mat2 = vector<Mat1>;
class Matrix : public Mat2
{
    // 友元
    // 显示矩阵
    friend ostream &operator<<(ostream &, const Matrix &);

private:
    // Mat2 *pMat;

public:
    // 矩阵的合法性检验
    bool IsLegitimate() const;
    // 加法(同型矩阵)
    Matrix operator+(const Matrix &) const;
    // 乘法
    Matrix operator*(const Matrix &) const;
    // 数乘
    // 幂运算
public:
    // 初等变换
    // 求逆
    // 转置
    //
    // Matrix(const Matrix &);
    /*~Matrix(); */
};
using pMatrix = Matrix *;