#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
char details [2];

int main(int argc, char **argv) {
	const char *file = "authentication.txt";
	const char *delimiter_characters = ",";
	FILE *input_file = fopen(file, "r");
	char buffer[ BUFFER_SIZE ];
	char *last_token;

	if (input_file == NULL) {
	
		fprintf(stderr, "No file can be displayed", file);
	}
	else {
		while(fgets(buffer, BUFFER_SIZE, input_file) != NULL) {
			last_token = strtok (buffer, delimiter_characters);
			while (last_token != NULL ){
				printf("%s\n", last_token);
				last_token = strok (NULL, delimiter_characters);
			}
		}

	
	fclose(input_file);
	
	}

	return 0;
				
	
}
