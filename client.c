#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h>


	#define MAXDATASIZE 100 /* max number of bytes we can get at once */

	#define ARRAY_SIZE 30

	#define RETURNED_ERROR -1

	#define PORT 54321

/*void Receive_Array_Int_Data(int socket_identifier, int size) {
    int number_of_bytes, i=0;
    uint16_t statistics;

	int *results = malloc(sizeof(int)*ARRAY_SIZE);

	for (i=0; i < size; i++) {
		if ((number_of_bytes=recv(socket_identifier, &statistics, sizeof(uint16_t), 0))
		         == RETURNED_ERROR) {
			perror("recv");
			exit(EXIT_FAILURE);			
		    
		}
		results[i] = ntohs(statistics);
	}
	for (i=0; i < ARRAY_SIZE; i++) {
		printf("Array[%d] = %d\n", i, results[i]);
	}
}*/



int main(int argc, char *argv[]) {
	char username;
	printf("==========================================\n\n");
	printf("Welcome to the Online ATM System\n\n");
	printf("==========================================\n\n\n\n");
	printf("You are required to logon with your registered Username and PIN\n\n");
	printf("Please enter your username--> ");
	scanf("%c", &username);
	printf("%c", username);
	//printf("Please enter your password--> \n");



	return 0;
}
