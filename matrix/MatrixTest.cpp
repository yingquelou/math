#include <iostream>
#include <iomanip>
#include <complex>
#include "Matrix.hpp"
int main(int argc, char* argv[])
{
	// std::ofstream os("aaa.txt");
	auto& os = std::cout;
	int arr[][3] = { {246, 427, 327},
					{1014, 543, 443},
					{-342, 721, 621} };
	Determinant<float>  m(arr);
	os << m.value();
	return 0;

}