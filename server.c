#define _GNU_SOURCE
#include <stdio.h>       
#include <pthread.h>     
#include <stdlib.h>      
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h> 
#include <string.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/wait.h> 
#include <unistd.h>

#define NUM_HANDLER_THREADS 10

#define BACKLOG 10

#define RETURNED_ERROR -1

pthread_mutex_t request_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

pthread_cond_t  got_request   = PTHREAD_COND_INITIALIZER;

int num_requests = 0;  

struct request {
    int number; 
    int socketid;            
    struct request* next;   
};

struct request* requests = NULL;     
struct request* last_request = NULL; 

const char delimiters[] = ",\n\t\r\v\f ";
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
        printf("\n");
        x++;
    }
    fclose(file);  
}

void add_request(int request_num, pthread_mutex_t* p_mutex, pthread_cond_t*  p_cond_var, int new_fd)
{
    int rc;                         
    struct request* a_request;      
    
    a_request = (struct request*)malloc(sizeof(struct request));
    if (!a_request) { 
        fprintf(stderr, "add_request: out of memory\n");
        exit(1);
    }
    a_request->number = request_num;
    a_request->next = NULL;
    a_request->socketid = new_fd;

    rc = pthread_mutex_lock(p_mutex);

    if (num_requests == 0) {
        requests = a_request;
        last_request = a_request;
    }
    else {
        last_request->next = a_request;
        last_request = a_request;
    }

    num_requests++;

    rc = pthread_mutex_unlock(p_mutex);

    rc = pthread_cond_signal(p_cond_var);
}

struct request* get_request(pthread_mutex_t* p_mutex)
{
    int rc;                         
    struct request* a_request;      

    rc = pthread_mutex_lock(p_mutex);

    if (num_requests > 0) {
        a_request = requests;
        requests = a_request->next;
        if (requests == NULL) {
            last_request = NULL;
        }
        num_requests--;
    }
    else { 
        a_request = NULL;
    }
    rc = pthread_mutex_unlock(p_mutex);
    return a_request;
}

char *login(int thread, int socketid) {
    char username[255];
    char password[255];
    int p = 0;
    int n = 0;
	int h;
    int ret = 0;

    bzero(username, 255);

    n = recv(socketid, username, 255, 0);
    username[strcspn(username, "\n")] = 0;

    if (n < 0) {
        error("ERROR reading from socket");
    }

    printf("UsernameGotten: %s\n", username);
    //n = send(socketid, "Got username\n", 12, 0);
    
    if (n < 0) {
        error("ERROR writing to socket");
    }

    if (socketid < 0) {
        error("ERROR on accept");
    }   

    bzero(password, 255);

    p = recv(socketid, password, 255, 0); 
    password[strcspn(password, "\n")] = 0;
    
    if (p < 0) {
        error("ERROR reading from socket");
    }
    printf("PasswordGotten: %s\n", password);
    char success[255] = "True";
    char *client_no = malloc(255);  
    char first_name[255];
    char last_name[255];  

   	for (int i = 0; i<11; i++) {
        //printf("%s ", Authentication[i][0]);
		if (strcmp(username, Authentication[i][0]) == 0 && strcmp(password, Authentication[i][1]) == 0) {		
			send(socketid, success, 255, 0);
            strcpy(client_no, Authentication[i][2]);
            send(socketid, client_no, 255, 0);
            //send(socketid, success, 4, 0);
            //printf("<%s>\n\n", client_no);
            for (int x = 0; x<11; x++) {
                //printf("<%s>\n", ClientDetails[x][2]);
                if (strcmp(client_no, ClientDetails[x][2]) ==  0) { 

                    strcpy(first_name, ClientDetails[x][0]);
                    send(socketid, first_name, 255, 0);

                    strcpy(last_name, ClientDetails[x][1]);
                    send(socketid, last_name, 255, 0);
                    //strcpy(last_name, Authentication[i][1]);
                    //send(socketid, last_name, 255, 0);  
                }
            }	
		}
	}


    //printf("True");
    //p = send(socketid, "Done\n", 4, 0);
    return client_no;
        
}

char *listAccounts(int socketid, char *auth, char *chosen){
    char *chosenaccount = malloc(255);
    int x=0;
    char *acc1 = malloc(255);
    char *acc2 = malloc(255);
    char *acc3 = malloc(255);

    //printf("-A-");
    for(x=0; x < 11; x++){
        //printf("%s\n", ClientDetails[x][2]);
        if (strcmp(auth, ClientDetails[x][2]) == 0){
            acc1 = ClientDetails[x][3];
            acc2 = ClientDetails[x][4];
            acc3 = ClientDetails[x][5];
            //printf("-B-");
        }
    }

    int num = atoi(chosen);
    if(num == 1){
        //printf("-C-");
        send(socketid, acc1, 255, 0);
        send(socketid, acc2, 255, 0);
        send(socketid, acc3, 255, 0);
        recv(socketid, chosenaccount, 255, 0);
    }else if(num == 2){
        send(socketid, acc1, 255, 0);
        send(socketid, acc2, 255, 0);
        send(socketid, acc3, 255, 0);
        recv(socketid, chosenaccount, 255, 0);
    }else if(num == 3){
        send(socketid, acc1, 255, 0);
        send(socketid, acc2, 255, 0);
        send(socketid, acc3, 255, 0);
        recv(socketid, chosenaccount, 255, 0);
    }else if(num == 4){
        
    }else if(num == 5){
        
    }        

    return chosenaccount;
}

void retclosebal(int socketid, char *chosenAcc){

    int AccPos;
    for(int x = 0; x < 25; x++){
        if(strcmp(chosenAcc, Accounts[x][0]) == 0){
            AccPos = x;
        }
        //printf("Account Bal: %s\n", Accounts[x][2]);
    }
    //printf("Account Sent: %s\n", Accounts[AccPos][2]);
    send(socketid, Accounts[AccPos][2], 255, 0);

}

void updateclosebal(int socketid, char *chosenAcc){
    char *newclose = malloc(255);
    char *fail = "False";
    recv(socketid, newclose, 255, 0);
    newclose[strcspn(newclose, "\n")] = 0;
    printf("<%s>\n", newclose);
    printf("<%s>\n", fail);
    
    for(int x = 0; x < 25; x++){
        if(strcmp(chosenAcc, Accounts[x][0]) == 0){
            //Accounts[x][2] = malloc(sizeof(newclose));
            if(strcmp(newclose, "False") !=0){
                Accounts[x][2] = newclose;
            }
        }
    }
    printf("\n");
}

void mainmenu(struct request* a_request, int thread_id, char *auth){
    int socketid = a_request->socketid;
    char chosen[255];
    char quit[255];
    char *chosenAcc;

    while(1){
        recv(socketid, chosen, 255, 0);

        int num = atoi(chosen);

        if(num == 1){
            printf("%s\n", chosen);
            chosenAcc = listAccounts(socketid, auth, chosen);
            printf("%s\n", chosenAcc);
            retclosebal(socketid, chosenAcc);
        }else if(num == 2){
            printf("%s\n", chosen);
            chosenAcc = listAccounts(socketid, auth, chosen);
            if(strcmp(chosenAcc, "e") != 0){
                recv(socketid, quit, 255, 0);
                quit[strcspn(quit, "\n")] = 0;
                if(strcmp(quit, "True") == 0){
                    retclosebal(socketid, chosenAcc);
                    updateclosebal(socketid, chosenAcc); 
                } 
            }
        }else if(num == 3){
            printf("%s\n", chosen);
            chosenAcc = listAccounts(socketid, auth, chosen);
            if(strcmp(chosenAcc, "e") != 0){
                recv(socketid, quit, 255, 0);
                quit[strcspn(quit, "\n")] = 0;
                if(strcmp(quit, "True") == 0){
                    retclosebal(socketid, chosenAcc);
                    updateclosebal(socketid, chosenAcc);
                }
            }
        }else if(num == 4){
            printf("%s\n", chosen);
        }else if(num == 5){
            printf("%s\n", chosen);
        }else if(num == 6){
            printf("%s\n", chosen);
            break;
        } 
        num = 0;
    }
}



void handle_request(struct request* a_request, int thread_id)
{
    if (a_request) {

        int p = 0;
        char *auth = login(thread_id, a_request->socketid);

        if(auth != NULL){
            mainmenu(a_request, thread_id, auth);
        }else{
            printf("YOU FAIL\n");   
        }
    }
}

void* handle_requests_loop(void* data)
{
    int rc;                         
    struct request* a_request;      
    int thread_id = *((int*)data);  

    rc = pthread_mutex_lock(&request_mutex);

    while (1) {

        if (num_requests > 0) { 
            a_request = get_request(&request_mutex);
            if (a_request) { 
                rc = pthread_mutex_unlock(&request_mutex);
                handle_request(a_request, thread_id);
                free(a_request);
                rc = pthread_mutex_lock(&request_mutex);
            }
        }
        else {
            rc = pthread_cond_wait(&got_request, &request_mutex);
        }
    }
}

int main(int argc, char* argv[]) {
    tokenAuth();
    tokenClient();
    tokenAccount();
    tokenClient();

    int        i;                        
    int        thr_id[NUM_HANDLER_THREADS];      
    pthread_t  p_threads[NUM_HANDLER_THREADS];   
    struct timespec delay;    

    int sockfd, new_fd;              
    struct sockaddr_in my_addr;    
    struct sockaddr_in their_addr; 
    socklen_t sin_size;

    if (argc != 2) {
        fprintf(stderr,"usage: client port_number\n");
        exit(1);
    }

    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(atoi(argv[1]));     
    my_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) \
    == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    printf("server starts listnening ...\n");

    for (i=0; i<NUM_HANDLER_THREADS; i++) {
        thr_id[i] = i;
        pthread_create(&p_threads[i], NULL, handle_requests_loop, (void*)&thr_id[i]);
    }

    int a = 1;

    while(1) {    
        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, \
        &sin_size)) == -1) {
            perror("accept");
            continue;
        }
        printf("server: got connection from %s\n", \
            inet_ntoa(their_addr.sin_addr));
        add_request(a, &request_mutex, &got_request, new_fd);

        a++;
    }
	
	
    
    return 0;
}
