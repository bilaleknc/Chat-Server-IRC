#include "Server.hpp"

void Server::PASS(User &user)
{
	vector<std::string> com = this->getCommands();
	if (com.size() == 9 && !com[2].compare("NICK") && !com[4].compare("USER"))
	{
		if (!this->password.compare(com[1].substr(1, com[1].length() - 1)))
		{
			user.setIsLogin(true);
			user.setIsActive(true);
			user.setNickName(com[3]);
			user.setUserName(com[5]);
			user.setRealName(com[8].substr(1, com[9].length() - 1));

			std::cout << "User " << user.getNickName() << " registered" << std::endl;
		}
	}
	else
	{
		if (this->password.compare(this->getCommands()[1]) == 0)
		{
			user.setIsLogin(true);
			send(user.getFd(), "You are logged in\n", 18, 0);
		}
		else
		{
			send(user.getFd(), "Wrong password\n", 15, 0);
		}
	}
}

void Server::NICK(User &user)
{
	if (this->getCommands().size() == 2)
	{
		user.setNickName(this->getCommands()[1]);
	}
}

void Server::USER(User &user)
{
	if (this->getCommands().size() == 5)
	{
		user.setUserName(this->getCommands()[1]);
		user.setRealName(this->getCommands()[4]);
		user.setIsActive(true);
	}
}

void Server::PRIVMSG(User &user)
{
	std::string name = this->getCommands()[1];
	std::string message;
	for (size_t i = 2; i < this->getCommands().size(); i++)
	{
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

void Server::JOIN(User &user)
{
	(void)user;
}

void Server::PART(User &user)
{
	(void)user;
}

void Server::QUIT(User &user)
{
	User *userQuit = getUserbyFd(user.getFd());
	if (userQuit == nullptr)
		return;
	std::cout << "Client " << userQuit->getFd() << " disconnected" << std::endl;
	removeUser(userQuit->getFd());
	close(userQuit->getFd());
}