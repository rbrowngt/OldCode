//
//  server.c
//  Project1 3251
//
//  Created by Ronald Brown on 9/19/11.
//  Copyright 2011 Georgia Institute of Technology. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Practical.h"

static const int MAXPENDING = 5; // Maximum outstanding connection requests
static const char *dlMode = "ttweet_REQUEST"; // the 'keyword' we use to determine
int main(int argc, char *argv[]) {
    
    if (argc != 2) // Test for correct number of arguments
        DieWithUserMessage("Parameter(s)", "<Server Port>");
    
    in_port_t servPort = atoi(argv[1]); // First arg:  local port
    
    // Create socket for incoming connections
    int servSock; // Socket descriptor for server
    if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithSystemMessage("socket() failed");
    
    // Construct local address structure
    struct sockaddr_in servAddr;                  // Local address
    memset(&servAddr, 0, sizeof(servAddr));       // Zero out structure
    servAddr.sin_family = AF_INET;                // IPv4 address family
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Any incoming interface
    servAddr.sin_port = htons(servPort);          // Local port
    
    // Bind to the local address
    if (bind(servSock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0)
        DieWithSystemMessage("bind() failed");
    
    // Mark the socket so it will listen for incoming connections
    if (listen(servSock, MAXPENDING) < 0)
        DieWithSystemMessage("listen() failed");
    
    char message1[128];
    strcpy(message1, "Empty String");
    char message2[128];
    strcpy(message2, "Empty String");
    
    int count = 0;
    int whichMsg = 1;
    int pntMode = 0;
    
    for (;;) { // Run forever
        struct sockaddr_in clntAddr; // Client address
        // Set length of client address structure (in-out parameter)
        socklen_t clntAddrLen = sizeof(clntAddr);
        
        // Wait for a client to connect
        int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
        if (clntSock < 0)
            DieWithSystemMessage("accept() failed");
        
        // clntSock is connected to a client!
        
        char clntName[INET_ADDRSTRLEN]; // String to contain client address
        if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName,
                      sizeof(clntName)) != NULL)
            printf("Handling client %s/%d\n", clntName, ntohs(clntAddr.sin_port));
        else
            puts("Unable to get client address");
        
       char *p = HandleTCPClient(clntSock);
       
       //Check what mode we're in
       pntMode = strcmp(dlMode, p) == 0 ? 1 : 0;
       
      if(pntMode == 0) { 
	
	    //Determine where to add this message
	    if(count == 0) { 
	      if(strlen(p) > 1) {
		strcpy(message1, p);
		count++;
	      }
	    }
	    else if(count == 1) {
		if(strlen(p) > 1) {
		  strcpy(message2, p);
		  count++;
		}
	    }
	    else if(count == 2) {
	      if(strlen(p) > 1) {
		strcpy(message1, message2);
		strcpy(message2, p);
	      }
	    }
	      
	      
	      
	  }
	  

      //If the mode is -d, return current messages saved on the server
      else if(pntMode == 1) {
	  char finBuf[BUFSIZE];
	  char szBuf[4];
	  
	  snprintf(finBuf, sizeof(finBuf), "%s%s%s%s", "Message 1: ", message1, "\n Message 2: ", message2);
	  ssize_t nSizeBytes;
	  szBuf[0] = strlen(finBuf);
	  nSizeBytes = send(clntSock, szBuf, 3, 0);
	  if(nSizeBytes < 0)
	    DieWithSystemMessage("send() failed at sending size of message string");
	 
	  printf("Size of messages being transmitted %i\n", strlen(finBuf));

	  ssize_t numBytes;
	  numBytes = send(clntSock, finBuf, BUFSIZE, 0);
          if (numBytes < 0)
            DieWithSystemMessage("send() failed");
	  
	  close(clntSock);
	  pntMode = 0;

	
	  }
	 
       }
	
    
    // NOT REACHED
}