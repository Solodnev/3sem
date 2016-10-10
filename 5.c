#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

char** split(char* str);
int read_file(const char* filename, char** buf_addr, int* len);

int main(int argc, char **argv)
{
	char* file_content;
	int file_len = 0;
	read_file("commands.txt", &file_content, &file_len);
	
	int is_odd = 1;
	int timing = 0;

	char* pch = strtok(file_content, "\n");
    while (pch != NULL)
    {
		if (is_odd)
		{
			timing = atoi(pch);
			is_odd = 0;
		} else
		{
			pid_t pid = fork();
			if (pid == 0)
			{
				char** args = split(pch);
				printf("Wait '%d'\n", timing);
				sleep(timing);
				printf("I am running program '%s'\n", args[0]);
				execvp(args[0], args);
				exit(0);
			}
			is_odd = 1;
		}
        pch = strtok(NULL, "\n");
	}

	free(file_content);
	return 0;
}

char** split(char* str)
{
	char* string = strdup(str);
	char* pch = string;
	int spaces = 0;
	while (*pch != '\0')
	{
		if (*pch == ' ')
			++spaces;
		++pch;
	}
	char** result = (char**) calloc(spaces + 2, sizeof(*result));
	pch = string;
	char* start_string = pch;
	int index = 0;
	while (*pch != '\0')
	{
		if (*pch == ' ')
		{
			result[index] = start_string;
			++index;
			*pch = '\0';
			start_string = pch + 1;
		}
		++pch;
	}
	result[index] = start_string;
	++index;
	result[index] = NULL;
	return result;
}

int read_file(const char* filename, char** buf_addr, int* len)
{
    FILE* stream = fopen(filename, "rb");
    if (!stream)
        return 1;
    if (fseek(stream, 0, SEEK_END))
        return 2;

    *len = ftell(stream);
    rewind(stream);
    char* buffer = (char*) calloc(*len, sizeof(*buffer));
    fread(buffer, *len, sizeof(*buffer), stream);
    fclose(stream);
    *buf_addr = buffer;
    return 0;
}

