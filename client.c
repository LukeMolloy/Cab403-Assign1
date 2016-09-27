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
	char username [50];




int main(int argc, char *argv[]) {
	int sockfd, numbytes, i=0;  
	char buf[MAXDATASIZE];
	/* Thread and thread attributes */
	pthread_t client_thread;
	pthread_attr_t attr;

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



	buf[numbytes] = '\0';




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

	
	if (username != NULL) {
		printf("Please enter your password--> \n");
	}
	int blah = 5;
	send(socket_id, &blah, sizeof(uint16_t), 0);
	close(sockfd);
	return 0;
}

/*int main(int argc, char *argv[]) {
	
}*/
