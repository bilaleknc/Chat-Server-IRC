
#include "Server.hpp"

void Server::readMessage(int fd)
{
	memset(buffer, 0, 1024);
	int valread = recv(this->getFds()[fd].fd, buffer, 1024, 0);
	if (valread == 0)
	{
		close(this->getFds()[fd].fd);
		this->getFds()[fd].fd = 0;
	}
}

void Server::parseMessage()
{
	std::string buffer(this->buffer);
	if (buffer.length() == 0)
		return;
	this->commands.clear();
	for (size_t i = 0; i < buffer.length(); i++)
	{
		if (buffer[i] == '\n' || buffer[i] == '\r')
			buffer[i] = ' ';
	}
	std::istringstream iss(buffer);
	std::string token;
	while (std::getline(iss, token, ' '))
	{
		if (token.length() > 0)
			this->commands.push_back(token);
	}
	buffer.clear();
}

void Server::controlMessage(int fd)
{
	User *user = getUserbyFd(fd);
	void (Server::*tools[])(User &user) = {&Server::PASS, &Server::NICK, &Server::USER, &Server::PRIVMSG, &Server::JOIN, &Server::PART, 
			&Server::QUIT, &Server::TOPIC, &Server::NOTICE, &Server::PING, &Server::WHO, &Server::MODE, &Server::KICK, &Server::LIST};
	std::string commands[] = {"PASS", "NICK", "USER", "PRIVMSG", "JOIN", "PART", "QUIT", "TOPIC", "NOTICE", "PING", "WHO", "MODE", "KICK", "LIST"};
	for (size_t i = 0; i < 14; i++)
	{
		if ((!user->getIsLogin() || !user->getIsActive()) && i > 3)
			break;

		if (!this->getCommands().empty() && (this->getCommands()[0].compare(commands[i]) == 0))
		{
			(this->*tools[i])(*user);
			break;
		}
		if (i == 13)
			send(user->getFd(), "Command not found\n", 18, 0);
	}
	if (!user->getIsLogin())
		send(user->getFd(), "Please login\n", 13, 0);
	else if (user->getNickName().length() == 0)
		send(user->getFd(), "Please enter your nickname\n", 27, 0);
	else if (user->getUserName().length() == 0)
		send(user->getFd(), "Please enter your username\n", 27, 0);
}

