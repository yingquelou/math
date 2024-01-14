test:test.exe
	./test.exe
test.exe:MatrixTest.cpp
	g++ -std=c++23 MatrixTest.cpp -o test.exe