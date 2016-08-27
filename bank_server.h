#include  <sys/types.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <unistd.h>
#include <signal.h>
#include <sys/signal.h>
#include  <errno.h>
#include  <string.h>
#include  <sys/socket.h>
#include  <netdb.h>
#include <pthread.h>

struct account{
  char account_name[100];
  float balance;
  int in_session;
  int owned;
  pthread_mutex_t account_lock;
};


pthread_mutex_t lock;
struct account clients[20];

int claim_port( const char * port ){
  struct addrinfo addrinfo;
  struct addrinfo *result;
  int sd;
  char message[256];
  int on = 1;
  int index;

  addrinfo.ai_flags = AI_PASSIVE;   // for bind()
  addrinfo.ai_family = AF_INET;   // IPv4 only
  addrinfo.ai_socktype = SOCK_STREAM; // Want TCP/IP
  addrinfo.ai_protocol = 0;   // Any protocol
  addrinfo.ai_addrlen = 0;
  addrinfo.ai_addr = NULL;
  addrinfo.ai_canonname = NULL;
  addrinfo.ai_next = NULL;
  
  if ( getaddrinfo( 0, port, &addrinfo, &result ) != 0 ){    // want port 
    fprintf( stderr, "getaddrinfo( %s ) failed errno is %s.  File %s line %d.\n", port, strerror( errno ), __FILE__, __LINE__ );
    return -1;
  }

  else if ( errno = 0, (sd = socket( result->ai_family, result->ai_socktype, result->ai_protocol )) == -1 ){
    write( 1, message, sprintf( message, "socket() failed.  File %s line %d.\n", __FILE__, __LINE__ ) );
    freeaddrinfo( result );
    return -1;
  }

  else if ( setsockopt( sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) ) == -1 ){
    write( 1, message, sprintf( message, "setsockopt() failed.  File %s line %d.\n", __FILE__, __LINE__ ) );
    freeaddrinfo( result );
    close( sd );
    return -1;
  }

  else if ( bind( sd, result->ai_addr, result->ai_addrlen ) == -1 ){
    freeaddrinfo( result );
    close( sd );
    write( 1, message, sprintf( message, "Binding to port %s ...\n", port ) );
    return -1;
  }

  else{
    write( 1, message, sprintf( message,  "SUCCESS : Bind to port %s\n", port ) );
    freeaddrinfo( result );   

    for(index=0;index<20;index++)
        clients[index].owned=0;    //sets each bank account to being unowned

    return sd;      // bind() succeeded;
  }

}

void print_account_info(){
  int i;

  for(i=0;i<20;i++){
      if(clients[i].owned==0)
        break;

      printf("%s\n",clients[i].account_name);
      printf(" %f\n",clients[i].balance );
      if(clients[i].in_session==1)
        printf(" %s\n\n", "IN SERVICE" );
      else{
        printf(" %s\n\n", "NOT IN SERVICE");
      }
  }

  alarm(20);

}

int start(char *name){
  int i;
  for(i=0;i<20;i++){
      if(strcmp(clients[i].account_name, name)==0){
        clients[i].in_session=1;
        pthread_mutex_lock(&clients[i].account_lock);
        return i;
      }
  }
    return -1;
}


int search_bank(char * name){
  int i;

  for(i=0;i<20;i++){
      if(clients[i].owned==0)
         return i;
      if(strcmp(clients[i].account_name, name)==0)
        return -1;
  }
    return i;


}

int open_account(char * name){
    pthread_mutex_lock(&lock);
    sleep(2);

    int i;
    i= search_bank(name);

    if(i>-1 && i < 20){
        memset(clients[i].account_name, 0 , sizeof(clients[i].account_name));
        strcpy(clients[i].account_name, name);
        clients[i].balance=0.0;
        clients[i].in_session=0;
        clients[i].owned=1;
        pthread_mutex_unlock(&lock);
        return 1;
    }


    pthread_mutex_unlock(&lock);
    return i;

} 

void bank_operation(char * command, char * request, int index, char * message, int * current_account, int * customer){

    char second_argument[110];
    memset(second_argument, 0, sizeof(second_argument));
    strncpy(second_argument, request+index, strlen(request)-index);
    int result;

    if(strcmp(command,"open")==0){
        result=open_account(second_argument);

        if(result==1){
          strcpy(message,"Account"); strcat(message, second_argument); strcat(message, " was opened successfully");
        }else if(result==-1){
          strcpy(message, "Account"); strcat(message, second_argument); strcat(message, " already exists");
        }else{
          strcpy(message, "Account"); strcat(message, second_argument); strcat(message, " couldn't be opened. Bank is full");
        }

    }else if(strcmp(command,"start")==0){
      result=start(second_argument);
        if(result>=0){
          strcpy(message,"Account"); strcat(message, second_argument); strcat(message, " is now in session");
          *current_account=result;
          *customer=1;
        }else if(result==-1){
          strcpy(message, "Account"); strcat(message, second_argument); strcat(message, " does not exist");
        }
    }

}

int credit(char * amount, int current_account){
  int x = atoi(amount);
  clients[current_account].balance += (float) x;
  return 1;
} 

int debit(char * amount, int current_account){
  int x = atoi(amount);

  if(clients[current_account].balance - x < 0)
    return -1;

  clients[current_account].balance -= (float) x;
  return 1;
}

void session_operation(char * command, char * request, int index, char * message, int current_account){
  char amount[512];
  int result;
  memset(amount, 0, sizeof(amount));

  if(strcmp(command,"balance")!=0){
    strncpy(amount, request+index, strlen(request)-index);

      if(strcmp(command,"credit")==0){
          result=credit(amount, current_account);
          strcpy(message, amount); strcat(message, " has been credited to"); strcat(message, clients[current_account].account_name);
      }else if(strcmp(command, "debit")==0){
          result=debit(amount, current_account);
          if(result==1){
            strcpy(message, amount); strcat(message, " has been debited from"); strcat(message, clients[current_account].account_name);
          }else{
            strcpy(message, amount); strcat(message, " could not be debited from"); strcat(message, clients[current_account].account_name);
          }
      }else{
        strcpy(message, "The commands while your account is in session are: credit  debit  balance  finish");
      } 

  }else{
      strcpy(message, "The balance for"); strcat(message, clients[current_account].account_name); strcat(message, " is");
      sprintf(amount,"%f", clients[current_account].balance);
      strcat(message, amount);
  }


}


void customer_session(int sd, int current_account){
  char request[512];
  char command[512];
  char message[512];
  int i;
  int size;

  while ( read( sd, request, sizeof(request) ) > 0 ){
    printf( "server received from%s:  %s\n", clients[current_account].account_name, request );

    size = strlen( request );

    for(i=0;i<size;i++){
        if(request[i]==' ')
          break;
    }

    if(i==0){
      write(sd, "bad input", 10);
      continue;
    }

    memset(message, 0, 512);
    memset(command, 0, sizeof(command));
    strncpy(command, request, i);

    if(strcmp(command,"finish")==0){
      pthread_mutex_unlock(&clients[current_account].account_lock);
      clients[current_account].in_session=0;
      write(sd, "Customer session has been ended", 32);
      return;
    }

    session_operation(command, request, i, message, current_account);
    message[strlen(message)+1]='\0';
    write(sd, message, strlen(message)+1);
    
    sleep(2);
  }

}


void *client_session_thread( void * arg ){
  int sd;
  char request[512];
  char command[512];
  char message[512];
  int i;
  int size;
  int current_account, customer;

  sd = *(int *)arg;
  free(arg);          // keeping to memory management covenant
  pthread_detach(pthread_self());   // Don't join on this thread

  if (pthread_mutex_init(&lock, NULL) != 0)
    {
        perror("\n mutex initialization failed\n");
    }


  while ( read( sd, request, sizeof(request) ) > 0 ){
      printf( "server receives input:  %s\n", request );

      size = strlen( request );

      for(i=0;i<size;i++){
          if(request[i]==' ')
            break;
      }

      if(i==0){
        write(sd, "bad input", 10);
        continue;
      }

      memset(message, 0, 512);
      memset(command, 0, sizeof(command));
      strncpy(command, request, i);

      if(strcmp(command,"exit")==0)
        break;
      
      bank_operation(command,request,i, message, &current_account, &customer);
      message[strlen(message)+1]='\0';
      write(sd, message, strlen(message)+1);

     if(customer==1){
        customer_session(sd, current_account);
        customer=0;
     }

     sleep(2);
  }

  close( sd );
  return 0;
}

