#include "Server.hpp"

Server::Server(std::string ip, int port, std::string password)
{
	this->port = port; 
	this->ip = ip;
	this->password = password;
	address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(this->port);
	this->fds = new struct pollfd[100];
	this->buffer = new char[1024];
	this->opt = 1;
}

Server::~Server()
{
	delete[] this->fds;
	free(buffer);
}

Server::Server(const Server &other)
{
	this->port = other.port;
	this->ip = other.ip;
	this->password = other.password;
}


Server &Server::operator=(const Server &other)
{
	this->port = other.port;
	this->ip = other.ip;
	this->password = other.password;
	return (*this);
}

void Server::createServer()
{
	this->setServerFd(socket(AF_INET, SOCK_STREAM, 0));
	fcntl(this->serverFd, F_SETFL, O_NONBLOCK);	
	setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &(this->opt), sizeof(opt));

	bind(this->serverFd, reinterpret_cast<const struct sockaddr *>(&(this->address)), sizeof(struct sockaddr_in));
	listen(this->serverFd, 3);
	memset(this->getFds(), 0, 100);
	this->fds[0].fd = this->serverFd;
	this->fds[0].events = POLLIN;
}

void Server::start()
{
	std::cout << "Server started" << std::endl;
	while (1)
	{
		poll(this->fds, 10, -1);
		userAccept();
		for(int fd = 4; fd < 10; fd++)
		{
			if (this->getFds()[fd].revents & POLLIN)
			{
				readMessage(fd);
				parseMessage();
				controlMessage(fd);
			}
		}
	}
}