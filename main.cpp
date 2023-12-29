#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <poll.h>
#include <arpa/inet.h>
#define MAX_CLIENTS 10
#include "Server.hpp"

int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		std::cout << "Usage: ./server <port> <ip> <password>" << std::endl;
		return (1);
	}

	Server server(argv[1], atoi(argv[2]), argv[3]);
	server.createServer();
	server.start();
	close(server.getServerFd());
	return (0);
}