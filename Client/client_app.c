#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>


#define ADDRESS_IPV4	AF_INET		/*AF_INET is a macro with value 2 and supports IPv4 addrss family*/
#define ADDR_IPV6	AF_INET6	/*AF_INET6 is macro for IPv6 address family*/
#define TCP_SOCK	SOCK_STREAM	/*SOCK_STREAM represents TCP*/
#define UDP_SOCK	SOCK_DGRAM	/*SOCK_DGRAM represents UDP*/
#define CLIENT_COUNT	(3)		/*Server can wait on Three client*/
#define REC_PACKET_SIZE	(200)		/*recieved packet size:number of bytes*/
#define RW_FLAG		(0)		/*some flags required by accept api*/

enum {FALSE,TRUE}bool_t;
enum {ERROR = -1}error_t;

int create_socket(void)
{
	short hsock = 0;
	printf("creating socket ....\n");
	hsock = socket(ADDRESS_IPV4, TCP_SOCK, 0);
	return hsock;
}
int connect_socket(int hsock)
{
	int iRetval = -1;
	int server_port = 12348;
	struct sockaddr_in remote = {0};
	remote.sin_family = ADDRESS_IPV4;
	remote.sin_addr.s_addr = inet_addr("127.0.0.1");
	remote.sin_port = htons(server_port);

	iRetval = connect(hsock,(struct sockaddr *)&remote,sizeof(struct sockaddr_in));
	return iRetval;
	
}
int send_packet(int hSocket,char* msg,short lenRqst) {
    int shortRetval = -1;
    struct timeval tv;
    tv.tv_sec = 20;  /* 20 Secs Timeout */
    tv.tv_usec = 0;

    if(setsockopt(hSocket,SOL_SOCKET,SO_SNDTIMEO,(char *)&tv,sizeof(tv)) < 0) {
        printf("Time Out\n");
        return -1;
    }
    shortRetval = send(hSocket, msg, lenRqst, RW_FLAG);
    return shortRetval;
}
int recieve_packet(int hsocket, char *msg, short lenMsg)
{
	int iRetval = -1;
	iRetval = recv(hsocket, msg, lenMsg, RW_FLAG);
	return iRetval;
}

int main()
{
	int hsock = 0,status;
	char rec_msg[REC_PACKET_SIZE];
	char *trans_msg = "Hello from Khan's client!";
	hsock = create_socket();
	if(hsock < 0)
	{
		printf("socket creation failed!\n");
		return ERROR;
	}
	printf("socket creation succeeded\n");
	status = connect_socket(hsock);
	if(status < 0)
	{
		printf("connection failed!\n");
		return ERROR;
	}
	printf("connection succeeded\n");

	status = send_packet(hsock, trans_msg, strlen(trans_msg));
	if(status < 0)
	{
		printf("sending failed!\n");
		return ERROR;
	}
	status = recieve_packet(hsock,rec_msg, sizeof(rec_msg));
	if(status < 0)
	{
		printf("recieve failed!\n");
		return ERROR;
	}
	printf("%s\n", rec_msg);
	close(hsock);
	sleep(10);

}
