#pragma once

typedef double Dtype;

// 二、三阶行列式求解
Dtype determinant_2_3(int Order, Dtype a[Order][Order]);

// n阶行列式求解（转换为上三角矩阵）
Dtype n_determinant(int Order, Dtype a[Order][Order]);

