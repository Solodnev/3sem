#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>


int main(int argc, char **argv)
{	
	int i;
	int N = 5;
	for (i = 0; i < N; ++i){
		pid_t pid = fork ();
		if (pid == 0) {
			printf("I am a child and my PID is: %d\n", getpid());
			sleep(i + 1);
			printf("%d\n", i);
		} if (pid != 0) {
			printf("I am a parent of %d, my PID is %d and I am waiting for a child\n", pid, getpid());
			int status = 0;
			waitpid(pid, &status, 0);
			printf("I am a parent of %d, my PID is %d and my child has finished\n", pid, getpid());
			exit(0);
		}
	}
	return 0;
}

