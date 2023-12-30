#include "Server.hpp"

#define RPL_WELCOME(nickname, username) (":irc.example.com 001 " + nickname + " :Welcome to the IRC Network " + nickname + "!" + username + "\r\n")

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
			RPL_WELCOME(user.getNickName(), user.getUserName());

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

void Server::JOIN(User &user)
{
	std::string name = this->getCommands()[1];
	if (name[0] == '#')
	{
		Channel *channel = getChannelbyName(name);
		if (channel == nullptr)
		{
			send(user.getFd(), "Channel created\n", 16, 0);
			this->createChannel(name);
			channel = getChannelbyName(name);
			channel->addUser(user.getFd());
			channel->setTopic("No topic");
			channel->addAdmin(user.getFd());
		}
		else
		{
			send(user.getFd(), "Channel joined\n", 15, 0);
			std::cout << user.getNickName() << " joined channel " << name << std::endl;
			channel->addUser(user.getFd());
		}
	}
	else
	{
		send(user.getFd(), "Wrong channel name\n", 19, 0);
	}
}

void Server::PART(User &user)
{
	std::string name = this->getCommands()[1];
	if (name[0] == '#')
	{
		Channel *channel = getChannelbyName(name);
		if (channel == nullptr)
		{
			send(user.getFd(), "Channel not found\n", 18, 0);
			return;
		}
		channel->removeUser(user.getFd());
	}
	else
	{
		send(user.getFd(), "Wrong channel name\n", 19, 0);
	}
}

void	Server::TOPIC(User &user)
{
	std::string name = this->getCommands()[1];
	std::string topic;
	if (this->getCommands().size() < 3)
	{
		std::cout << "Wrong command" << std::endl;
		return;
	}
	if (this->getCommands()[2][0] != ':')
	{
		std::cout << "Wrong command" << std::endl;
		return;
	}
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
		channel->setTopic(topic);
	}
	else
	{
		send(user.getFd(), "Wrong channel name\n", 19, 0);
	}
}



void Server::QUIT(User &user)
{
	User *userQuit = getUserbyFd(user.getFd());
	if (userQuit == nullptr)
		return;
	std::cout << "Client " << userQuit->getFd() << " disconnected" << std::endl;
	removeUser(userQuit->getFd());
}

void Server::CAP(User &user)
{
	std::cout << "CAP" << std::endl;
	std::string message = user.getNickName();
	message += " CAP * LS :multi-refix sasl\r\n";

	std::cout << message << std::endl;
	send(user.getFd(), message.c_str(), message.length(), 0);

	// if (this->getCommands().size() < 2)
	// {
	// 	std::cout << "Wrong command" << std::endl;
	// 	return;
	// }
	// if (this->getCommands()[1] == "LS")
	// {
	// 	send(user.getFd(), "CAP LS\n", 7, 0);
	// 	std::cout << "CAP LS" << std::endl;
	// }
	// else if (this->getCommands()[1] == "REQ")
	// {
	// 	send(user.getFd(), "CAP REQ\n", 8, 0);
	// 	std::cout << "CAP REQ" << std::endl;
	// }
	// else if (this->getCommands()[1] == "END")
	// {
	// 	send(user.getFd(), "CAP END\n", 8, 0);
	// 	std::cout << "CAP END" << std::endl;
	// }
	// else
	// {
	// 	send(user.getFd(), "Wrong command\n", 14, 0);
	// 	std::cout << "Wrong command" << std::endl; 
	// }
}