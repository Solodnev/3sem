#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

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
				char* command = strdup(pch);
				char* pch2 = command;
				int spaces = 0;
/*
 * вы не делали отдельную ф-ю Split, поэтому сейчас у вас "солянка" в коде.
 */
				while (*pch2 != '\0')
				{
					if (*pch2 == ' ')
					    ++spaces;
					++pch2;
				}
				char** args = (char**) calloc(spaces + 2, sizeof(*args));
				pch2 = command;
				char* start_command = pch2;
				int index = 0;
/*
 * вы фактически сами здесь реализуете функционал strtok'а.
 * всегда стараются использовать стандартную ф-ю, а не "изобретать свой велосипед", если он заведомо не лучше.
 * 
 * FIXIT:
 * вынесите в отдельную ф-ю все куски кода, которые отвечают за разбиение строки на отдельные слова (можете текущую реализацию оставить).
 */
				while (*pch2 != '\0')
				{
					if (*pch2 == ' ')
					{
						args[index] = start_command;
						++index;
						*pch2 = '\0';
						start_command = pch2 + 1;
					}
					++pch2;
				}
				args[index] = start_command;
				++index;
				args[index] = NULL;
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
