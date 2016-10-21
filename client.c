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

void welcome(){
	printf("==========================================\n\n");
	printf("Welcome to the Online ATM System\n\n");
	printf("==========================================\n\n\n\n");
	printf("You are required to logon with your registered Username and PIN\n\n");
}

void info(char *client_no, char *first_name, char *last_name){
	/*char client_no[255];
	char first_name[255];
	char last_name[255];
	char first_option[255];*/

	printf("Welcome to the ATM System\n\n");
	printf("You are currently logged in as: %s", first_name);
	printf(" %s\n", last_name);
	printf("Client number: %s\n", client_no);
	printf("\n\n");
}

void options(){
	printf("Please enter a selection\n");
	printf("<1> Account Balance\n");
	printf("<2> Withdrawal\n");
	printf("<3> Deposit\n");
	printf("<4> Transfer\n");
	printf("<5> Transaction Listing\n");
	printf("<6> EXIT\n\n");
}

char *login(int sockfd){
	char username[255];
	char password[255];
	char *success = malloc(255);

	printf("\nPlease enter your username--> ");

	bzero(username, 255);
	fgets(username, 255, stdin);
	send(sockfd, username, strlen(username), 0);
	bzero(username, 255);

	if (username != NULL) {
		printf("\nPlease enter your password-->");
		bzero(password, 255);
		fgets(password, 255, stdin);
		send(sockfd, password, strlen(password), 0);
		bzero(password, 255);	
	}

	recv(sockfd, success, 255, 0);

	return success;
}


void displaybal(int sockfd, char *chosenaccount, char *firstname, char *lastname){
	char balance[255];
	recv(sockfd, balance, 255, 0);

	printf("============================================================\n");
	printf("\n");
	printf("Account Name - %s %s\n", firstname, lastname);
	printf("\n");
	printf("Current Balance for Account %s : $%s\n", chosenaccount, balance);
	printf("\n");
	printf("============================================================\n");
}


char *getaccount(int sockfd, int option){
	char acc1[255];
	char acc2[255];
	char acc3[255];
	char *chosenaccount = malloc(255);
	char *options[3];

	int optionCount = 0;

	recv(sockfd, acc1, 255, 0);
	recv(sockfd, acc2, 255, 0);
	recv(sockfd, acc3, 255, 0);

	int accnum1 = atoi(acc1);
	int accnum2 = atoi(acc2);
	int accnum3 = atoi(acc3);

	printf("Select Account Type\n");
	//printf("%d\n",accnum1);
	if((accnum1 != 0) && ((accnum1 % 11) == 0)) {
		options[optionCount] = acc1;
		optionCount++;
		printf("%d. Savings Account\n",optionCount);
		
	}else if((accnum1 != 0) && ((accnum1 % 12) == 0) && (option != 2)) {
		options[optionCount] = acc1;
		optionCount++;
		printf("%d. Loan Account\n",optionCount);
		
	}else if((accnum1 != 0) && ((accnum1 % 13) == 0)) {
		options[optionCount] = acc1;
		optionCount++;
		printf("%d. Credit Card\n",optionCount);
		
	}
	//printf("%d\n",accnum2);
	if((accnum2 != 0) && ((accnum2 % 11) == 0)) {
		options[optionCount] = acc2;
		optionCount++;
		printf("%d. Savings Account\n",optionCount);
		
	}else if((accnum2 != 0) && ((accnum2 % 12) == 0) && (option != 2)) {
		options[optionCount] = acc2;
		optionCount++;
		printf("%d. Loan Account\n",optionCount);
		
	}else if((accnum2 != 0) && ((accnum2 % 13) == 0)) {
		options[optionCount] = acc2;
		optionCount++;
		printf("%d. Credit CardA\n",optionCount);
		
	}
	//printf("%d\n",accnum3);
	if((accnum3 != 0) && ((accnum3 % 11) == 0)) {
		options[optionCount] = acc3;
		optionCount++;
		printf("%d. Savings Account\n",optionCount);
		
	}else if((accnum3 != 0) && ((accnum3 % 12) == 0) && (option != 2)) {
		options[optionCount] = acc3;
		optionCount++;
		printf("%d. Loan Account\n",optionCount);
		
	}else if((accnum3 != 0) && ((accnum3 % 13) == 0)) {
		options[optionCount] = acc3;
		optionCount++;
		printf("%d. Credit Card\n",optionCount);
		
	}

	int loop = 0;
	while(loop == 0){
		printf("\nEnter your selection (E/e to exit) - ");
		char chosen[255];
		fgets(chosen, 255, stdin);
		chosen[strcspn(chosen, "\n")] = 0;
		int numoption = atoi(chosen);
	
		if((numoption == 1) && (optionCount >= numoption)){
			chosenaccount = options[0];
			loop++;
		}else if((numoption == 2) && (optionCount >= numoption)){
			chosenaccount = options[1];
			loop++;
		}else if((numoption == 3) && (optionCount >= numoption)){
			chosenaccount = options[2];
			loop++;
		}else if(strcmp(chosen, "e") == 0){
			chosenaccount = "e";
			loop++;
		}else if(strcmp(chosen, "E") == 0){
			chosenaccount = "e";
			loop++;
		}
	}
	printf("\n");
	send(sockfd, chosenaccount, 255, 0);

	return chosenaccount;

	
}

double withdraw(int sockfd, int accountnum, char *money){
	char balance[255];
	char account[255];
	char *strfinal = malloc(255);
	recv(sockfd, balance, 255, 0);
	//printf("Account: <%d>\n", accountnum);

	double numbalance = atof(balance);
	//printf("Balance: %.2lf\n", numbalance);
	double nummoney = atof(money);
	//printf("Withdrawal: %.2lf\n", nummoney);

	double numfinal = numbalance - nummoney;
	//printf("Final Balance: %.2lf\n", numfinal);

	sprintf(strfinal, "%.2lf", numfinal);

	if((numfinal >= 0) && ((accountnum % 11) == 0)) {
			
		send(sockfd, strfinal, 255, 0);

	}else if((numfinal >= -5000) && ((accountnum % 13) == 0)) {
		
		send(sockfd, strfinal, 255, 0);

	}else{
		send(sockfd, "False", 255, 0);
		numfinal = -5001;
	}

	return numfinal;
}

double deposit(int sockfd, int accountnum, char *money){
	char balance[255];
	char account[255];
	char *strfinal = malloc(255);
	recv(sockfd, balance, 255, 0);
	//printf("Account: <%d>\n", accountnum);

	double numbalance = atof(balance);
	//printf("Balance: %.2lf\n", numbalance);
	double nummoney = atof(money);
	//printf("Withdrawal: %.2lf\n", nummoney);

	double numfinal = numbalance + nummoney;
	//printf("Final Balance: %.2lf\n", numfinal);

	sprintf(strfinal, "%.2lf", numfinal);
			
	send(sockfd, strfinal, 255, 0);

	return numfinal;
}

void mainmenu(int sockfd){
	char first_option[255];
	char money[255];
	char client_no[255];
	char first_name[255];
	char last_name[255];
	char *chosenaccount = malloc(255);

	recv(sockfd, client_no, 255, 0);
	recv(sockfd, first_name, 255, 0);
	recv(sockfd, last_name, 255, 0);

	info(client_no, first_name, last_name);

	while(1){
		options();
		printf("Selection option 1 - 6 ->");
		fgets(first_option, 255, stdin);
		printf("\n");

		int num = atoi(first_option);

		if(num == NULL){
			printf("Invalid selection. Please select option from menu!\n\n\n");
		}else if(num == 1){
			send(sockfd, first_option, 255, 0);
			chosenaccount = getaccount(sockfd, num);
			displaybal(sockfd, chosenaccount, first_name, last_name);
			printf("\n");
		}else if(num == 2){
			send(sockfd, first_option, 255, 0);
			printf("============================================================\n");
			printf("\n");
			chosenaccount = getaccount(sockfd, num);
			if(strcmp(chosenaccount, "e") != 0){
				printf("Enter the amount to withdraw (E/e to exit) : $");
				fgets(money, 255, stdin);
				money[strcspn(money, "\n")] = 0;
				if((strcmp(money,"e") != 0) && (strcmp(money,"E") != 0)){
					send(sockfd, "True", 255, 0);
					int account = atoi(chosenaccount);
					//printf("%d\n", account);
					double success = withdraw(sockfd, account, money);

					if(success != -5001){
						printf("\nWithdrawal Completed: Closing Balance : $%.2lf\n\n", success);
					}else{
						printf("\nInsufficient Funds - Unable to process request\n\n");
					}
					printf("============================================================\n");
				}else{
					send(sockfd, "False", 255, 0);
				}
			}
			
		}else if(num == 3){
			send(sockfd, first_option, 255, 0);
			int moneycheck = 1001;
			printf("============================================================\n");
			printf("\n");
			printf("The maximum daily deposit is $1000.00!\n\n\n");
			chosenaccount = getaccount(sockfd, num);
			int account = atoi(chosenaccount);
			if(strcmp(chosenaccount, "e") != 0){
				while(moneycheck >= 1001){
					printf("Enter the amount to deposit (E/e to exit) : $");
					fgets(money, 255, stdin);
					money[strcspn(money, "\n")] = 0;
					if((strcmp(money, "e") != 0) && (strcmp(money, "E") != 0)){
						send(sockfd, "True", 255, 0);
						moneycheck = atoi(money);
						if(moneycheck >= 1001){
							printf("\nYou cannot deposit more then $1000 in a single transaction!\n\n");
						}else{
							double success = deposit(sockfd, account, money);
							printf("\nDeposit Completed: Closing Balance : $%.2lf\n\n", success);
							printf("============================================================\n\n");
							//break;
						}
					}else{
						send(sockfd, "False", 255, 0);
						break;
					}
				}
			}
			
		}else if(num == 5){
			send(sockfd, first_option, 255, 0);
		}else if(num == 6){
			send(sockfd, first_option, 255, 0);
			exit(1);
		}else if(num >= 7){
			printf("Invalid selection. Please select option from menu!\n\n\n\n");
		}
	}	
}

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

	welcome();
	char success2[255] = "True";

	char *success = login(sockfd);

	if(strcmp(success, success2) == 0){
		mainmenu(sockfd);
	}

	close(sockfd);

	return 0;
}
