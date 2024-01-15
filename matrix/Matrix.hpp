#ifndef MATRIX_H
#define MATRIX_H 1
#include <concepts>
#include <string>
#include <vector>
// #define DEBUG
#define CAT << '|' <<
#define LOGMESSAGE(msg)                            \
	((std::stringstream()                          \
	  << __DATE__                                  \
			 CAT __TIME__                          \
				 CAT __FILE__                      \
					 CAT __FUNCTION__ CAT __LINE__ \
						 CAT msg)                  \
		 .str())
// template <typename T>
// concept ElemType = std::is_floating_point_v<T> || std::is_same_v<T, std::complex<float>> || std::is_same_v<T, std::complex<double>> || std::is_same_v<T, std::complex<long double>>;
template <typename T>
concept ElemType = requires(T t1, T t2) {
	{
		t1 + t2
	} -> std::same_as<T>;
	{
		t1 - t2
	} -> std::same_as<T>;
	{
		t1* t2
	} -> std::same_as<T>;
	{
		t1 / t2
	} -> std::same_as<T>;
	{
		std::cout << t1
	} -> std::same_as<std::ostream&>;
	t1 = 0;
	t1 = 1;
	t1 = -1;
};

// 定义父类MatrixBase
template <ElemType T>
class MatrixBase
{
public:
	using type = T;
	static constexpr const type ZERO = 0;

protected:
	size_t m_rows;
	size_t m_cols;
	std::vector<T> m_data;

public:
	// 构造函数
	MatrixBase(const MatrixBase& other)
	{
		m_rows = other.m_rows;
		m_cols = other.m_cols;
		m_data = other.m_data;
	}

	MatrixBase(const int& rows, const int& cols) : m_rows(rows), m_cols(cols)
	{
		if (rows > 0 && cols > 0)
			m_data.resize(rows * cols);
		else
			throw std::invalid_argument(LOGMESSAGE("rows > 0 ? cols > 0 ?"));
	}

	template <typename U, size_t r, size_t c>
		requires std::is_convertible_v<U, T>
	MatrixBase(const U(&arr)[r][c]) : MatrixBase(r, c)
	{
		decltype(r) i = 0;
		for (auto& line : arr)
			for (auto& v : line)
				m_data[i++] = v;
	}

	// 获取矩阵或行列式的行数和列数
	constexpr size_t rows() const { return m_rows; }
	constexpr size_t cols() const { return m_cols; }

	// 获取、设置矩阵或行列式指定行列的值
	const T& operator()(const size_t& rows, const size_t& cols) const { return m_data[rows * m_rows + cols]; }
	T& operator()(const size_t& rows, const size_t& cols) { return m_data[rows * m_rows + cols]; }

	// 矩阵转置
	MatrixBase transpose() const
	{
		MatrixBase<T> ret(m_cols, m_rows);
		auto& cur = *this;
		for (size_t i = 0; i < m_rows; ++i)
			for (size_t j = 0; j < m_cols; ++j)
				ret(j, i) = cur(i, j);
		return ret;
	}
	// 计算行列式的值
	T value() const
	{
		if (m_rows != m_cols)
			throw std::invalid_argument(LOGMESSAGE("it isn't a square matrix"));
		auto mat = *this;
		size_t i, j, cnt = 0;
		for (i = 0; i < m_rows; ++i)
		{
			for (j = i; j < m_rows; ++j)
				if (mat(j, i) != ZERO)
					break;
			if (j == m_rows)
				continue;
			if (i != j)
			{
				++cnt;
				for (size_t k = i; k < m_cols; ++k)
				{
					auto t = mat(i, k);
					mat(i, k) = mat(j, k);
					mat(j, k) = t;
				}
			}
			for (size_t k = i + 1; k < m_rows; ++k)
			{
				auto p = mat(k, i);
				if (p != ZERO)
				{
					mat(k, i) = ZERO;
					p /= mat(i, i);
					for (size_t m = i + 1; m < m_cols; ++m)
						mat(k, m) -= p * mat(i, m);
				}
			}
		}
		T result = cnt % 2 ? -1 : 1;
		for (i = 0; i < m_rows; ++i)
		{
			if (mat(i, i) != ZERO)
				result *= mat(i, i);
			else
				return 0;
		}
		return result;
	}
	
};

// 定义矩阵类Matrix，继承自MatrixBase
// template <typename T>
// class Matrix : public MatrixBase<T>
// {
// public:
//     // 构造函数
//     Matrix(size_t rows, size_t cols) : MatrixBase<T>(rows, cols) {}

//     // 矩阵加法
//     Matrix<T> operator+(const Matrix<T> &other) const
//     {
//         this->m_cols == other.m_cols;
//         this->m_rows == other.m_cols;
//         // return this;
//     };

//     // 矩阵减法
//     Matrix<T> operator-(const Matrix<T> &other) const;

//     // 矩阵乘法
//     Matrix<T> operator*(const Matrix<T> &other) const;
// };

// // 定义行列式类Determinant，继承自MatrixBase
template <typename T>
class Determinant : public MatrixBase<T>
{
public:
	// 构造函数
	Determinant(size_t n) : MatrixBase<T>(n, n) {}

	template <typename U>
		requires std::is_convertible_v<U, T>
	Determinant(const Determinant<U>& m)
	{
		if (m.rows() != m.cols())
			throw std::invalid_argument(LOGMESSAGE("it isn't a square matrix"));
		else
		{
			this->m_rows = m.rows();
			this->m_cols = m.cols();
			auto& d = this->m_data;
			for (size_t i = 0; i < this->m_rows; ++i)
				for (size_t j = 0; j < this->m_cols; ++j)
					d.push_back(m(i, j));
		};
	}
	Determinant(const MatrixBase<T>& m)
	{
		if (m.rows() != m.cols())
			throw std::invalid_argument(LOGMESSAGE("it isn't a square matrix"));
		else
		{
			this->m_rows = m.rows();
			this->m_cols = m.cols();
			auto& d = this->m_data;
			for (size_t i = 0; i < this->m_rows; ++i)
				for (size_t j = 0; j < this->m_cols; ++j)
					d.push_back(m(i, j));
		}
	}

	template <typename U, size_t N>
		requires std::is_convertible_v<U, T>
	Determinant(const U(&arr)[N][N]) : MatrixBase<T>(arr)
	{
	}

	template <typename K>
		requires std::is_convertible_v<K, T>
	Determinant& operator*=(const K& k)
	{
		for (size_t i = 0; i < this->m_cols; ++i)
			this->m_data[i] *= k;
		return *this;
	}

	// 代数余子式
	Determinant cofactor(const size_t& row, const size_t& col) const
	{
		auto&& r = this->rows() - 1;
		if (row <= r && col <= r)
		{
			Determinant<T> det(r);
			const auto& cur = *this;
			bool status1, status2;
			for (size_t i = 0, status1 = false; i < r; ++i)
			{
				if (i == row)
					status1 = true;
				for (size_t j = 0, status2 = false; j < r; ++j)
				{
					if (j == col)
						status2 = true;
					det(i, j) = cur(i + status1, j + status2);
				}
			}
			return det *= (row + col ? -1 : 1);
		}
		else
			throw std::invalid_argument(LOGMESSAGE("row or col? too large!"));
	}
};

template <typename T>
std::ostream& operator<<(std::ostream& _os_, const MatrixBase<T>& mat)
{
#include <sstream>
	std::stringstream ret;
	auto&& f = _os_.flags();
	ret.flags(f);
	ret << "{\n";
	auto&& r = mat.rows();
	auto&& c = mat.cols();
	for (size_t i = 0; i < r; i++)
	{
		decltype(ret) line;
		line.flags(f);
		for (size_t j = 0; j < c; j++)
			line << mat(i, j) << ' ';
		auto&& str = line.str();
		str.pop_back();
		ret << str << '\n';
	}
	ret << '}';
	return _os_ << ret.str();
}

#endif
