#include "../Server.hpp"

void Server::PART(User &user)
{
	if (this->getUserbyFd(user.getFd()) == nullptr)
		return;
	std::string name = this->getCommands()[1];
	if (name[0] == '#')
	{
		Channel *channel = getChannelbyName(name);
		if (channel == nullptr)
		{
			send(user.getFd(), "Channel not found\n", 18, 0);
			return;
		}
		channel->removeUser(user.getFd());
	}
	else
	{
		send(user.getFd(), "Wrong channel name\n", 19, 0);
	}
}