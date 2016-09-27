#include <arpa/inet.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/wait.h> 
#include <unistd.h>
#include <errno.h>

const char delimiters[] = "	 \n";
char *Authentication[11][3];

void tokenAuth(){
	FILE  *file;
	char *string, *found;

	file = fopen("Authentication.txt", "r");
	
	char line[256];
	int x = 0;

	while (fgets(line, sizeof(line), file)) {

    	string = strdup(line);

    	found = strtok (string, delimiters);
    	Authentication[x][0]=found;

    	found = strtok (NULL, delimiters);
    	Authentication[x][1]=found;

    	found = strtok (NULL, delimiters);
    	Authentication[x][2]=found;
    	
    	x++;
    }
    fclose(file);
}


int main(int argc, char *argv[]) {

	tokenAuth();

    /*for(int x = 0; x<11; x++){
    	printf("%s\n", "");
    	for(int y = 0; y<3; y++){
    		printf("%s\n", Authentication[x][y]);
    	}
    }*/

}