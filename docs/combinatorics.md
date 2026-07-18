# 排列组合

在 C/C++ 中，有关排列组合问题的标准库函数通常包含在 `<cmath>` 和 `<algorithm>` 头文件中。以下是一些常用的函数：

1. factorial(n)：计算 n 的阶乘，即 n!。
2. permutation(n, k)：计算从 n 个元素中选择 k 个元素进行排列的方案数，即 A(n,k)。
3. combination(n, k)：计算从 n 个元素中选择 k 个元素进行组合的方案数，即 C(n,k)。
4. next_permutation()：生成下一个排列，用于枚举排列或求解排列的全排列。
5. prev_permutation()：生成上一个排列，与 next_permutation() 相反。

这些函数的具体使用方法可以参考 C/C++ 的相关文档和教程。另外，也可以自己实现这些函数，例如通过循环或递归来计算阶乘、排列数和组合数。
