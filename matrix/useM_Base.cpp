#include "M_Base.cpp"
int main(void)
{
    /*Matrix a{{1,9}, {1,8}};
    Matrix b(a);
    std::cout << a.GetInverseMatrix() << std::endl;*/
	Determinant c{ {1,2},
        {2,1} };
    std::cout << c.transpose() << std::endl;

    return 0;
}