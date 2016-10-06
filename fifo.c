#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


int main(int argc, char **argv)
{
	int result = 0;
	int i = 0;
  	(void)umask(0); 
  	int userIndex = 0;
  	char str[200];
	const char* fifos[] = {"first.fifo", "second.fifo"};
	for(i = 0; i!=2; i++)
	{
		if(mknod(fifos[i], S_IFIFO | 0666, 0) < 0)
		{	
			if (errno !=EEXIST)
			{	
				printf("Can\'t create FIFO\n");
				exit(-1);
			}
		} 	
	}	
	userIndex = atoi(argv[1]);
	int fdRead, fdWrite;
	if (userIndex == 0)
	{
		fdRead = open(fifos[0], O_RDONLY);
		fdWrite = open(fifos[1], O_WRONLY);
	} else
	{
		fdWrite = open(fifos[0], O_WRONLY);
		fdRead = open(fifos[1], O_RDONLY);
	}
	printf("fdRead = %d \n fdWrite = %d",fdRead, fdWrite);
	result = fork();
	if (result > 0)
	{
		while (10)
		{
			fgets(str, 200, stdin);
			int bytesWritten = 
				write(fdWrite, str, strlen(str) + 1);
			printf("written %d\n", bytesWritten);
		}
//		close (fdWrite);
//		printf("Parent exit\n");
	}
	else
	{
		while(10);
		{
			int bytesRead = read(fdRead, str, 200);
			printf("read %d\n", bytesRead);
			printf("%s", str);
		}
//		close(fdRead);
	}
		return 0;
}


