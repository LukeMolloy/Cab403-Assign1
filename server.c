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



	#define ARRAY_SIZE 30  /* Size of array to receive */

	#define BACKLOG 10     /* how many pending connections queue will hold */

	#define RETURNED_ERROR -1

	#define MAXDATASIZE 100 /* max number of bytes we can get at once */

	#define USERNAME_SIZE 50



const char delimiters[] = "	 \n,";
char *Authentication[11][3];
char *Accounts[25][3];
char *ClientDetails[11][6];
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






/*void Send_Username(int socket_id) {

	int blah=2;

	uint16_t statistics;  
		statistics = htons(blah);
		send(socket_id, &statistics, sizeof(uint16_t), 0);
	
}*/


int main(int argc, char *argv[]) {
	tokenAuth();

	int sockfd, new_fd, n, p, s;  /* listen on sock_fd, new connection on new_fd */
	struct sockaddr_in my_addr;    /* my address information */
	struct sockaddr_in their_addr; /* connector's address information */
	socklen_t sin_size;
	int i=0;
	char username[256];
	char password[256];

	/* Get port number for server to listen on */
	if (argc != 2) {
		fprintf(stderr,"usage: client port_number\n");
		exit(1);
	}

	/* generate the socket */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	/* generate the end point */
	my_addr.sin_family = AF_INET;         /* host byte order */
	my_addr.sin_port = htons(atoi(argv[1]));     /* short, network byte order */
	my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */
		/* bzero(&(my_addr.sin_zero), 8);   ZJL*/     /* zero the rest of the struct */

	/* bind the socket to the end point */
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) \
	== -1) {
		perror("bind");
		exit(1);
	}

	/* start listnening */
	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	printf("server starts listnening ...\n");

	char dest[50];
		
	new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);
	
	//Get username
	if (new_fd < 0) {
		error("ERROR on accept");
	}	

	bzero(username, 256);





	
	n = recv(new_fd, username, 255, 0);
	username[strcspn(username, "\n")] = 0;



	

	
	if (n < 0) {
		error("ERROR reading from socket");
	}

	printf("UsernameGotten: %s\n", username);
	//n = send(new_fd, "Got username\n", 12, 0);

	if (n < 0) {
		error("ERROR writing to socket");
	}


	//Get password
	if (new_fd < 0) {
		error("ERROR on accept");
	}	

	bzero(password, 256);

	p = recv(new_fd, password, 255, 0); 
	
	if (p < 0) {
		error("ERROR reading from socket");
	}

	printf("PasswordGotten: %s\n", password);
	//p = send(new_fd, "Got password\n", 12, 0);

	if (p < 0) {
		error("ERROR writing to socket");
	}

	password[strcspn(password, "\n")] = 0;

	
		

	char success[4] = "True";
	//*((int*)success) = 1111;



	for (int i = 0; i<11; i++) {
		if (strcmp(username, Authentication[i][0]) == 0
		&& strcmp(password, Authentication[i][1]) == 0) {		
					printf("Account Found");
					//bzero(success, 265);
					//fgets(success, 255, stdin);
					send(new_fd, success, 4, 0);
					
					
		}
	}


	
			

	close(new_fd);  
return 0;
}
