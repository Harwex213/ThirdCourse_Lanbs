#include <iostream>
#include <windows.h>
using namespace std;

int main()
{
    int const KB = 1024;
    int const MB = KB * KB;
    int const SIZE_PAGE = 4 * KB;
    int const QUANTITY_PAGES = 256;
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    LPVOID xmemaddr = VirtualAlloc(nullptr, QUANTITY_PAGES * SIZE_PAGE, MEM_COMMIT, PAGE_READWRITE);
    cout << "size: " << QUANTITY_PAGES * SIZE_PAGE / MB << " MB" << endl;
    cout << "size in hex: " <<  hex << xmemaddr << endl;

    int* arrayint = (int*) xmemaddr;

    if (arrayint == nullptr) {
        cout << "ERROR: VirtualAlloc(...)";
        exit(-1);
    }

    for (int i = 0; i < QUANTITY_PAGES * SIZE_PAGE / sizeof(int); i++)
    {
        *arrayint = i;
        arrayint++;
    }

    // К А П      К = CA; А = C0; П = CF;

    int page = 0xCA;
    int offset = 0xC0C;

    int* number = (int*)(((int*)xmemaddr + (SIZE_PAGE)*page / sizeof(int) + offset / sizeof(int)));
    cout << "" << *number << endl;
    system("pause");
}

