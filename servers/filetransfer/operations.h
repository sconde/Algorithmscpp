#ifndef __OPERATIONS_H__
#define __OPERATIONS_H__

#include "utilities.h"

#define BUFFSIZE 100

void echoClient(int *sockfd)
{
	char buff[100];
	int client_status, commfd = *sockfd;                                                  
	while (1)                                                           
	{                                                                   
		client_status = checkSocket(commfd);                     
		if (client_status < 1)
		{
			fprintf(stdout, "Connection %s broken\n", getClientIP(commfd));
			break;     
		}
                                                                                
		bzero(buff, BUFFSIZE);                                                
		read(commfd, buff, BUFFSIZE);                                  
		fprintf(stdout, "Echoing back: %s", buff);                     
		write(commfd, buff, strlen(buff)+1);                       
	}
}

#endif // __OPERATIONS_H__
