tuple支持的操作|解释说明
|:-:|:-|
`tuple<T1，T2，....Tn> t`| `t`是一个`tuple`，成员数为`n`，第`i`个成员的类型为`Ti`。 所有成员都进行值初始化
`tuple<T1，T2，.Tn> t(v1,v2，....vn)` |`t`是一个`tuple`，成员类型为`T1…Tn`，每个成员用对应的初始值vi进行初始化。此构造函数是explicit的
`make_tuple(vl,v2.....vn)` |返回一个用给定初始值初始化的`tuple`。`tuple`的类型从初始值的类型推断
`t1=t2` |当两个`tuple`具有相同数量的成员且成员对应相等时，两个`tuple`相等。这两个操作使用成员的`==`运算符来完成。一旦发现某对成员不等，接下来的成员就不用比较了 
`t1!= t2` |
`t1 relop t2` |`tuple`的关系运算使用字典序.两个`tuple`必须具有相同数量的成员。使用`<`运算符比较`t1`的成员和`t2`中的对应成员
`get<i>(t)` |返回`t`的第`i`个数据成员的引用:如果`t`是一个左值，结果是一个左值引用;否则，结果是一个右值引用。`tuple`的所有成员都是`public`的
`tuple_size<tupleType>::value` |一个类模板，可以通过一个`tuple`类型来初始化。它有一个名为`value`的`public constexpr static`数据成员，类型为`size_t`，表示给定`tuple`类型中成员的数量
`tuple_element<i,tupleType>::type`| 一个类模板，可以通过一个整型常量和一个`tuple`类型来初始化。它有一个名为`type`的`public`成员，表示给定`tuple`类型中指定成员的类型