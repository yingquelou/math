// import java.io.*;
public class HelloWorld {
    public static void main(String[] args) {
        // final
        /*
         * byte:-128 ~ 127
         * short: -32768 ~ 32767
         * int: -2147483648 ~ 2147483647
         * long: -9223372036854775808 ~ 9223372036854775807
         */
        final byte bt = 10;
        short st = bt;
        int it = st;
        long lg = it;
        float fl = lg;
        double db = fl;
        char ch = 'a';
        boolean bl = true;
        var val = bl;
        /*
         *)
         * ! ~ ++ --
         * / %
         * + -
         * << >> >>>
         * &
         * |
         * += -= *= /=
         * !
         * >  >=  <  <=
         * ==  !=
         * &&
         * ||
         */
        for(int i = 0; i < 10; ++i)
            System.out.println(i + it);
    }
}