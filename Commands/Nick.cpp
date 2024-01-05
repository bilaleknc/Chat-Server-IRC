#include "../Server.hpp"

void Server::NICK(User &user)
{
	std::string nick = this->getCommands()[1];
	if(this->getUserbyNickName(nick) != nullptr)
	{
		send(user.getFd(), "Nickname already taken\n", 23, 0);
		return;
	}
	if (this->getCommands().size() == 2)
	{
		user.setNickName(this->getCommands()[1]);
	}
}