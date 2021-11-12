#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


#define KB (1024)
#define MB (KB*KB)
#define SIZE (256 * MB)


int main() {
    void * mem = malloc(SIZE);
    int * arr = (int*)mem;
    printf("pid: %d\n", getpid());
    printf("malloc\n");
    for(int i = 0; i < SIZE/ sizeof(int); i++, arr++){
        *arr = i;
    }
    printf("pause\n");
    sleep(3600);

    printf("free\n");
    free(mem);
}
