// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "SocketClient.h"
#define PORT 8080


SocketClient::SocketClient(string ipaddress,int ipport):ip(ipaddress),port(ipport){
	
}

SocketClient::~SocketClient(){
	
}

int SocketClient::setup(){
	
	char *hello = "Hello from client";
	char buffer[1024] = {0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}
}


void SocketClient::writeSocketClient(uint8_t value){
	char msg[3] = {0x02,value,'\r'};
	send(sock, msg, strlen(msg), 0);
}

const uint8_t *SocketClient::readSocketClient(){
	char msg[2] = {0x01,'\r'};
	send(sock, msg, strlen(msg), 0);
	uint8_t buffer[1024] = {0};
	valread = read( sock , buffer, 1024);
	return buffer;
}

