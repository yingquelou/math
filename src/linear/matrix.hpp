#ifndef MATRIX_HPP
#define MATRIX_HPP 1
#include "array.hpp"
#include <string>
namespace linear
{
	// 矩阵类
	template <typename T, typename SizeType = std::size_t>
	class matrix : public array<T, SizeType>
	{
	public:
		// 类型定义
		using super_type = array<T, SizeType>;
		using value_type = typename super_type::value_type;
		using size_type = typename super_type::size_type;

	public:
		// 构造与析构
		~matrix() = default;
		using super_type::super_type;
		// 列表初始化
		matrix(const std::initializer_list<std::initializer_list<value_type>> &list) : super_type([&list]()
																								  {
			std::vector<size_type> shape = {list.size(), list.begin()->size()};
			matrix<T, SizeType> mat(shape);
			size_type i = 0;
			for (const auto &row : list)
			{
				size_type j = 0;
				for (const auto &val : row)
				{
					mat(i, j) = val;
					++j;
				}
				++i;
			}
			return mat; }()) {}

		// 行列数初始化
		matrix(const size_type &rows, const size_type &cols) : super_type({rows, cols}) {}

	public:
		size_type getRow() const
		{
			auto shape = this->getShape();
			return shape.size() >= 1 ? shape[0] : 0;
		}
		size_type getColumn() const
		{
			auto shape = this->getShape();
			return shape.size() >= 2 ? shape[1] : 0;
		}
		// 矩阵的乘法运算
		matrix &operator*=(const matrix &mat)
		{
			if (this->getColumn() != mat.getRow())
				throw std::invalid_argument("Matrix multiplication dimension mismatch");
			matrix<T, SizeType> result(this->getRow(), mat.getColumn());
			for (size_type i = 0; i < this->getRow(); ++i)
				for (size_type j = 0; j < mat.getColumn(); ++j)
					for (size_type k = 0; k < this->getColumn(); ++k)
						result(i, j) += (*this)(i, k) * mat(k, j);
			*this = std::move(result);
			return *this;
		}
		// 矩阵的乘法运算
		matrix operator*(const matrix &mat) const
		{
			matrix<T, SizeType> result(*this);
			result *= mat;
			return result;
		}
		// 矩阵的标量乘法运算
		matrix &operator*=(const T &scalar)
		{
			for (auto &val : this->values)
				val *= scalar;
			return *this;
		}
		// 矩阵的标量乘法运算
		matrix operator*(const T &scalar) const
		{
			matrix<T, SizeType> result(*this);
			result *= scalar;
			return result;
		}
		// 矩阵的加法运算
		matrix &operator+=(const matrix &mat)
		{
			if (this->getRow() != mat.getRow() || this->getColumn() != mat.getColumn())
				throw std::invalid_argument("Matrix addition dimension mismatch");
			for (size_type i = 0; i < this->getRow(); ++i)
				for (size_type j = 0; j < this->getColumn(); ++j)
					(*this)(i, j) += mat(i, j);
			return *this;
		}
		// 矩阵的加法运算
		matrix operator+(const matrix &mat) const
		{
			matrix<T, SizeType> result(*this);
			result += mat;
			return result;
		}
		// 矩阵的转置运算
		matrix transpose() const
		{
			matrix<T, SizeType> result({this->getColumn(), this->getRow()});
			for (size_type i = 0; i < this->getRow(); ++i)
				for (size_type j = 0; j < this->getColumn(); ++j)
					result(j, i) = (*this)(i, j);
			return result;
		}
		// 矩阵的访问运算
		value_type &operator()(size_type row, size_type col)
		{
			return static_cast<super_type *>(this)->operator()({row, col});
		}
		const value_type &operator()(size_type row, size_type col) const
		{
			return static_cast<const super_type *>(this)->operator()({row, col});
		}
		// 行交换
		matrix &lineExchange(const size_type &i, const size_type &j)
		{
			if (i >= getRow() || j >= getRow())
				throw std::out_of_range("Row index out of range");
			for (size_type col = 0; col < getColumn(); ++col)
				std::swap((*this)(i, col), (*this)(j, col));
			return *this;
		}
		// 初等变换——某行倍增
		matrix &lineMul(const size_type &i, const value_type &k)
		{
			if (i >= getRow())
				throw std::out_of_range("Row index out of range");
			for (size_type col = 0; col < getColumn(); ++col)
				(*this)(i, col) *= k;
			return *this;
		}
		// 初等变换——某行倍增所得加到某行上去
		matrix &lineMulToLine(const size_type &i, const value_type &k, const size_type &j)
		{
			if (i >= getRow() || j >= getRow())
				throw std::out_of_range("Row index out of range");
			for (size_type col = 0; col < getColumn(); ++col)
				(*this)(j, col) += (*this)(i, col) * k;
			return *this;
		}
		// 求可左乘当前矩阵的单位矩阵
		matrix leftMulUnitMatrix() const
		{
			matrix<T, SizeType> unitMat(getRow(), getRow());
			for (size_type i = 0; i < getRow(); ++i)
				unitMat(i, i) = 1;
			return unitMat;
		}
		// 求可右乘当前矩阵的单位矩阵
		matrix rightMulUnitMatrix() const
		{
			matrix<T, SizeType> unitMat(getColumn(), getColumn());
			for (size_type i = 0; i < getColumn(); ++i)
				unitMat(i, i) = 1;
			return unitMat;
		}
		// 行最简形矩阵
		matrix &rowEchelonForm()
		{
			size_type lead = 0,column=getColumn(),row=getRow();
			for (size_type r = 0; r < row; ++r)
			{
				if (lead >= column)
					return *this;
				size_type i = r;
				while (this->operator()(i, lead) == 0)
				{
					++i;
					if (i == row)
					{
						i = r;
						++lead;
						if (lead == column)
							return *this;
					}
				}
				lineExchange(i, r);
				value_type lv = this->operator()(r, lead);
				lineMul(r, 1 / lv);
				for (i = 0; i < row; ++i)
				{
					if (i != r)
					{
						lv = this->operator()(i, lead);
						lineMulToLine(r, -lv, i);
					}
				}
				++lead;
			}
			return *this;
		}
		// 行最简形矩阵
		matrix rowEchelonForm() const
		{
			matrix<T, SizeType> result(*this);
			result.rowEchelonForm();
			return result;
		}
		// 标准形矩阵
		matrix &standardForm()
		{
			rowEchelonForm();
			for (size_type i = 0; i < getRow(); ++i)
			{
				size_type lead = 0;
				while (lead < getColumn() && this->operator()({i, lead}) != 1)
					++lead;
				if (lead == getColumn())
					continue;
				for (size_type j = 0; j < getRow(); ++j)
				{
					if (j != i)
					{
						value_type lv = this->operator()({j, lead});
						lineMulToLine(i, -lv, j);
					}
				}
			}
			return *this;
		}
		// 标准形矩阵
		matrix standardForm() const
		{
			matrix<T, SizeType> result(*this);
			result.standardForm();
			return result;
		}
		// 矩阵的秩
		size_type rank() const
		{
			matrix<T, SizeType> rrefMat = this->rowEchelonForm();
			size_type rank = 0;
			for (size_type i = 0; i < rrefMat.getRow(); ++i)
			{
				bool nonZeroRow = false;
				for (size_type j = 0; j < rrefMat.getColumn(); ++j)
				{
					if (rrefMat(i, j) != 0)
					{
						nonZeroRow = true;
						break;
					}
				}
				if (nonZeroRow)
					++rank;
			}
			return rank;
		}
		// 求逆
		matrix inverse() const
		{
			if (getRow() != getColumn())
				throw std::invalid_argument("Only square matrices can be inverted");
			matrix<T, SizeType> augmentedMat(getRow(), getColumn() * 2);
			for (size_type i = 0; i < getRow(); ++i)
				for (size_type j = 0; j < getColumn(); ++j)
				{
					augmentedMat(i, j) = (*this)(i, j);
					augmentedMat(i, j + getColumn()) = (i == j) ? 1 : 0;
				}
			augmentedMat.rowEchelonForm();
			for (size_type i = 0; i < getRow(); ++i)
			{
				value_type diagElem = augmentedMat(i, i);
				if (diagElem == 0)
					throw std::runtime_error("Matrix is singular and cannot be inverted");
				for (size_type j = 0; j < augmentedMat.getColumn(); ++j)
					augmentedMat(i, j) /= diagElem;
				for (size_type k = 0; k < getRow(); ++k)
				{
					if (k != i)
					{
						value_type factor = augmentedMat(k, i);
						for (size_type j = 0; j < augmentedMat.getColumn(); ++j)
							augmentedMat(k, j) -= factor * augmentedMat(i, j);
					}
				}
			}
			matrix<T, SizeType> invMat(getRow(), getColumn());
			for (size_type i = 0; i < getRow(); ++i)
				for (size_type j = 0; j < getColumn(); ++j)
					invMat(i, j) = augmentedMat(i, j + getColumn());
			return invMat;
		}
		// 将矩阵的内容储存到一个串里
		std::string toString() const
		{
			std::string result;
			for (size_type i = 0; i < getRow(); ++i)
			{
				result += "[ ";
				for (size_type j = 0; j < getColumn(); ++j)
				{
					result += std::to_string((*this)(i, j)) + " ";
				}
				result += "]\n";
			}
			return result;
		}
		// 返回当前矩阵对应的LeTex源码
		std::string toLaTeX() const
		{
			std::string result = "\\begin{bmatrix}\n";
			for (size_type i = 0; i < getRow(); ++i)
			{
				for (size_type j = 0; j < getColumn(); ++j)
				{
					result += std::to_string((*this)(i, j));
					if (j < getColumn() - 1)
						result += " & ";
				}
				if (i < getRow() - 1)
					result += " \\\\\n";
			}
			result += "\n\\end{bmatrix}";
			return result;
		}
		// 流
		friend std::ostream &operator<<(std::ostream &os, const matrix &mat)
		{
			os << mat.toString();
			return os;
		}
	};

} // namespace linear
#endif