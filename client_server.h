#include	<sys/types.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<errno.h>
#include	<string.h>
#include	<sys/socket.h>
#include	<netdb.h>


int connect_to_server(char * server, const char * port ){

	int	sd;
	struct addrinfo addrinfo;
	struct addrinfo *result;
	char message[256];
	memset(&addrinfo, 0, sizeof(addrinfo));

	addrinfo.ai_flags = 0;
	addrinfo.ai_family = AF_INET;		// IPv4 only
	addrinfo.ai_socktype = SOCK_STREAM;	// Want TCP/IP
	addrinfo.ai_protocol = 0;		// Any protocol
	addrinfo.ai_addrlen = 0;
	addrinfo.ai_addr = NULL;
	addrinfo.ai_canonname = NULL;
	addrinfo.ai_next = NULL;

	if ( getaddrinfo(server, port, &addrinfo, &result ) != 0 )
	{
		fprintf( stderr, "getaddrinfo( %s ) failed.  File %s line %d.\n", server, __FILE__, __LINE__ );
		return -1;
	}
	else if ( errno = 0, (sd = socket( result->ai_family, result->ai_socktype, result->ai_protocol )) == -1 )
	{
		freeaddrinfo( result );
		return -1;
	}
	else
	{
		do {
			if ( errno = 0, connect( sd, result->ai_addr, result->ai_addrlen ) == -1 )
			{
				sleep( 3 );
				write( 1, message, sprintf( message, "Connecting to server %s ...\n", server ) );
			}
			else
			{
				freeaddrinfo( result );
				return sd;		// connect() succeeded
			}
		} while ( errno == ECONNREFUSED );
		freeaddrinfo( result );
		return -1;
	}
}


int reconnect_to_server(char * server, const char * port ){

	int	sd;
	struct addrinfo addrinfo;
	struct addrinfo *result;
	memset(&addrinfo, 0, sizeof(addrinfo));

	addrinfo.ai_flags = 0;
	addrinfo.ai_family = AF_INET;		// IPv4 only
	addrinfo.ai_socktype = SOCK_STREAM;	// Want TCP/IP
	addrinfo.ai_protocol = 0;		// Any protocol
	addrinfo.ai_addrlen = 0;
	addrinfo.ai_addr = NULL;
	addrinfo.ai_canonname = NULL;
	addrinfo.ai_next = NULL;

	if ( getaddrinfo(server, port, &addrinfo, &result ) != 0 )
	{
		fprintf( stderr, "getaddrinfo( %s ) failed.  File %s line %d.\n", server, __FILE__, __LINE__ );
		return -1;
	}
	else if ( errno = 0, (sd = socket( result->ai_family, result->ai_socktype, result->ai_protocol )) == -1 )
	{
		freeaddrinfo( result );
		return -1;
	}
	else
	{
		do {
			if ( errno = 0, connect( sd, result->ai_addr, result->ai_addrlen ) == -1 )
			{
				return -1;
			}
			else
			{
				freeaddrinfo( result );
				return sd;		// connect() succeeded
			}
		} while ( errno == ECONNREFUSED );
		freeaddrinfo( result );
		return -1;
	}
}
