#include <iostream>
#include <ctime>

using namespace std;

int main()
{
    int i = 0;
    clock_t t1 = clock();
    clock_t t2 = 0;
    while (true)
    {
        i++;
        t2 = clock();
        if ((t2 - t1) == 5000 || (t2 - t1) == 10000)
        {
            cout << "number: " << i << endl;
        }
        if ((t2 - t1) == 15000)
            break;
    }
    cout << "number: " << i << endl;
}