#include "Matrix.h"
#include <fstream>
#include <sstream>
int main(int argc, char* argv[])
{
	auto& os = std::cout;
	Matrix a{ {1, 2}, {4, 5} }, b{ {100, 10}, {10, 100} }, c{ {1, 2}, {2, 5}, {3, 7} };
	os << a << '\t' << a.getRow() << '\t' << a.getColumn() << '\n';
	os << b << '\t' << b.getRow() << '\t' << b.getColumn() << '\n';
	os << c << '\t' << c.getRow() << '\t' << c.getColumn() << '\n';

	os << Matrix::AssignValuesRandomly() << '\n';
	os << a.getInverseMatrix() << '\n';
	os << a.leftMulUnitMatrix() << '\n';
	os << a.rightMulUnitMatrix() << '\n';
	os << a.lineExchange(0, 1) << '\n';
	os << a.lineMul(0, 0.5) << '\n';
	os << a.lineMulToLine(0, 2, 1) << '\n';

	os << a.standardShape() << '\n';

	if (a)
		os << "OK\n";

	os << c.rankOfMatrix() << '\n';
	os << c.rowSimplestForm() << '\n';
	os << c.transpose() << '\n';

	os << a + b << '\n';
	os << a - b << '\n';
	os << a * b << '\n';
	os << a * 0.5 << '\n';
	os << 0.5 * a << '\n';
	os << std::boolalpha << (a == b) << '\n';
	os << (a &= b) << '\n';
	return 0;
}