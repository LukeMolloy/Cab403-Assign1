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

const char delimiters[] = "	 \n,";
char *Authentication[11][3];
char *Accounts[25][3];
char *ClientDetails[][6];
char *Transactions[1][4];

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

void tokenAccount(){
    FILE  *file;
    char *string, *found;

    file = fopen("Accounts.txt", "r");
    
    char line[256];
    int x = 0;

    while (fgets(line, sizeof(line), file)) {

        string = strdup(line);

        found = strtok (string, delimiters);
        Accounts[x][0]=found;

        found = strtok (NULL, delimiters);
        Accounts[x][1]=found;

        found = strtok (NULL, delimiters);
        Accounts[x][2]=found;
        
        x++;
    }
    fclose(file);
}

void tokenClient(){
    FILE  *file;
    char *string, *found;

    file = fopen("Client_Details.txt", "r");
    
    char line[256];
    int x = 0;

    while (fgets(line, sizeof(line), file)) {

        string = strdup(line);

        found = strtok (string, delimiters);
        ClientDetails[x][0]=found;

        found = strtok (NULL, delimiters);
        ClientDetails[x][1]=found;

        found = strtok (NULL, delimiters);
        ClientDetails[x][2]=found;

        found = strtok (NULL, delimiters);
        ClientDetails[x][3]=found;

        found = strtok (NULL, delimiters);
        if(found == NULL){
            ClientDetails[x][4] = " ";
        }else{
            ClientDetails[x][4]=found;
        }

        found = strtok (NULL, delimiters);
        if(found == NULL){
            ClientDetails[x][5] = " ";
        }else{
            ClientDetails[x][5]=found;
        }
        
        x++;
    }
    fclose(file);  
}

int main(int argc, char *argv[]) {



    return 0;
}