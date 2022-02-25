// import java.io.*;
public class HelloWorld {
    public static void main(String[] args) {
        // 需要用关键字final声明一个常量
        // 整数类型
        /*
         * byte：-128 ~ 127
         * short: -32768 ~ 32767
         * int: -2147483648 ~ 2147483647
         * long: -9223372036854775808 ~ 9223372036854775807
         */
        final byte bt = 10;
        short st = bt;
        int it = st;
        long lg = it;
        // 浮点数类型
        float fl = lg;
        double db = fl;
        // 字符类型
        char ch = 'a';
        // 布尔类型
        boolean bl = true;
        // 关键字var的作用类似于C++中的auto
        var val = bl;
        /*
         * 定义变量时，要遵循作用域最小化原则，尽量将变量定义在尽可能小的作用域，
         * 并且，不要重复使用变量名
         * 运算优先级从高到低依次是:
         * ()
         * ! ~ ++ --
         * / %
         * + -
         * << >> >>>
         * &
         * |
         * += -= *= /=
         * 关系运算符的优先级从高到低依次是：
         * !
         * >，>=，<，<=
         * ==，!=
         * &&
         * ||
         */
        for (int i = 0; i < 10; ++i)
            System.out.println(i + it);
    }
}