#include "Print_price_calculation.h"
#define paper1 0.8
#define paper2 0.6
int main(void)
{
    int c;
    double money;
    while (c = my_choose())
        switch (c)
        {
        case 1:
            money = paper_price1();
            if (money <= 3)
            {
                printf("It's %.1lf yuan.\n", money);
                break;
            }
            else
            {
                money /= paper1;
                money = change(money);
                printf("It's %.1lf yuan.\n", money);
                break;
            }
        case 2:
            money = paper_price2();
            if (money <= 4)
            {
                printf("It's %.1lf yuan.\n", money);
                break;
            }
            else
            {
                money /= paper2;
                money = change(money);
                printf("It's %.1lf yuan.\n", money);
                break;
            }
        default:
            return 0;
            break;
        }
}