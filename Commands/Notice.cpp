#include "../Server.hpp"

void Server::NOTICE(User &user)
{
	std::string name = this->getCommands()[1];
	std::string message;
	if (this->getCommands()[2][0] != ':')
	{
		send(user.getFd(), "Wrong command\n", 14, 0);
		return;
	}
	for (size_t i = 2; i < this->getCommands().size(); i++)
	{
		if(i == 2)
			message += this->getCommands()[i].substr(1, this->getCommands()[i].length() - 1);
		else
			message += " " + this->getCommands()[i];
	}

	if (name[0] == '#')
	{
		Channel *channel = getChannelbyName(name);
		if (channel == nullptr)
		{
			std::cout << "Channel not found" << std::endl;
			return;
		}

		sendChannelMessage(user.getFd(), name, message);
	}
	else
	{
		sendPrivateMessage(user.getFd(), name, message);
	}
}