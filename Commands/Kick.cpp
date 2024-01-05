#include "../Server.hpp"

void	Server::KICK(User &user)
{
	std::string name = this->getCommands()[1];
	std::string nickname = this->getCommands()[2];
	std::string reason;
	
	User *targetUser = this->getUserbyNickName(nickname);
	Channel *channel = getChannelbyName(name);
	std::vector<int> users = channel->getUsers();
	std::vector<int> operators = channel->getOperators();

	if (targetUser == nullptr || channel == nullptr)
		return;
	for (size_t i = 0; i < users.size(); i++)
	{
		if (users[i] == user.getFd())
			break;
		if (i == users.size() - 1)
		{
			send(user.getFd(), "You are not on the channel\n", 27, 0);
			return;
		}
	}
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
		for (size_t j = 0; j < users.size(); j++)
		{
			if (users[j] == targetUser->getFd())
			{
				channel->removeUser(targetUser->getFd());
				std::string kick = ":" + user.getNickName() + "!" + user.getUserName() + "@" + this->ip + " KICK " + name + " " + nickname + " :" + reason + "\r\n";
				send(user.getFd(), kick.c_str(), kick.length(), 0);
				std::cout << user.getNickName() << " kicked " << nickname << " from " << name << std::endl;
				return;
			}
		}
	}
	else
	{
		send(user.getFd(), "Wrong channel name\n", 19, 0);
	}
}