#include <iostream>
#include <unistd.h>

using namespace std;

int main() {
    pid_t pid = getpid();

    for(int i = 0; i < 125; i++ ){
        cout << "PID: " << pid << endl;
        sleep(1);
    }
    return 0;
}