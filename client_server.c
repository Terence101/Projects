#include "client_server.h"

int main( int argc, char ** argv ){
	int	sd;
	int test;
	char message[256];
	char string[512];
	char buffer[512];
	char prompt[] = " ";
	int	len;
	//int	count;

	if ( argc < 2 ){
		fprintf( stderr, "No host name specified.  File %s line %d.\n", __FILE__, __LINE__ );
		exit( 1 );
	}
	else if ( (sd = connect_to_server( argv[1], "4567" )) == -1 ){
		write( 1, message, sprintf( message,  "Could not connect to server %s errno %s\n", argv[1], strerror( errno ) ) );
		return 1;
	}
	else{
		printf( "Connected to server %s, please enter a command:\n", argv[1] );
			printf("%s\n", "To Begin: " );
			printf("1.) open accountname\n");
			printf("2.) start accountname\n");
			printf("3.) exit\n");
			printf("\n%s\n", "After starting a Customer Session:");
			printf("1.) credit amount\n");
			printf("2.) debit amount\n");
			printf("3.) balance\n");
			printf("4.) finish\n");
			
		while ( write( 1, prompt, sizeof(prompt) ), (len = read( 0, string, sizeof(string) )) > 0 ){
			memset(buffer, 0, 512);
			string[len-1]= '\0';
			write( sd, string, strlen( string ) + 1 );
			read( sd, buffer, sizeof(buffer) );

			if(strcmp(buffer,"exit")==0){
				break;
			}
			write( 1, buffer, strlen(buffer) + 1);
			printf("\n");


			if ( (test = reconnect_to_server( argv[1], "4567" )) == -1 ){
				write(1, "Server has been killed\n", 24);
				break;
			}

			sleep(2);
		}
		close( sd );
		return 0;
	}
}
