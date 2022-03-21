#include "M_Base.h"
M_Base::M_Base()
{
	pB = nullptr;
	status = false;
}
M_Base::~M_Base()
{
	pB = nullptr;
	status = false;
}
M_Base::M_Base(const size_t& n, const bool& TF) {
	M_Base::rows_type rows(n, 0);
	if (n&&(pB = std::make_unique<std::vector<M_Base::rows_type>>(std::vector<M_Base::rows_type>(n, rows))))
	{
		status = true;
		if (TF)
		{
			M_Base& ret = *this;
			for (size_t i = 0; i < n; ++i)
				ret[i][i] = 1;
		}
	}
	else
		this->~M_Base();
}
M_Base::M_Base(const M_Base& mb)
{
	if (mb && (pB = std::make_unique<std::vector<rows_type>>(*(mb.pB))))
		status = true;
	else
		this->~M_Base();
}
M_Base& M_Base::operator=(const M_Base mb)
{
	if (mb && (pB = std::make_unique<std::vector<rows_type>>(*(mb.pB))))
	{
		status = true;
		return *this;
	}
	return *this;
}
M_Base::rows_type& M_Base::operator[](const size_t& n)
{
	if (*this && (pB->size() > n))
		return pB->at(n);
	else
	{
		throw std::runtime_error("Object or Subscript is incorret!");
	}
}
const M_Base::rows_type& M_Base::operator[](const size_t& n) const
{
	if (*this && (pB->size() > n))
		return pB->at(n);
	else
		throw std::runtime_error("Object or Subscript is incorret!--const");
}
M_Base::operator bool() const { return status; }
size_t M_Base::GetRows() const
{
	if (status)
		return pB->size();
	else
		return 0;
}
size_t M_Base::GetColumn() const
{
	if (GetRows())
		return pB->cbegin()->size();
	else
		return 0;
}
M_Base M_Base::transpose()const{
	const auto&& c = GetColumn(), && r = GetRows();
	if (c)
	{
		M_Base ret(1);
		const auto& ma = *this;
		ret.pB->clear();
		for (size_t i = 0; i < c; ++i)
		{
			M_Base::rows_type rr;
			for (size_t j = 0; j < r; ++j)
			{
				rr.push_back(ma[j][i]);
			}
			ret.pB->push_back(rr);
		}
		return ret;
	}
	else return M_Base();
}
M_Base::M_Base(const std::initializer_list<rows_type>& il)
{
	if (il.size())
	{
		auto c = il.begin()->size();
		status = true;
		for (auto i = il.begin() + 1; i != il.end(); ++i)
			if (i->size() != c)
			{
				status = false;
				pB = nullptr;
				break;
			}
		if (status)
			pB = std::make_unique<std::vector<rows_type>>(il);
		else
			goto gt;
	}
	else
	{
	gt:
		pB = nullptr;
		status = false;
	}
}
std::ostream& operator<<(std::ostream& out, const M_Base& mb)
{
	if (!mb)
		return out;
	auto r = mb.pB->size(), c = mb.pB->begin()->size();
	for (size_t i = 0; i < r; ++i)
	{
		for (size_t j = 0; j < c; ++j)
		{
			out << mb.pB->at(i).at(j);
			if (j != c - 1)
				out << ',';
		}
		if (i != r - 1)
			out << std::endl;
	}
	return out;
}
Matrix& Matrix::operator+=(const Matrix mat)
{
	auto p = GetRows(), q = GetColumn();
	if (q && mat && mat.GetRows() == q && mat.GetColumn() == p)
		for (decltype(p) i = 0; i < p; ++i)
			for (decltype(p) j = 0; j < q; ++j)
				(*this)[i][j] += mat[i][j];
	return *this;
}
Matrix Matrix::operator+(const Matrix& mat) const
{
	Matrix ret(mat);
	return ret += mat;
}
M_Base::iterator M_Base::begin()
{
	if (GetColumn())
		return pB->begin();
	else
		return pB->end();
}
M_Base::iterator M_Base::end()
{
	/*if (GetColumn())
		return  pB->begin();
	else*/
	return pB->end();
}
Matrix Matrix::operator*(const Matrix::value_type& k) const
{
	auto c = GetColumn();
	if (c && !k)
	{
		Matrix ret(*this);
		for (auto&& i : ret)
			for (auto&& j : i)
				j *= k;
		return ret;
	}
	else
		return *this;
}
Matrix& Matrix::operator-=(const Matrix& mat)
{
	return *this += mat * -1;
}
Matrix& Matrix::operator*=(const Matrix mat)
{
	size_t r = GetRows(), c, n;
	// r c c n
	if ((c = GetColumn()) && (c == mat.GetRows()) && (n = mat.GetColumn()))
	{
		Matrix bridge(*this), & ret = *this;
		Matrix::value_type tmp;
		size_t i, j, k;
		for (i = 0; i < r; ++i)
		{
			for (j = 0; j < n; ++j)
			{
				for (k = 0, tmp = 0; k < c; ++k)
				{
					tmp += bridge[i][k] * mat[k][j];
				}
				ret[i][j] = tmp;
			}
		}
	}
	return *this;
}
Matrix Matrix::operator*(const Matrix& mat) const
{
	Matrix ret(*this);
	return ret *= mat;
}
Matrix operator*(const Matrix::value_type& k, const Matrix& mat)
{
	return mat * k;
}
Matrix& Matrix::operator&=(const Matrix& mat)
{
	auto c = GetColumn(), r = GetRows(), mr = mat.GetRows(), mc = mat.GetColumn();
	if (r && mr == r && mc)
	{
		Matrix& ret = *this;
		for (size_t i = 0; i < r; ++i)
			for (auto&& j : mat[i])
				ret[i].push_back(j);
		return ret;
	}
	else return *this;
}
Matrix& Matrix::LineExchange(const size_t& i, const size_t& j)
{
	auto c = GetColumn(), r = GetRows();
	if (c && i != j && i < c && j < c)
	{
		auto& ret = *this;
		Matrix::rows_type tmp = ret[i];
		ret[i] = ret[j];
		ret[j] = tmp;
		return ret;
	}
	else return *this;
}
Determinant& Determinant::lineSwap(const size_t& i, const size_t& j)
{
	auto c = GetColumn(), r = GetRows();
	if (c && i != j && i < c && j < c)
	{
		auto& ret = *this;
		Determinant::rows_type zeroLine(c, 0);
		auto &&cs = (ret[i] != zeroLine) + (ret[j] != zeroLine);
		switch(cs)
		{
		case 2:
			for (auto& val : ret[i])
				val *= -1;
		case 1:
			std::swap(ret[i], ret[j]);
		}
		return ret;
	}
	else return *this;
}
Matrix Matrix::LineExchange(const size_t& i, const size_t& j) const
{
	Matrix ret(*this);
	return ret.LineExchange(i, j);
}
Matrix& Matrix::RSFM_REM(const bool& TF)
{
	const auto r = GetRows(), c = GetColumn();
	Matrix result(*this);
	size_t SwapPos = r - 1;
	auto NotZeroLog = new int[r];
	size_t ZeroLine = 0;
	for (size_t i = 0; i < r;)
	{
		size_t j = 0;
		while (j < c && !result[i][j])
			++j;
		if (j < c)
		{
			NotZeroLog[i] = j;
			auto times = result[i][j];
			result[i][j] = 1;
			for (size_t k = j + 1; k < c; ++k)
			{
				result[i][k] /= times;
			}
			if (TF)
				for (size_t u = 0; u < i; ++u)
				{
					if (times = result[u][j])
						for (size_t x = j; x < c; ++x)
						{
							result[u][x] -= times * result[i][x];
						}
				}
			for (size_t d = i + 1; d < r; ++d)
			{
				if (times = result[d][j])
					for (size_t x = j; x < c; ++x)
					{
						result[d][x] -= times * result[i][x];
					}
			}
			++i;
		}
		else if (i < SwapPos)
		{
			result.LineExchange(i, SwapPos);
			NotZeroLog[SwapPos] = -1;
			++ZeroLine;
			--SwapPos;
		}
		else
		{
			++ZeroLine;
			NotZeroLog[i] = -1;
			break;
		}
	}
	std::map<int, size_t> ma;
	for (size_t i = 0; i < r; ++i)
		ma[NotZeroLog[i]] = i;
	Matrix& ret = *this;
	/* for (auto &i : ma)
		ret.push_back(result[i.second]); */
	{
		size_t j = 0;
		for_each(ma.cbegin(), ma.cend(), [&](decltype(ma)::value_type it)
			{if (it.first >= 0) { ret[j] = result[it.second]; ++j; } }); }
		Matrix::rows_type tmp(c, 0);
		for (size_t i = r - 1; ZeroLine; --i, --ZeroLine)
			ret[i] = tmp;
		delete[] NotZeroLog;
		NotZeroLog = nullptr;
		// ret.shrink_to_fit();
		return ret;
	
}
Matrix& Matrix::RowSimplestFormOfMatrix(void)
{
	return RSFM_REM(true);
}
Matrix& Matrix::RowEchelonMatrix(void)
{
	return RSFM_REM(false);
}
size_t Matrix::RankOfMatrix(void) const
{
	if (!GetColumn())
		return 0;
	Matrix mat(*this);
	mat.RSFM_REM(false);
	size_t ret = 0, r = GetRows(), c = GetColumn();
	auto tmp = Matrix::rows_type(c, 0.0);
	for (size_t i = r - 1; mat[i] == tmp; --i)
		++ret;
	return r - ret;
}
Matrix& Matrix::LineMul(const size_t& i, const Matrix::value_type& k)
{
	auto r = GetRows(), c = GetColumn();
	if (c && i < r && k != 1)
	{
		Matrix& ret = *this;
		for (auto& val : ret[i])
			val *= k;
		return ret;
	}
	else return *this;
}
Matrix Matrix::LineMul(const size_t&i, const Matrix::value_type&k)const {
	Matrix ret(*this);
	return ret.LineMul(i,k);
}
Matrix& Matrix::LineMulToLine(const size_t& i, const Matrix::value_type& k, const size_t& j) {
	auto r = GetRows(), c = GetColumn();
	if (c && i < r && k && j < r)
	{
		Matrix& ret = *this;
		Matrix::rows_type ik(ret[i]);
		for (size_t t = 0; t < c; ++t)
			ret[j][t] += ik[t] * k;
		return ret;
	}
	else return *this;
}
Matrix Matrix::LineMulToLine(const size_t&i, const Matrix::value_type&k, const size_t&j)const {
	Matrix ret(*this);
	return ret.LineMulToLine(i,k,j);
}
Matrix Matrix::GetInverseMatrix(void) const
{
	const size_t &&c = GetColumn(),&&r=GetRows();
	if (c == r && c == RankOfMatrix())
	{
		Matrix UM(r,true);
		Matrix ret(*this);
		ret &= UM;
		ret.RowSimplestFormOfMatrix();
		for (size_t i = 0; i < c; ++i)
			ret[i].erase(ret[i].begin(), ret[i].begin() + c);
		return ret;
	}
	else
		return Matrix();
}
bool Determinant::RSFM_REM()
{
	const auto r = GetRows(), c = GetColumn();
	Determinant& result = *this;
	size_t SwapPos = r - 1;
	auto NotZeroLog = new int[r];
	size_t ZeroLine = 0;
	for (size_t i = 0; i < r;)
	{
		size_t j = 0;
		while (j < c && !result[i][j])
			++j;
		if (j < c)
		{
			NotZeroLog[i] = j;
			for (size_t d = i + 1; d < r; ++d)
			{
				if (auto &&times = result[d][j] / result[i][j])
					for (size_t x = j; x < c; ++x)
						result[d][x] -= times * result[i][x];
			}
			++i;
		}
		else if (i < SwapPos)
		{
			result.lineSwap(i, SwapPos);
			NotZeroLog[SwapPos] = c + ZeroLine;
			++ZeroLine;
			--SwapPos;
		}
		else
		{
			NotZeroLog[i] = c + ZeroLine;
			++ZeroLine;
			break;
		}
	}
	if (ZeroLine)
		return false;
	std::map<size_t, size_t> ma;
	for (size_t i = 0; i < r; ++i)
		ma[NotZeroLog[i]] = i;
	/* for (auto &i : ma)
		ret.push_back(result[i.second]); */
	Determinant t(result);
	size_t j = 0;
	for_each(ma.cbegin(), ma.cend(), [&](const decltype(ma)::value_type& it)
		{if (it.first < c) { result[j] = t[it.second]; ++j; } });
	size_t count = 0;
	for (size_t m = 0; m < r-1; ++m)
		for (size_t n = m + 1;NotZeroLog[m] > NotZeroLog[n]; ++n)
			++count;
	if (count % 2)
		for (auto& val : result[0])
			val *= -1;
	delete[] NotZeroLog;
	NotZeroLog = nullptr;
	// ret.shrink_to_fit();
	return true;
}
//bool Determinant::RSFM_REM()const
//{
//	Determinant ret(*this);
//	return ret.RSFM_REM();
//}
Determinant::value_type Determinant::operator+(const Determinant&Dt)const {
	Determinant left(*this),right(Dt);
	return left() + right();
}
Determinant::value_type Determinant::operator()()const
{
	Determinant tmp(*this);
	size_t c = GetColumn();
	if (c && c == GetColumn() && tmp.RSFM_REM())
	{
		Determinant::value_type ret = 1;
		for (size_t i = 0; i < c; ++i)
			ret *= tmp[i][i];
		return ret;
	}
	else return 0;
}