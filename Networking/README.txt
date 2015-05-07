Ronald Brown
CS 3251 Networking I

Readme

How to Compile
-----------------------

If you cd to the directory where you extracted the .tar.gz file, you should see a makefile exists.  If you run make in the terminal, this will compile and link all files as needed.  It will generated a "server" and "client" binary files that can be run.  The makefile also contains commands:  debug, submit, and clean.  

How to run
-----------------------
Server:
To execute the server, simply run 

./server [Port] 

The server will not execute without a port provided

Client:
To execute the client (assuming this is being done on the same computer that is currently running the server) simply type one of two things:

Downlading messages currently saved on the server
./ttweet -d 127.0.0.1 [Same port as server]

Uploading messages to the server can be done one of two ways.  Either sending simply a word, this can be done without quotes. 

./ttweet -u 127.0.0.1 [Port of server] Test

Or if you wish to send a phrase between 1 and 60 characters use quotes. i.e.:

./ttweet -u 127.0.0.1 [Port of server] "The Quick Brown Fox2"

Please remember to use a high port number if you are not running as root/superuser.


Protocol Documentation
-----------------------
	The protocol used for this project was implemented on top of the standard TCP connect.  It makes used of two different types of datagram packets that are setup depending on on the ttweet 'flag' used when uploading or downloading a flag.  If the user passes in an upload flag, the ttweet client generates a standard packet with the data contents of the packet being whatever the user passed in.  However, if the download or '-d' flag is passed in, a packet is generated in a slightly different way.  The data portion of the packet contains a pre-defined keyword of 'ttweet_REQUEST'.  
	On the server side, the packet is both types of packets are received the exact same way however if the '-d' download flag was passed into the client, the server will look for the flag and also identify the 'ttweet_REQUEST' data portion of the packet.  Upon identification of a 'download' request, the server returns from it's receive. (NOTE:In a normal upload packet, an acknowledgment packet is sent back to the client to let them know they have successfully transmitted their data.) The ttweet client knows that it has sent a download message request so it begins to wait to hear a response from the server, expecting first the size of the buffer to accept and then the buffer itself.  The server sends first the size of the buffer in containing the two messages.  This is then followed by the buffer itself.  On the client side, the client parses the size and allocates a buffer of said size to read in the message.  The client then waits for the server to transmit the downloaded messages and checks to make sure the size of the data received indeed matches the size expected.  



References Used
-----------------------
http://cs.ecs.baylor.edu/~donahoo/practical/CSockets2/textcode.html
Files:
TCPEchoClient4.c
AddressUtility.c
Practical.c
Practical.h
TCPServerUtility.c