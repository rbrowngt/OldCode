#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Practical.h"

int main(int argc, char *argv[]) {

  if (argc < 4 || argc > 6) // Test for correct number of arguments
      DieWithUserMessage("Please use correct format",
		       " < -u or -d> < ServerIP> <ServerPort> <Message(not required if -d mode)> Note: If you wish to type a sentence, please surround in quotes ");
	
    
	  

  char *servIP = argv[2];     // First arg: server IP address (dotted quad)
  char *cmdType = argv[1];
  char *echoString = (argc == 5) ? argv[4] : "ttweet_REQUEST"; // Second arg: string to echo
   //If the argc is 4, make sure string is formatted correctly, meaning -d is used and not -u
    if((argc == 4) && (strcmp(cmdType, "-d")!= 0)) {
      DieWithUserMessage("Please use correct format",
		       " < -u or -d> < ServerIP> <ServerPort> <Message(not required if -d mode)> Note: If you wish to type a sentence, please surround in quotes");
    }
    
    if((argc == 5) && (strcmp(cmdType, "-u")!= 0)) {
      DieWithUserMessage("Please use correct format",
		       " < -u or -d> < ServerIP> <ServerPort> <Message(not required if -d mode)> Note: If you wish to type a sentence, please surround in quotes");
    }
    
    if(argc == 5 && (strlen(echoString) < 1 || strcmp(echoString, " ") == 0 ))
      DieWithUserMessage("Please input a string",
			 " of length 1 or more, no empty strings. Note: If you wish to type a sentence, please surround in quotes");
    
  in_port_t servPort = atoi(argv[3]);

  // Create a reliable, stream socket using TCP
  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock < 0)
    DieWithSystemMessage("socket() failed");

  // Construct the server address structure
  struct sockaddr_in servAddr;            // Server address
  memset(&servAddr, 0, sizeof(servAddr)); // Zero out structure
  servAddr.sin_family = AF_INET;          // IPv4 address family
  // Convert address
  int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);
  if (rtnVal == 0)
    DieWithUserMessage("inet_pton() failed", "invalid address string");
  else if (rtnVal < 0)
    DieWithSystemMessage("inet_pton() failed");
  servAddr.sin_port = htons(servPort);    // Server port

  // Establish the connection to the echo server
  if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
    DieWithSystemMessage("connect() failed");

  
  //if argc is 5, then we have an upload.  Simply send the message to the server and if success, print
  if(argc == 5) {
      size_t echoStringLen = strlen(echoString); // Determine input length
      //Check to see if string length > 30
      if(echoStringLen > 60) {
	DieWithSystemMessage("String length > 60, please re ttweet with a message less then 60 characters");
	return;
      }
      // Send the string to the server
      ssize_t numBytes;
      
      numBytes = send(sock, echoString, echoStringLen, 0);
      
      if (numBytes < 0)
	DieWithSystemMessage("send() failed");
      else if (numBytes != echoStringLen)
	DieWithUserMessage("send()", "sent unexpected number of bytes");

      // Receive the same string back from the server
      unsigned int totalBytesRcvd = 0; // Count of total bytes received
      while (totalBytesRcvd < echoStringLen) {
	char buffer[BUFSIZE]; // I/O buffer
	/* Receive up to the buffer size (minus 1 to leave space for
	a null terminator) bytes from the sender */
	numBytes = recv(sock, buffer, BUFSIZE - 1, 0);
	if (numBytes < 0)
	  DieWithSystemMessage("recv() failed");
	else if (numBytes == 0)
	  DieWithUserMessage("recv()", "connection closed prematurely");
	totalBytesRcvd += numBytes; // Keep tally of total bytes
	buffer[numBytes] = '\0';    // Terminate the string!
	fputs("Successful ttweet of ", stdout);
	fputs(buffer, stdout);      // Print the echo buffer
      }

      fputc('\n', stdout); // Print a final linefeed
  }
  
  
  if(argc == 4) {
    //Just recieve whatever
    size_t echoStringLen = strlen(echoString); // Determine input length
      //Check to see if string length > 30
      if(echoStringLen > 60) {
	DieWithSystemMessage("String length > 60, please re ttweet with a message less then 60 characters");
	return;
      }
      // Send the string to the server
      ssize_t numBytes;
      
      numBytes = send(sock, echoString, echoStringLen, 0);
      
      if (numBytes < 0) {
	DieWithSystemMessage("send() failed"); 
	return;
      }
      else if (numBytes != echoStringLen) {
	DieWithUserMessage("send()", "sent unexpected number of bytes");
	return;
      }
 
      //Recieve the size of the buffer
      int bufSize = 0, tlBytes = 0, ttlBytes = 0, nBytes = 0;
      char bufSizeBuf[4];
      
      while(ttlBytes < 4) {
	nBytes = recv(sock, bufSizeBuf, 3, 0);
	ttlBytes += numBytes;
      }
      //Anticipated buffer size
      bufSize = *(int *)bufSizeBuf;
      
      //Get ready to recieve the messages from server
      char buffer[bufSize + 1];
      while(tlBytes < bufSize) {
      
	numBytes = recv(sock, buffer, bufSize, 0);
	tlBytes +=numBytes;
	buffer[numBytes] = '\0';
	//Print out contents
	fputs("Messages Recieved:\n ", stdout);
	fputs(buffer, stdout);
	printf("\n");
    
     }
     
     
      
  }

  close(sock);
  exit(0);
}
