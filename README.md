# SOCKET COMMUNICATION (in c programming)
 
 In simple way socket communication involved server and client and the protocol used is TCP/UDP using IP address and port number

 To make socket communication possible we need server application that will use some specific APIs and client application which also uses that similar APIs for socket communication

 The main functions executed are

## Server:
 - create new socket
 - bind socket to local address
 - listen
 - accept
 - connect
 - send/receive
 - close socket
## client:  
 - Create socket
 - Connect
 - send/recieve
 - close
 
## Header files for these APIs
- <sys/types.h> 
- <sys/socket.h>
- <netinet/in.h>
- <netdb.h>
- <arpa/inet.h>

    This code contain both server and client basic applications for communicating over the socket to get familiar with socket programming in c
    To successfully communicating you must first run the server and then run the client app
