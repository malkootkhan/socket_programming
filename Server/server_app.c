#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define ADDRESS_IPV4	AF_INET		/*AF_INET is a macro with value 2 and supports IPv4 addrss family*/
#define ADDR_IPV6	AF_INET6	/*AF_INET6 is macro for IPv6 address family*/
#define TCP_SOCK	SOCK_STREAM	/*SOCK_STREAM represents TCP*/
#define UDP_SOCK	SOCK_DGRAM	/*SOCK_DGRAM represents UDP*/
#define CLIENT_COUNT	(3)		/*Server can wait on Three client*/
#define REC_PACKET_SIZE	(200)		/*recieved packet size:number of bytes*/
#define RW_FLAG		(0)		/*some flags required by accept api*/
#define ANY_INTERFACE	INADDR_ANY	/*It means server can accept connection on any communication interface of host*/


enum {FALSE, TRUE}bool_t;
enum {ERROR = -1}error_t;
/*Server application*/

int create_socket(void)
{
	/*socket creating code*/
	short hsocket =0;
	printf("Creating socket....\n");
	hsocket = socket(ADDRESS_IPV4, TCP_SOCK, 0);
	return hsocket;
}
int bind_created_socket(int hsock)
{
	/*binding code*/
	int iRetval = -1;
	int client_port = 12348;
	struct sockaddr_in remote={0};
	remote.sin_family = AF_INET;
	remote.sin_addr.s_addr = htonl(ANY_INTERFACE);
	remote.sin_port = htons(client_port);

	iRetval = bind(hsock, (struct sockaddr *)&remote, sizeof(remote));
	return iRetval;	
}
void error(const char *err)
{
	printf("%s\n", err);
	exit(1);

}
int main()
{

	int socket_desc = 0, sock=0, clientLen=0,status;
	struct sockaddr_in client;
	char client_message[REC_PACKET_SIZE] = {0};
	char message[100] = {0};
	const char *pMessage = "Hello from Khan's client!";

	socket_desc = create_socket();
	if(socket_desc < 0)
		error("creating socket failed!\n");

	printf("Socket creation succeeded\n");
	status = bind_created_socket(socket_desc);
	if(status < 0)
		error("binding failed!\n");

	printf("binding succeeded\n");
	listen(socket_desc, CLIENT_COUNT);		/*number of client to be waiting*/
	/*accept connection*/
	while(TRUE)
	{
		printf("Waiting for connections....\n");
		clientLen = sizeof(struct sockaddr_in);
		sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&clientLen);
		if(sock < 0)
			error("accept failed!\n");

		printf("connection accepted!\n");
		//memset(client_message, '\0', sizeof(client_message));
		//memset(message, '\0', sizeof(message));
		bzero(client_message, sizeof(client_message));	/*It is a dedicated function to initialize the memory with 0*/
		bzero(message, sizeof(message));

		status = recv(sock, client_message, REC_PACKET_SIZE, RW_FLAG);
		if(status < 0)
			error("packet reception failed!\n");

		printf("client response: %s\n", client_message);

		if(strcmp(pMessage, client_message) == 0)
			strcpy(message, "Hi there from server!");
		else
			strcpy(message, "Invalid response!");

		status = send(sock,message, strlen(message), RW_FLAG);
		if(status < 0)
			error("send failed\n");
		close(sock);
		sleep(10);
	}

	return 0;
}
	


