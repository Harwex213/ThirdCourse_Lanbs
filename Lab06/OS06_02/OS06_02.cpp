#include <iostream>
#include "windows.h"
using namespace std;

int main()
{
	for (int i = 0; i < 10000000; i++)
	{
		cout << i << endl;
		Sleep(200);
	}
}