#include "../Server.hpp"

void Server::MODE(User &user)
{
	std::string channelName = this->getCommands()[1];
	std::string mode = this->getCommands()[2];
	std::string nick = this->getCommands()[3];

	User *targetUser = this->getUserbyNickName(nick);
	if (targetUser == nullptr)
		return;
	if (channelName[0] == '#')
	{
		Channel *channel = getChannelbyName(channelName);
		if (channel == nullptr)
		{
			std::cout << "Channel not found" << std::endl;
			return;
		}
		for (size_t i = 0; i < channel->getUsers().size(); i++)
		{
			if (channel->getOperators()[i] == user.getFd())
				break;
			if (i == channel->getOperators().size() - 1)
			{
				std::cout << "User is not operator" << std::endl;
				return;
			}
		}
		if (mode[0] == '+')
		{
			if (mode[1] == 'o')
			{
				for (size_t i = 0; i < channel->getUsers().size(); i++)
				{
					if (channel->getUsers()[i] == targetUser->getFd())
					{
						channel->addOperator(targetUser->getFd());
						return;
					}
				}
			}
		}
		else if (mode[0] == '-')
		{
			if (mode[1] == 'o')
			{
				for (size_t i = 0; i < channel->getUsers().size(); i++)
				{
					if (channel->getUsers()[i] == targetUser->getFd())
					{
						channel->removeOperator(targetUser->getFd());
						std::cout << "User " << targetUser->getNickName() << " is not operator anymore" << std::endl;
						return;
					}
				}
			}
		}
	}
	else
	{
		send(user.getFd(), "Wrong channel name\n", 19, 0);
	}
}