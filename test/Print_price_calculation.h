#include <stdio.h>
#include <ctype.h>
#include <math.h>
#define unit_selling_price 0.2
#define last_discount 0.5
#define chnum 3
#define A "Please select the price calculation method:\n"
#define B "      1.one-side  2.two-side  3.exit\n"
#define C "Please enter 1 or 2\n"
#define D "When you enter the number \"%d\", you will exit the program!\n", chnum
#define E "Please enter a positive integer!\n"
#define F "You should have just entered a positive integer!\n"
#define G "This option is not developed yet!\n"
//菜单打印
void menu(void);
//获取数字
int getnum(void);
//获取选择
int my_choose(void);
//分段函数
double judge(int n);
//清空缓冲区
void Clear_buffer(void);
//单面打印计算
double paper_price1(void);
//双面打印计算
double paper_price2(void);
//金额四舍五入
double change(double x);
double paper_price1(void)
{
    printf(E);
    int n;
    while (!(n = getnum()))
        printf(E);
    if (n > 0)
    {
        if (n < 11)
            return (n > 6 ? 3 : (n > 3 ? 2 : 1));
        if (n > 500)
            return n * unit_selling_price * last_discount;
        if (n > 10)
            return judge(n) < 3 ? 3 : judge(n);
    }
    printf(F);
    return 0;
}
double paper_price2(void)
{
    printf(E);
    int n;
    while (!(n = getnum()))
        printf(E);
    if (n > 0)
    {
        if (n % 2 > 0)
            n = (n / 2) + 1;
        else
            n /= 2;
        if (n < 11)
            return (n > 6 ? 4 : (n > 3 ? 3 : 2));
        if (n > 500)
            return n * unit_selling_price * last_discount;
        if (n > 10)
            return judge(n) < 4 ? 4 : judge(n);
    }
    printf(F);
    return 0;
}
double change(double x)
{
    if (x - floor(x) <= 0.5)
        return floor(x) + 0.5;
    else
        return ceil(x);
}
void Clear_buffer(void)
{
    while ('\n' != getchar())
        continue;
    return;
}
double judge(int n)
{
    int i = n % 50, j = n / 50;
    if (i >= 0)
    {
        if (i == 0)
            --j;
        return n * unit_selling_price * (0.95 - 0.05 * j);
    }
}
int my_choose(void)
{
    menu();
    int c;
    do
    {
        c = getnum();
        if (c > 0 && c <= chnum)
            return c;
        else
        {
            c = 0;
            printf("reenter choose,it's 1 to %d.\n", chnum);
        }
    } while (!c);
}
int getnum(void)
{
    int c;
    if (1 == scanf("%d", &c))
    {
        Clear_buffer();
        return c;
    }
    else
    {
        printf("input error\n");
        Clear_buffer();
        return 0;
    }
}
void menu(void)
{
    printf(A B C D);
    return;
}