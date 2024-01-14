# 笔记

在 C/C++ 中，有关排列组合问题的标准库函数通常包含在 `<cmath>` 和 `<algorithm>` 头文件中。以下是一些常用的函数：

1. factorial(n)：计算 n 的阶乘，即 n!。
2. permutation(n, k)：计算从 n 个元素中选择 k 个元素进行排列的方案数，即 A(n,k)。
3. combination(n, k)：计算从 n 个元素中选择 k 个元素进行组合的方案数，即 C(n,k)。
4. next_permutation()：生成下一个排列，用于枚举排列或求解排列的全排列。
5. prev_permutation()：生成上一个排列，与 next_permutation() 相反。

这些函数的具体使用方法可以参考 C/C++ 的相关文档和教程。另外，也可以自己实现这些函数，例如通过循环或递归来计算阶乘、排列数和组合数。

## $constexpr$

在 C++11 中引入了关键字 constexpr，用于指定一个函数或变量是在编译期间可求值的常量表达式。constexpr 可以用于以下场景：

1. 常量表达式函数：如果一个函数满足一些限制条件（例如只包含返回语句和条件语句等），则可以将其声明为 constexpr 函数。这样，在编译期间调用该函数时，会被视为常量表达式并进行优化，从而加速程序运行。例如：

```c
constexpr int factorial(int n) {
    return n == 0 ? 1 : n * factorial(n - 1);
}

constexpr int x = factorial(5); // 在编译期间计算出 x 的值为 120
```

2. 字面值类型变量：如果一个变量是字面值类型（例如整数、浮点数、枚举类型等），并且其初始化表达式是常量表达式，则可以将其声明为 constexpr 变量。这样，在编译期间使用该变量时，也会被视为常量表达式并进行优化。例如：

```c
constexpr int n = 10;
constexpr double pi = 3.14159;
```

3. 模板参数：在模板中，可以将某个参数声明为 constexpr，从而限制其取值必须是常量表达式。这样，在实例化模板时，就可以在编译期间求解该参数的值。例如：

```c
template <int N>
struct Fibonacci {
    static const int value = Fibonacci<N-1>::value + Fibonacci<N-2>::value;
};

template <>
struct Fibonacci<0> {
    static const int value = 0;
};

template <>
struct Fibonacci<1> {
    static const int value = 1;
};

constexpr int fib = Fibonacci<10>::value; // 在编译期间计算出斐波那契数列的第十项为 55
```

在以上场景中，使用 constexpr 可以提高程序的性能和可移植性，因为它可以将某些运算在编译期间完成，从而减少运行时的开销。同时，constexpr 还可以帮助编写更加安全和可靠的代码，因为它可以确保某些变量或函数不会被无意之间改变。

## concept

在 C++20 中引入了关键字 concept，用于定义模板参数的约束条件，从而限制该参数可以取值的范围。concept 可以用于以下场景：

1. 定义函数或类模板的约束条件：可以使用 concept 关键字定义一个约束条件，例如对某个模板参数进行类型检查、成员函数或操作符要求等。在定义函数或类模板时，可以使用 requires 子句将所需的 concept 引入，从而限制该参数的取值范围。例如：

```cpp
template<typename T>
concept Integral = std::is_integral_v<T>;

template <typename T>
requires Integral<T>
void foo(T value) {
    // ...
}
```

在上述示例中，定义了一个名为 Integral 的 concept，用于约束模板参数必须是整数类型。在定义函数模板时，使用 requires 子句引入该 concept，从而限制模板参数的取值范围。

2. 提供模板参数的文档和约束信息：可以使用 concept 关键字声明一个只有文档作用的约束条件，用于提供模板参数的约束信息和文档说明。这样，在调用模板时，可以通过编译器提示来了解该参数的约束条件和可取值范围。例如：

```cpp
template <typename T>
concept Copyable = requires (T a) {
    {a = a};
    {new T(a)};
    {delete new T(a)};
};

template <Copyable T>
void foo(T value) {
    // ...
}
```

在上述示例中，定义了一个名为 Copyable 的 concept，用于约束模板参数必须支持拷贝构造函数、拷贝赋值运算符和析构函数。虽然这些要求并不是实际的约束条件，但它们可以提供调用者所需的文档信息和约束提示。

通过使用 concept，C++ 可以更加灵活和类型安全地实现泛型编程，同时也可以提高代码的可读性和可维护性。

### 另例

在C/C++中，关键字"concept"是C++20新引入的特性，用于定义模板类型参数的约束条件。具体而言，通过使用concept关键字可以在模板函数或模板类的类型参数中指定一个或多个约束条件，以保证传入的实参类型满足某些要求。

以下是一些concept关键字的用例：

1. 定义可调用对象的概念

```cpp
template<typename F>
concept Callable = requires(F f) {
    {f()} -> std::convertible_to<int>;
};

void my_function(Callable auto func) {
    // 检查func是否可调用并返回int类型
    int result = func();
    // ...
}
```

在这个例子中，我们定义了一个名为Callable的concept，它要求模板参数F必须是一个可调用对象，并且其返回值可以转换为int类型。然后我们在my_function函数的参数列表中使用了这个concept，以确保传入的函数对象符合要求。

2. 定义迭代器的概念

```cpp
template<typename T>
concept Iterator = requires(T t) {
    {++t} -> std::same_as<T&>; // 能够前置递增，并返回T&
    {*t} -> std::copy_constructible; // 可以进行解引用，并且可以复制构造
};

template<Iterator Iter>
void my_algorithm(Iter begin, Iter end) {
    // 使用迭代器对区间进行操作
    for (Iter it = begin; it != end; ++it) {
        // ...
    }
}
```

在这个例子中，我们定义了一个名为Iterator的concept，它要求模板参数T必须是一个迭代器，并且其支持前置递增和解引用操作。然后我们在my_algorithm函数的参数列表中使用了这个concept，以确保传入的迭代器符合要求。

通过使用concept关键字，我们可以更加精确地指定模板类型参数的约束条件，从而使代码更加健壮和易于维护。

## &与&&

在 C++11 标准中，类的成员函数可以通过后置限定符&和&&来指定“左值”和“右值”限定符。这些限定符定义了成员函数能否被非 const 或右值对象调用的规则。

- `&`限定符：表示该成员函数可以接受“左值”对象的调用。

  - 示例：一个字符串类的成员函数 append()使用&限定符可以确保只有左值调用该函数，从而避免了潜在的资源泄漏问题：

    ```c
    class MyString {
    public:
        // 左值引用限定符&
        void append(const MyString& str) & {
            // 实现代码
        }
    };
    ```

- `&&`限定符：表示该成员函数可以接受“右值”对象的调用。

  - 示例：一个移动语义相关的类的成员函数 move()使用&&限定符可以确保只有右值调用该函数，从而避免潜在的数据共享和安全问题：

    ````c
    class MyObject {
    public:
    // 右值引用限定符&&
    MyObject(MyObject&& other) noexcept {
    // 实现代码
    }
            // 返回右值引用
            MyObject&& move() && noexcept {
                return std::move(*this);
            }
        };
        ```
    注意，限定符&和&&不能同时使用。如果没有指定限定符，则默认为&限定符，允许左值和右值对象都可以调用该成员函数。
    ````

### 另例

在C++中，类成员函数可以通过在其声明中添加限定符`&`或`&&`来指定该函数的“引用资格”，这些限定符通常被称为后置说明符（postfix qualifiers），它们用于区分可以修改其调用对象的函数和不可修改其调用对象的函数。以下是这两个限定符的用途：

- `&`后置限定符：表示该函数必须以左值形式调用，即只能被一个具有名称的对象调用，并且不能在右值表达式中使用。使用`&`后置限定符的函数被称为“左值引用修饰函数”（lvalue-ref-qualified function）。

```cpp
class Example {
public:
    void setValue(int val) & { // 该函数只能被左值对象调用
        memberVar = val;
    }
private:
    int memberVar;
};

int main() {
    Example ex1;
    ex1.setValue(10);   // 正确，ex1是一个左值
    Example().setValue(20); // 错误，Example()返回的是一个右值
    return 0;
}
```

在上面的示例中，`setValue`函数使用`&`后缀说明符来限制该函数只能由左值调用。因此，我们可以使用`ex1`调用`setValue`方法，但不能使用临时对象或其他右值调用该函数。

- `&&`后置限定符：表示该函数可以以右值形式调用，即可以通过右值引用参数调用，也可以通过值参数调用。使用`&&`后置限定符的函数被称为“右值引用修饰函数”（rvalue-ref-qualified function）。

```cpp
class Example {
public:
    int getValue() && { // 该函数可以被左值和右值对象调用
        return memberVar;
    }
private:
    int memberVar;
};

int main() {
    Example ex1;
    ex1.getValue(); // 错误，getValue不能被左值调用
    Example().getValue(); // 正确，getValue可以被右值调用
    return 0;
}
```

在上述示例中，`getValue`函数使用`&&`后缀说明符来允许该函数以右值形式调用，并且还可以接受左值调用。因此，我们可以使用一个右值或左值调用`getValue`方法。

需要注意的是，一个成员函数只能同时拥有一个后置限定符，即要么是`&`或者是`&&`，而不能两者都有。此外，在使用后置限定符时，如果成员函数没有修改其调用对象的意图，则应使用`const`关键字进行修饰。
