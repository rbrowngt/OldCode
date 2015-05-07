# Makefile
# CS 3251
# Ronald Brown
# Project 1

SUBMIT = *.c *.h *.o ttweet server Makefile README.txt

all: 
	gcc -g -O -c TCPServerUtility.c Practical.c AddressUtility.c 
	gcc -o ttweet client.c AddressUtility.o Practical.o TCPServerUtility.o
	gcc -o server server.c AddressUtility.o Practical.o TCPServerUtility.o
debug:
	gcc -g -O -c TCPServerUtility.c Practical.c AddressUtility.c 
	gcc -g -o ttweet client.c AddressUtility.o Practical.o TCPServerUtility.o
	gcc -g -o server server.c AddressUtility.o Practical.o TCPServerUtility.o

submit: 
	tar cvfz Brown.tar.gz $(SUBMIT)

clean: 
	rm -f *.o ttweet server



