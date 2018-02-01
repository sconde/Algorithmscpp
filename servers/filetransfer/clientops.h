#ifndef __CLIENTOPS_H__
#define __CLIENTOPS_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#if defined(__linux__)                                                          
#   include <endian.h>                                                          
#elif defined(__FreeBSD__) || defined(__NetBSD__)                               
#   include <sys/endian.h>                                                      
#elif defined(__OpenBSD__)                                                      
#   include <sys/types.h>                                                       
#   define be16toh(x) betoh16(x)                                                
#   define be32toh(x) betoh32(x)                                                
#   define be64toh(x) betoh64(x)                                                
#endif


#define BUFFSIZE 1024


// sendString will send input from stdin to server.
void sendString(int sockfd)                                                
{                                                                               
    char sendline[BUFFSIZE];                                                         
    char recvline[BUFFSIZE];                                                         
    
	while (1)                                                                   
    {                                                                           
        bzero(sendline, BUFFSIZE);                                                   
        bzero(recvline, BUFFSIZE);

        fgets(sendline, BUFFSIZE, stdin);                                                                                                                    
        write(sockfd, sendline, strlen(sendline)+1);                            
        read(sockfd, recvline, BUFFSIZE);                                            
        
		printf("received: %s\n", recvline);                                        
    }                           
} 


// writeFile will take the contents from buf and write them to a file.
int writeFile(FILE *file, const void *buf, int len)
{
	const char *pbuf = (const char *)buf;
	while (len > 0)
	{
		int written = fwrite(pbuf, 1, len, file);
		if (written < 1)
		{
			fprintf(stderr, "cannot write to file\n");
			return -1;
		}
		pbuf += written;
		len -= written;
	}
	return 0;
}

// readFile will connect to a socket and save contents to buf.
int readFile(int sockfd, void *buf, int len)
{
	char *pbuf = (char *)buf;
	int total = 0;
	while (len > 0)
	{
		int n = recv(sockfd, pbuf, len, 0);
		if (n < 0)
		{
			fprintf(stderr, "cannot read from socket\n");
			return -1;
		}
		if (n == 0)
		{
			fprintf(stderr, "socket disconnected\n");
			return 0;
		}
		pbuf += n;
		len -= n;
		total += n;
	}
	return total;
}

// recvFile uses a socket to connect to a server, recive file data and store
// it. It utilizes the functions readFile and writeFile.
void recvFile(int sockfd)
{
	char buff[BUFFSIZE];
	FILE *file = fopen("recv.txt", "wb");
	if (file == NULL)
	{
		fprintf(stderr, "Error opening file\n");
		return;                                                                 
	}

	int n;
	/* conventional 32bit call
	 * long size = 0;
	 */
	uint64_t size = 0;
	n = readFile(sockfd, &size, sizeof(size));
	printf("%d bytes recv: %ld\n", n, size);
	if (n>0)
	{
		/* Conventional 32bit call 
		 * size = ntohl(size);
		 */
		size = be64toh(size);
		while (size > 0)
		{
			n = readFile(sockfd, buff, MIN(sizeof(buff), size));
			if (n < 1)
				break;
			if (writeFile(file, buff, n) == -1)
				break;
		}
	}
	fclose(file);
}

#endif // __CLIENTOPS_H__
