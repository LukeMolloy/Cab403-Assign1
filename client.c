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

	#define USERNAME_SIZE 50


/*void Receive_Array_Int_Data(int socket_identifier, int size) {
    int number_of_bytes, i=0;
    uint16_t statistics;

	int *results = malloc(sizeof(int)*USERNAME_SIZE);


		if ((number_of_bytes=recv(socket_identifier, &statistics, sizeof(uint16_t), 0))
		         == RETURNED_ERROR) {
			perror("recv");
			exit(EXIT_FAILURE);			
		    
		}
		results = ntohs(statistics);
	
	
		printf("%d\n", results);
	
}*/





int main(int argc, char *argv[]) {
	int sockfd, numbytes, i=0, n;  
	char buf[MAXDATASIZE];
	struct hostent *he;
	struct sockaddr_in their_addr; /* connector's address information */

	if (argc != 3) {
		fprintf(stderr,"usage: client_hostname port_number\n");
		exit(1);
	}

	if ((he=gethostbyname(argv[1])) == NULL) {  /* get the host info */
		herror("gethostbyname");
		exit(1);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}


	their_addr.sin_family = AF_INET;      /* host byte order */
	their_addr.sin_port = htons(atoi(argv[2]));    /* short, network byte order */
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(their_addr.sin_zero), 8);     /* zero the rest of the struct */

	if (connect(sockfd, (struct sockaddr *)&their_addr, \
	sizeof(struct sockaddr)) == -1) {
		perror("connect");
		exit(1);
	}

	
	char username[256];
	char password[256];
	
	
	printf("==========================================\n\n");
	printf("Welcome to the Online ATM System\n\n");
	printf("==========================================\n\n\n\n");
	printf("You are required to logon with your registered Username and PIN\n\n");
	printf("\nPlease enter your username--> ");
	
	bzero(username, 265);
	fgets(username, 255, stdin);
	n = send(sockfd, username, strlen(username), 0);
	bzero(username, 256);
	

	
	if (username != NULL) {
		printf("\nPlease enter your password-->");
		bzero(password, 265);
		fgets(password, 255, stdin);
		n = send(sockfd, password, strlen(password), 0);
		bzero(password, 256);
	}


	//bzero(success, 256);
	//bzero(success, 256);
	//n = recv(sockfd, success, strlen(success), 0);
	//success[strcspn(success, "\n")] = 0;
	//printf("%s", success);
	
	char success [4];
	recv(sockfd, success, 4, 0);
	//printf("%s", success);
	if (strcmp(success, "True") == 0) {
		printf("Welcome to the ATM System\n\n");
		printf("You are currently logged in as: \n");
		printf("Client number: \n\n\n");
		printf("Please enter a selection\n");
		printf("<1> Account Balance\n");
		printf("<2> Withdrawal\n");
		printf("<3> Deposit\n");
		printf("<4> Transfer\n");
		printf("<5> Transaction Listing\n");
		printf("<6> EXIT\n\n");
		printf("Selection option 1 - 6 ->");


	}		

	
//printf("PasswordGotten: %s\n", password);





	//buf[numbytes] = '\0';

	//printf("Received: %s",buf);

	close(sockfd);

	return 0;
}



/*

	printf("==========================================\n\n");
	printf("Welcome to the Online ATM System\n\n");
	printf("==========================================\n\n\n\n");
	printf("You are required to logon with your registered Username and PIN\n\n");
	printf("Please enter your username--> ");
	scanf("%s", &username);
	printf("%s\n", username);
	 

	//int clientSocket, portNum, nBytes;	

	//while(1) {
	//	scanf("%s", &username);
	//	printf("%s\n", username);
	//	fgets(username, 50, stdin);
	//	nBytes = strlen(username) + 1;
	//	send(clientSocket, username, nBytes, 0); 
		//recv(clientSocket, username, 50, 0);
	//}

	


/*	
	if (username != NULL) {
		printf("Please enter your password--> \n");
	}
	char blah = 5;
	//send(argv[1], &blah, sizeof(uint16_t), 0);
	close(sockfd);
	return 0;
}

/*int main(int argc, char *argv[]) {
	
}*/
