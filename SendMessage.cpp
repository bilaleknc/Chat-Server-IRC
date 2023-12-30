#include "Server.hpp"

void Server::sendPrivateMessage(int fd, std::string nickName, std::string message)
{
	User *user = getUserbyFd(fd);
	for (vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (it->getNickName() == nickName)
		{
			message = "Client " + user->getNickName() + " :" + message + "\n";
			send(it->getFd(), message.c_str(), message.length(), 0);
			break;
		}
	}
}

void Server::sendChannelMessage(int fd, std::string channelName, std::string message)
{
	Channel *channel = getChannelbyName(channelName);
	if (channel != nullptr)
	{
		message = "Client " + getUserbyFd(fd)->getUserName() + " : " + message;
		channel->sendMessageAllUsers(fd, message);
	}
}

void Server::sendServerMessage(int fd, std::string message)
{
	User *user = getUserbyFd(fd);
	if (user != nullptr)
	{
		message = "Client " + user->getUserName() + " : " + message;
		send(this->getServerFd(), message.c_str(), message.length(), 0);
	}
}