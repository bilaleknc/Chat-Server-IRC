#include "Server.hpp"

void Server::sendPrivateMessage(int fd, std::string nickName, std::string message)
{
	User *user = getUserbyFd(fd);
	if (user == nullptr || !user->getIsLogin() || !user->getIsActive())
		return;
	User *dst = getUserbyNickName(nickName);
	if (dst == nullptr || !dst->getIsLogin() || !dst->getIsActive() || dst->getFd() == fd)
		return;
	if (fd == dst->getFd())
		return;
	for (vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (it->getNickName() == nickName)
		{
			std::string buffer = ":" + user->getNickName() + "!" + user->getUserName() + "@" + this->ip + " PRIVMSG " + nickName + " :" + message + "\r\n";
			send(it->getFd(), buffer.c_str(), buffer.size(), 0);
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
			std::string buffer = ":" + getUserbyFd(fd)->getNickName() + "!" + getUserbyFd(fd)->getUserName() + "@" + this->ip + " PRIVMSG " + channelName + " :" + message + "\r\n";
			channel->sendMessageAllUsers(fd, buffer);
			
		}
		else
		{
			std::cout << "User not found" << std::endl;
			send(fd, "User not found\n", 15, 0);
		}
	}
}
