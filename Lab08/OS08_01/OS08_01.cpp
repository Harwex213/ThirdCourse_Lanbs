#include <iostream>
#include <ctime>

using namespace std;

int main()
{
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);

    cout << "Current local date and time : " << ltm.tm_mday << "." << ltm.tm_mon + 1 << "." << ltm.tm_year + 1900 << " " << ltm.tm_hour << ":" << ltm.tm_min << ":" << ltm.tm_sec << endl;
}