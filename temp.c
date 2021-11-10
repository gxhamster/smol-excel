#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// field starts from 0
char *getfield(char *line, int num)
{
	char *token;
	char *copy = (char *)malloc(strlen(line) + 1);
	strcpy(copy, line);

	int count = 0;
	token = strtok(copy, ",");
	if (count == num)
		return token;

	// /usr/bin,/bin,/usr/sbin,/sbin

	while (token = strtok(NULL, ",")) {
		count++;
		if (count == num)
			return token;
	}
}

int main() 
{
	char *token;
	const char *path = "/usr/bin,/bin,/usr/sbin,/sbin";
	/* PATH is something like "/usr/bin:/bin:/usr/sbin:/sbin" */

	printf("%s\n", getfield(path, 2));
}
