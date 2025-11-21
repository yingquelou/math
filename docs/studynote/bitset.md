初始化bitset的方法|解释说明
|:-:|:-|
`bitset<n> b`|`b`有`n`位;每一位均为`0`。此构造函数是一个`constexpr`
`bitset<n> b(u)`| `b`是`unsigned long long`值`u`的低`n`位的拷贝。如果`n`大于`unsigned long long`的大小,则`b`中超出`unsigned long long`的高位被置为`0`。此构造函数是一个`constexpr`
`bitset<n> b(s,pos,m,zero,one)`| `b`是`string s`从位置`pos`开始`m`个字符的拷贝。`s`只能包含字符`zero`或`one`;如果`s`包含任何其他字符，构造函数会抛出`invalid_argument`异常。字符在`b`中分别保存为`zero`和`one`。`pos`默认为`0`，`m`默认为`string::npos`，`zero`默认为`'0'`，`one`默认为`'1'`
`bitset<n> b(cp,pos,m,zero,one)`| 与上一个构造函数相同，但从`cp`指向的字符数组中拷贝字符。如果未提供`m`，则`cp`必须指向一个C风格字符串。如果提供了`m`，则从`cp开`始必须至少有`m`个`zero`或`one`字符
```cpp
接受一个string或一个字符指针的构造函数是explicit的。在新标准中增加了为0和1指定其他字符的功能。
```
bitset操作|解释说明
|:-:|:-|
`b.any()` |`b`中是否存在置位的二进制位 
`b.all()` |`b`中所有位都置位了吗 
`b.none()` |`b`中不存在位的二进制位吗 
`b.count()` |`b`中置位的位数 
`b.size()` |一个`constexpr`函数,返回`b`中的位数
`b.test(pos)`| 若`pos`位置的位是置位的，则返回`true`,否则返回`false`
`b.set(pos,v)`|将位置`pos`处的位设置为`bool`值`v`。`v`默认为`true`。如果未传递实参，则将`b`中所有位置位
`b.set()` | 
`b.reset(pos)`|将位置`pos`处的位复位或将`b`中所有位复位 
`b.reset()`|
`b.flip(pos)`|改变位置`pos`处的位的状态或改变`b`中每一位的状态 
`b.flip()`|
`b[pos]`| 访问`b`中位置`pos`处的位:如果`b`是`const`的，则当该位置位时`b[pos]`返回一个`bool`值`true`，否则返回`false`
`b.to_ulong()`|返回一个`unsigned long`或一个`unsigned long long`值,其位模式与b相同。如果`b`中位模式不能放入指定的结果类型,则抛出一个`overflow_error`异常
`b.to_ullong()`|
`b.to_string(zero,one)`|返回一个`string`，表示`b`中的位模式。`zero`和`one`的默认值分别为`0`和`1`，用来表示`b`中的`0`和`1` 
`os<<b`|将`b`中二进制位打印为字符`1`或`0`打印到流`os` 
`is >> b`|从`is`读取字符存入`b`。当下一个字符不是`1`或`0`时,或是已经读入`b.size()`个位时，读取过程停止