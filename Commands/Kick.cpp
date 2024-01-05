#include "../Server.hpp"

void	Server::KICK(User &user)
{
	// KICK #channel nickname :reason
	std::string name = this->getCommands()[1];
	std::string nickname = this->getCommands()[2];
	std::string reason;
	if (this->getCommands().size() > 3)
	{
		if (this->getCommands()[3][0] != ':')
		{
			send(user.getFd(), "Wrong command\n", 14, 0);
			return;
		}
		for (size_t i = 3; i < this->getCommands().size(); i++)
		{
			if(i == 3)
				reason += this->getCommands()[i].substr(1, this->getCommands()[i].length() - 1);
			else
				reason += " " + this->getCommands()[i];
		}
	}
	if (name[0] == '#')
	{
		Channel *channel = getChannelbyName(name);
		if (channel == nullptr)
		{
			std::cout << "Channel not found" << std::endl;
			return;
		}
		for (size_t i = 0; i < channel->getUsers().size(); i++)
		{
			if (channel->getUsers()[i] == user.getFd())
			{
				if (channel->getOperators()[i] == user.getFd() || channel->getAdmins()[i] == user.getFd())
				{
					for (size_t j = 0; j < channel->getUsers().size(); j++)
					{
						if (channel->getUsers()[j] == getUserbyNickName(nickname)->getFd())
						{
							channel->removeUser(getUserbyNickName(nickname)->getFd());
							std::string kick = ":" + user.getNickName() + "!" + user.getUserName() + "@" + this->ip + " KICK " + name + " " + nickname + " :" + reason + "\r\n";
							send(user.getFd(), kick.c_str(), kick.length(), 0);
							return;
						}
					}
				}
				else
				{
					send(user.getFd(), "You are not operator or admin\n", 30, 0);
					return;
				}
			}
		}
	}
	else
	{
		send(user.getFd(), "Wrong channel name\n", 19, 0);
	}
}