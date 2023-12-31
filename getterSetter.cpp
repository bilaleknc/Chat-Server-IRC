#include "Server.hpp"

Channel *Server::getChannelbyName(std::string name)
{
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (it->getChannelName() == name)
		{
			return (&(*it));
		}
	}
	return nullptr;
}

User *Server::getUserbyNickName(std::string name)
{
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (it->getNickName() == name)
		{
			return (&(*it));
		}
	}
	return nullptr;
}

User *Server::getUserbyFd(int fd)
{
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (it->getFd() == fd)
		{
			return (&(*it));
		}
	}
	return nullptr;
}

int Server::getPort() const
{
	return (this->port);
}

std::string Server::getIp() const
{
	return (this->ip);
}

std::string Server::getPassword() const
{
	return (this->password);
}

int Server::getServerFd() const
{
	return (this->serverFd);
}

struct pollfd *Server::getFds() const
{
	return (this->fds);
}

const struct sockaddr_in &Server::getAddress() const
{
	return this->address;
}

socklen_t &Server::getAddrlen()
{
	return (this->addrlen);
}

char *Server::getBuffer() const
{
	return (this->buffer);
}

void Server::setCommands(std::string command){
	this->commands.push_back(command);
}

vector<std::string> Server::getCommands() const{
	return this->commands;
}

void Server::setPort(int port)
{
	this->port = port;
}

void Server::setIp(int ip)
{
	this->ip = ip;
}

void Server::setPassword(int password)
{
	this->password = password;
}

void Server::setServerFd(int server_fd)
{
	this->serverFd = server_fd;
}

void Server::setAddress(struct sockaddr_in address)
{
	this->address = address;
}

void Server::setAddrlen(socklen_t addrlen)
{
	this->addrlen = addrlen;
}