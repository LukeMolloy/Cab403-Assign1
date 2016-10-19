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

const char delimiters[] = "  \n,";
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

void add_request(int request_num, pthread_mutex_t* p_mutex, pthread_cond_t*  p_cond_var, int new_fd)
{
    int rc;                         
    struct request* a_request;      
    printf("-B-\n");
    
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
    printf("-C-\n");
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

int login(int thread, int socketid) {
    printf("-D- %d\n", socketid);
    printf("In Thread %d", thread);
    char username[256];
    char password[256];
    int p = 0;
    int n = 0;
    int ret = 0;

    bzero(username, 256);

    n = recv(socketid, username, 255, 0);
    username[strcspn(username, "\n")] = 0;

    if (n < 0) {
        error("ERROR reading from socket");
    }

    printf("UsernameGotten: %s\n", username);
    n = send(socketid, "Got username\n", 12, 0);
    
    if (n < 0) {
        error("ERROR writing to socket");
    }

    if (socketid < 0) {
        error("ERROR on accept");
    }   

    bzero(password, 256);

    p = recv(socketid, password, 255, 0); 
    
    if (p < 0) {
        error("ERROR reading from socket");
    }
    //p = send(socketid, "-A-\n", 3, 0)
    printf("PasswordGotten: %s\n", password);
    p = send(socketid, "Got password\n", 12, 0);
    //p = send(socketid, "-B-\n", 3, 0)
    if (p < 0) {
        error("ERROR writing to socket");
    }

    password[strcspn(password, "\n")] = 0;

    p = send(socketid, "Checking Details\n", 16, 0);
    char success[256] = "True";     

    for (int i = 0; i<11; i++) {
        if ((strcmp(username, Authentication[i][0]) == 0) && (strcmp(password, Authentication[i][1]) == 0)) {       
            n = send(socketid, success, strlen(success), 0);
            ret = 1;
            i = 11;
        }
    }
    p = send(socketid, "Done\n", 4, 0);
    return ret;
        
}

void handle_request(struct request* a_request, int thread_id)
{
    printf("-E-\n");
    if (a_request) {
        //printf("Thread '%d' handled request '%d'\n",
         //      thread_id, a_request->number);
        //fflush(stdout);

        int auth;
        int p = 0;
        auth = login(thread_id, a_request->socketid);

        if(auth == 1){
            p = send(a_request->socketid, "Success\n", 7, 0);
        }else{
            p = send(a_request->socketid, "Fail\n", 4, 0);    
        }
    }
}

void* handle_requests_loop(void* data)
{
    printf("-F-\n");
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
    int        i;                        
    int        thr_id[NUM_HANDLER_THREADS];      
    pthread_t  p_threads[NUM_HANDLER_THREADS];   
    struct timespec delay;    

    int sockfd, new_fd;              
    struct sockaddr_in my_addr;    
    struct sockaddr_in their_addr; 
    socklen_t sin_size;

    tokenAuth();
   
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
        printf("-A-\n");
        add_request(a, &request_mutex, &got_request, new_fd);

        a++;
    }
    
    return 0;
}
