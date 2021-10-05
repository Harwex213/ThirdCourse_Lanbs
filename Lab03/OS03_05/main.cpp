#include <iostream>
#include <unistd.h>
#include <wait.h>

using namespace std;

void cycle(int count, char const* name, pid_t pid){
    for(int i = 0; i < count; i++){
        std::cout << "i:" << i << " " << name << ", pid:" << pid <<  std::endl;
        sleep(1);
    }
}

int main() {
    pid_t pid;
    switch (pid = fork()) {
        case -1:
            perror("fork error"); exit(-1);
        case 0:
            cycle(100, "child", getpid());
            exit(0);
        default:
            cycle(100, "main", getpid());
            wait(nullptr);
    }
    return 0;
}