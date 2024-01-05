#include "../Server.hpp"

void Server::MODE(User &user)
{
	//MODE #channel +o nick
	//MODE #channel -o nick
	std::string name = this->getCommands()[1];
	std::string mode = this->getCommands()[2];
	if (name[0] == '#')
	{
		Channel *channel = getChannelbyName(name);
		if (channel == nullptr)
		{
			std::cout << "Channel not found" << std::endl;
			return;
		}
		if (mode[0] == '+')
		{
			if (mode[1] == 'o')
			{
				for (size_t i = 0; i < channel->getUsers().size(); i++)
				{
					if (channel->getUsers()[i] == user.getFd())
					{
						channel->addOperator(user.getFd());
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
					if (channel->getUsers()[i] == user.getFd())
					{
						channel->removeOperator(user.getFd());
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