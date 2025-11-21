#include <windows.h>
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nShowCmd)
{
    MessageBox(nullptr, TEXT("Nice to meet you!"), TEXT("hello"), MB_OK);
    return 0;
}