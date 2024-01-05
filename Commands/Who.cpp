#include "../Server.hpp"

void Server::WHO(User &user)
{
	std::string name = this->getCommands()[1];
	Channel *channel = getChannelbyName(name);
	if (channel == nullptr)
		return;  
	for (size_t i = 0; i < channel->getUsers().size(); i++)
	{
		User *user2 = getUserbyFd(channel->getUsers()[i]);
		send(user.getFd(), RPL_WHOREPLY(name, user2->getNickName(), user2->getUserName(), this->getIp(), "irc.example.com", user2->getNickName(), "1", user2->getRealName()).c_str(), RPL_WHOREPLY(name, user2->getNickName(), user2->getUserName(), this->getIp(), "irc.example.com", user2->getNickName(), "1", user2->getRealName()).length(), 0);
	}
	send(user.getFd(), RPL_ENDOFWHO(name).c_str(), RPL_ENDOFWHO(name).length(), 0);
}