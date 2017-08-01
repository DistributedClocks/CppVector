#include "../src/CppVec.h"

#include <iostream>

/* For networking */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

#define SERVER_PORT 8887
#define CLIENT_PORT 8888
#define SERVERIP "127.0.0.1"
#define MAXBUFLEN 100
#define MESSAGES 10

/*
Compile using:
g++ -W -Wall -Werror -std=c++11 client-server.cpp ../src/CppVec.cpp ../src/VClock/VClock.cpp
*/

int initSocket(int portno) {

	struct sockaddr_in addr;
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
       cout << "ERROR opening socket" << endl;
       exit(0);
    }
    
    cout << "Socket opened on " << sockfd << endl;
    bzero((char *) &addr, sizeof(addr));

    // Set the appropriate fields in the server address
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
		cout << "ERROR on binding" << endl;
    	exit(0);
    }    

    return sockfd;

}

int server() {

	cout << "Start server" << endl;

	CppVec cv;
	cv.initCppVector("server", "serverLogFile");

	int sockfd;
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    
    sockfd = initSocket(SERVER_PORT);

    int i;
    int n = 0, nMinOne = 0, nMinTwo = 0;
    char inBuf[MAXBUFLEN];
    bzero(inBuf,MAXBUFLEN);

    for (i = 0; i < MESSAGES; i++) {

    	int nBytes = recvfrom(sockfd, inBuf, MAXBUFLEN-1, 0,
            (struct sockaddr*) &cli_addr, &clilen);
    
	    if (nBytes < 0) {
	    	cout << "ERROR reading from socket" << endl;
	    	exit(0);
	    }

	    int reply = 0;
	    cv.unpackReceive("Received message from client.", inBuf, &reply, MAXBUFLEN-1);

	    printf("Here is the message: %d. \n", reply);

	    /* Calculate fibonacci */
        if (reply == 0){
                nMinTwo = 0;
                n = 0;
        } else if (reply == 1){
                nMinOne = 0;
                n = 1;
        } else {
            nMinTwo = nMinOne;
            nMinOne = n;
            n = nMinOne + nMinTwo;
        }

	    const char* outBuf = cv.prepareSend("Sending message to client", n);
	    nBytes = sendto(sockfd, outBuf, MAXBUFLEN, 0, (struct sockaddr*) &cli_addr, clilen);
	    
	    if (nBytes < 0) {
	    	cout << "ERROR writing to socket" << endl;
	    	exit(0);
	    }

    }

    return 0;
}

int client() {

	cout << "Start client" << endl;
	
	CppVec cv;
	cv.initCppVector("client", "clientLogFile");

	int sockfd;
    struct sockaddr_in serv_addr;
    socklen_t servlen = sizeof(serv_addr);

    // Set up server address 
    bzero((char *) &serv_addr, servlen);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVERIP, &serv_addr.sin_addr);

    // And the client address
	sockfd = initSocket(CLIENT_PORT);

    int i;

    for (i = 0; i < MESSAGES; i++) {
	    
	    // Use GoVector prepareSend, then send the buffer
	    const char* outBuf = cv.prepareSend("Sending message to server", i);

	    int nBytes = sendto(sockfd, outBuf, MAXBUFLEN-1, 0, (struct sockaddr*) &serv_addr, servlen);
	    
	    if (nBytes < 0) {
	    	cout << "ERROR writing to socket" << endl;
	    	exit(0);
	    }
	        
	    char inBuf[MAXBUFLEN];
	    bzero(inBuf, MAXBUFLEN);

	    nBytes = recvfrom(sockfd, inBuf, MAXBUFLEN-1, 0,
	            (struct sockaddr*) &serv_addr, &servlen);
	    
	    if (nBytes < 0) {
	    	cout << "ERROR reading from socket" << endl;
	    	exit(0);
	    }

	    int reply = 0;
	    cv.unpackReceive("Received message from server.", inBuf, &reply, MAXBUFLEN-1);
	        
	    printf("GOT BACK: %d\n", reply);	
    }
    
    return 0;
}

int main() {

	int pid=fork();
    if (pid==0){
        sleep(2);
        client();    
    }
    else{ 
        server();
    }

    // Generate a shiviz log
    //system("./shiviz.sh");

	return 0;

}