#define _GNU_SOURCE
#include <sys/types.h>
#include <sched.h>
#include <stdio.h>
#include <linux/unistd.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/resource.h>

int main(int argc, char **argv)
{
    pid_t pid = getpid();
    pid_t tid = (pid_t) syscall(SYS_gettid);

    printf("pid: %d\n", pid);
    printf("tid: %d\n", tid);
    printf("current priority = %d\n", getpriority(PRIO_PROCESS, 0));

    cpu_set_t set;
    CPU_ZERO(&set);
    if (sched_getaffinity(0, sizeof(cpu_set_t), &set) == 0)
    {
        for(int i = 0; i < CPU_SETSIZE; i++)
        {
            if(CPU_ISSET(i, &set))
                printf("using cpu: %d\n", i);
        }
    }
    else printf("sched_getaaffinity error");
}
