#include "Server.hpp"

void Server::userAccept()
{
	int new_socket;

	if (this->getFds()[0].revents & POLLIN)
	{
		if ((new_socket = accept(this->serverFd, (struct sockaddr *)&(this->address), &(this->addrlen))) > 0)
		{
			std::cout << "New connection" << std::endl;
			this->createUser(new_socket);
			std::cout << "Client " << new_socket << " connected" << std::endl;
			send(new_socket, "Welcome to the server\n", 23, 0);
			fds[new_socket].fd = new_socket;
			fds[new_socket].events = POLLIN;
		}
	}
}

void Server::createUser(int fd)
{
	User user(fd);
	this->users.push_back(user);
}

void Server::createChannel(std::string channelName)
{
	Channel channel(channelName);
	this->channels.push_back(channel);
	std::cout << channelName << " created successfully" << std::endl;
}

void Server::removeUser(int fd)
{
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (it->getFd() == fd)
		{
			close(fd);
			users.erase(it);
			std::cout << it->getNickName() << " deleted" << std::endl;
			break;
		}
	}
}

void Server::removeChannel(std::string channelName)
{
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (it->getChannelName() == channelName)
		{
			channels.erase(it);
			std::cout << channelName << " deleted" << std::endl;
			break;
		}
	}
}