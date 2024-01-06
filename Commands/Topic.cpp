#include "../Server.hpp"

void	Server::TOPIC(User &user)
{
	std::string name = this->getCommands()[1];
	std::string topic;
	if (this->getCommands().size() < 3)
		return;
	if (this->getCommands()[2][0] != ':')
		return;
	for (size_t i = 2; i < this->getCommands().size(); i++)
	{
		topic += " " + this->getCommands()[i];
	}
	if (name[0] == '#')
	{
		Channel *channel = getChannelbyName(name);
		if (channel == nullptr)
		{
			std::cout << "Channel not found" << std::endl;
			return;
		}
		std::vector<int> operators = channel->getOperators();
		for (size_t i = 0; i < operators.size(); i++)
		{
			if (operators[i] == user.getFd())
				break;
			if (i == operators.size() - 1)
			{
				send(user.getFd(), "You are not operator\n", 21, 0);
				return;
			}
		}
		channel->setTopic(topic);
		send(user.getFd(), RPL_TOPIC(name, topic).c_str(), RPL_TOPIC(name, topic).length(), 0);
	}
	else
	{
		send(user.getFd(), RPL_NOTOPIC(name).c_str(), RPL_NOTOPIC(name).length(), 0);
	}
}