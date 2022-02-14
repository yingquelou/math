#include <iostream>
#include <windows.h>
using namespace std;
int main(void)
/* int APIENTRY WinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) */
{
    MessageBox(nullptr, TEXT("Nice to meet you!"), TEXT("hello"), MB_OK);
    // cout << "Nice to meet you!" << endl;
    return 0;
}