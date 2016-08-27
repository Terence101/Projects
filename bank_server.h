#include "bank_server.h"

int main( int argc, char *argv[]){
  int sd;
  char message[256];
  pthread_t tid;
  pthread_attr_t kernel_attr;
  socklen_t ic;
  int fd;
  struct sockaddr_in senderAddr;
  int *fdptr;
  int i, key, shmid;
  //struct clients accounts[20];

  signal(SIGALRM, print_account_info);
  i=0;

  key=ftok("bank_server.c",42);
  
  if((shmid=shmget(key,sizeof(struct Bank),IPC_CREAT|0666))<0){
    perror("shmget failed");
    exit(1);
  }

  bank=shmat(shmid,NULL,0);

  if ( pthread_attr_init( &kernel_attr ) != 0 ){
    printf( "pthread_attr_init() failed in file %s line %d\n", __FILE__, __LINE__ );
    return 0;
  }

  else if ( pthread_attr_setscope( &kernel_attr, PTHREAD_SCOPE_SYSTEM ) != 0 ){
    printf( "pthread_attr_setscope() failed in file %s line %d\n", __FILE__, __LINE__ );
    return 0;
  }

  else if ( (sd = claim_port("4567")) == -1 ){
    write( 1, message, sprintf( message,  "Could not bind to port %s errno %s\n", "4567", strerror( errno ) ) );
    return 1;
  }

  else if ( listen(sd, 10) == -1 ){
    printf( "listen() failed in file %s line %d\n", __FILE__, __LINE__ );
    close( sd );
    return 0;
  }

  else{
    ic = sizeof(senderAddr);
    while ( (fd = accept( sd, (struct sockaddr *)&senderAddr, &ic )) != -1 ){
      fdptr = (int *)malloc( sizeof(int) );
      *fdptr = fd;          // pointers are not the same size as ints any more.
      if ( pthread_create( &tid, &kernel_attr, client_session_thread, fdptr ) != 0 ){
        printf( "pthread_create() failed in file %s line %d\n", __FILE__, __LINE__ );
        return 0;
      }

      else{

        if(i==0){
          alarm(20);
          i=1;
        }

        continue;
      }

    }
    close( sd );
    return 0;
  }
}
