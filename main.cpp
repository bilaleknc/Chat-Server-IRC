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
#include "server.hpp"

int main(int argc, char const *argv[])
{
	if (argc != 4)
	{
		std::cout << "Usage: ./server <port> <ip> <password>" << std::endl;
		return (1);
	}

	Server server(1313, "127.0.0.1", "1234");
	server->createServer(std::string(argv[1]), atoi(argv[2]), std::string(argv[3]));
	// server->start();
	close(server->getServer_fd());
	return (0);
}