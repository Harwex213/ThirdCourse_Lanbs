#include <Windows.h>
#include <iostream>
#include <ctime>

using namespace std;

int main()
{
    for (int i = 2; i < 100000000; i++)
    {
        for (int j = 2; j < i; j++)
        {
            if (i % j == 0)
                break;
            else if (i == j + 1)
                cout << i << endl;

        }
    }
    return 0;
}