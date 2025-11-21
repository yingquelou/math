#ifndef ARRAY_HPP
#define ARRAY_HPP 1
#include <vector>
// 线性代数与线性空间
namespace linear
{
    /*
        容器类
        - 线性运算的基本数据结构
        - 只作为容器,具体的线性运算由其他类或函数承担
    */
    template <typename T, typename SizeType = std::size_t>
    class array
    {
    public:
        // 类型定义
        using value_type = T;       // 元素类型
        using size_type = SizeType; // 大小类型

    private:
        //  数据成员
        std::vector<value_type> values; // 存储数据的容器
        std::vector<size_type> shape;   // 数组的形状信息
    public:
        // 构造与析构

        // 默认构造函数
        array() = default;
        // 根据形状初始化
        array(const std::vector<size_type> &shape_) : shape(shape_)
        {
            size_type totalSize = 1;
            for (const auto &dim : shape)
            {
                totalSize *= dim;
            }
            values.resize(totalSize);
        }
        // 拷贝构造函数
        array(const array &other) : values(other.values), shape(other.shape) {}
        // 移动构造函数
        array(array &&other) noexcept : values(std::move(other.values)), shape(std::move(other.shape)) {}

        // 析构函数
        ~array() = default;


    private:

    public:
        // 静态成员函数
        /*
            多维索引->线性索引
         */
        static size_type toFlatIndex(const std::vector<size_type> &indices, const std::vector<size_type> &shape)
        {
            size_type flatIndex = 0;
            size_type stride = 1;
            for (size_type i = shape.size(); i-- > 0;)
            {
                flatIndex += indices[i] * stride;
                stride *= shape[i];
            }
            return flatIndex;
        }
        /* 线性索引->多维索引 */
        static std::vector<size_type> toMultiIndex(size_type flatIndex, const std::vector<size_type> &shape)
        {
            std::vector<size_type> indices(shape.size());
            for (size_type i = shape.size(); i-- > 0;)
            {
                indices[i] = flatIndex % shape[i];
                flatIndex /= shape[i];
            }
            return indices;
        }

    public:
        // 重载运算符

        // 赋值运算符
        array &operator=(const array &other)
        {
            if (this != &other)
            {
                values = other.values;
                shape = other.shape;
            }
            return *this;
        }
        // 移动赋值运算符
        array &operator=(array &&other) noexcept
        {
            if (this != &other)
            {
                values = std::move(other.values);
                shape = std::move(other.shape);
            }
            return *this;
        }

        /*
            访问元素（线性索引版本）
        */
        value_type &operator()(size_type index)
        {
            return values[index];
        }
        /*
            访问元素（线性索引版本，常量版本）
        */
        const value_type &operator()(size_type index) const
        {
            return values[index];
        }

        /*
            访问元素
            - 支持多维索引访问
        */
        value_type &operator()(const std::vector<size_type> &indices)
        {
            return values[toFlatIndex(indices, shape)];
        }
        /*
            访问元素（常量版本）
            - 支持多维索引访问
         */
        const value_type &operator()(const std::vector<size_type> &indices) const
        {
            return values[toFlatIndex(indices, shape)];
        }
        // 转换函数
        operator bool() const
        {
            return !values.empty();
        }

    public:
        // 一般成员函数

        /*
            获取形状信息
         */
        const std::vector<size_type> &getShape() const
        {
            return shape;
        }
        // 获取总大小
        size_type size() const
        {
            return values.size();
        }
    };

} // namespace linear

#endif