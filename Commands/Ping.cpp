#include "../Server.hpp"

void Server::PING(User &user)
{
	if (this->getCommands().size() < 2)
		return;
	std::string message = "PONG " + this->getCommands()[1] + "\r\n";
	send(user.getFd(), message.c_str(), message.length(), 0);
}