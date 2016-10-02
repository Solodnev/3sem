#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void GenerateString (int n, char* string);

int main()
{
	int count = 0;
	scanf("%d", &count);
	printf ("\n");
	char *string = malloc ((1 << (count + 1))*sizeof(char));
	GenerateString (count, string);
	free(string);
	return 0;
}

void GenerateString (int n, char* string)
{
	int i = 0;
	int lenght = 0;
	string[0] = 'a';
	for (;i < n; i++)
	{
		printf ("%s\n", string);
		lenght = strlen(string);
		string[lenght] = ('a' + i + 1);
		strncpy (string + lenght + 1, string ,lenght);
		string[2*lenght+1] = '\0';
	}	
	
	printf ("\n");
}


