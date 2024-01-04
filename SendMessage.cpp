#include "Server.hpp"

void Server::sendPrivateMessage(int fd, std::string nickName, std::string message)
{
	User *user = getUserbyFd(fd);
	if(fd == this->getUserbyNickName(nickName)->getFd())
		return;
	if (user == nullptr || !user->getIsLogin() || !user->getIsActive())
		return;
	for (vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (it->getNickName() == nickName)
		{
			message = "Client " + user->getNickName() + " : " + message + "\n";
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
		if (getUserbyFd(fd) != nullptr)
		{
			message = channel->getChannelName() + " Client " + getUserbyFd(fd)->getUserName() + " : " + message + "\n";
			channel->sendMessageAllUsers(fd, message);
			if (fd != serverFd)
				std::cout << message << std::endl;
		}
		else
		{
			std::cout << "User not found" << std::endl;
			send(fd, "User not found\n", 15, 0);
		}
	}
}