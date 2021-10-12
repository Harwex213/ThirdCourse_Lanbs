#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

void* thread1(void* arg)
{
	pid_t pid = getpid();
	for(int i = 1;i<75;i++)
	{
		sleep(1);
		printf("child %d-%d\n", pid, i);
	}
	pthread_exit("Child thread");
}


int main()
{
	 pthread_t a_thread;
	 void* thread_result;
	 pid_t pid = getpid();
	 printf("main: pid = %d \n", pid);
	 
	 int res =pthread_create(&a_thread, NULL, thread1, NULL);
	 
	 for(int i= 1;i<100;i++)
	 {
		 sleep(2);
		 printf("%d-%d \n", pid, i);
	 }
	 
	 int status = pthread_join(a_thread, (void**)&thread_result);
	 exit(0);
}
