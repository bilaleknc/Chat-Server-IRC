#include "Server.hpp"

#define RPL_WELCOME(nickname, username) (":irc.example.com 001 " + nickname + " :Welcome to the IRC Network " + nickname + "!" + username + "\r\n")
#define RPL_YOURHOST(nickname) (":irc.example.com 002 " + nickname + " :Your host is irc.example.com, running version 1.0\r\n")
#define RPL_CREATED(nickname) (":irc.example.com 003 " + nickname + " :This server was created sometime\r\n")
#define RPL_NOTOPIC(channel) (":irc.example.com 331 " + channel + " :No topic is set\r\n")
#define RPL_TOPIC(channel, topic) (":irc.example.com 332 " + channel + " :" + topic + "\r\n")
#define RPL_NAMREPLY(channel, nickname) (":irc.example.com 353 " + channel + " :@" + nickname + "\r\n")
#define RPL_ENDOFNAMES(channel) (":irc.example.com 366 " + channel + " :End of NAMES list\r\n")
#define RPL_JOIN(channel) (":irc.example.com 332 " + channel + " :Welcome to the channel " + channel + "\r\n")

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
			std::string welcomeMessage = RPL_WELCOME(user.getNickName(), user.getUserName());
			std::string yourHost = RPL_YOURHOST(user.getNickName());
			std::string created = RPL_CREATED(user.getNickName());
			send(user.getFd(), welcomeMessage.c_str(), welcomeMessage.length(), 0);
			send(user.getFd(), yourHost.c_str(), yourHost.length(), 0);
			send(user.getFd(), created.c_str(), created.length(), 0);
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

	std::cout<< name << std::endl;
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
    if (this->getCommands().size() < 2)
        return;
    if (this->getUserbyFd(user.getFd()) == nullptr)
        return;

    std::string name = this->getCommands()[1];
	send(user.getFd(), RPL_NAMREPLY(name.substr(0, name.length()), user.getNickName()).c_str(), RPL_NAMREPLY(name.substr(0, name.length()), user.getNickName()).length(), 0);
	send(user.getFd(), RPL_ENDOFNAMES(name.substr(0, name.length())).c_str(), RPL_ENDOFNAMES(name.substr(0, name.length())).length(), 0);
	if (name[0] == '#')
    {
        Channel *channel = getChannelbyName(name);
        if (channel == nullptr)
        {
            this->createChannel(name);
            channel = getChannelbyName(name);
            channel->addUser(user.getFd());
            channel->setTopic("No topic");
            channel->addAdmin(user.getFd());
			sendmessage_for_topic(user, RPL_NOTOPIC(name.substr(0, name.length() - 1)));

		}
        else
        {
            std::cout << user.getNickName() << " joined channel " << name << std::endl;
            channel->addUser(user.getFd());
			send(user.getFd(), RPL_JOIN(name.substr(0, name.length())).c_str(), RPL_JOIN(name.substr(0, name.length())).length(), 0);
        }
    }
    else
    {
        send(user.getFd(), "Wrong channel name\n", 19, 0);
    }
	sendmessage(user, "JOIN You are now in channel " + name.substr(0, name.length()));
	send(user.getFd(), RPL_NAMREPLY(name.substr(0, name.length() - 1), user.getNickName()).c_str(), RPL_NAMREPLY(name.substr(0, name.length()), user.getNickName()).length(), 0);
	send(user.getFd(), RPL_ENDOFNAMES(name.substr(0, name.length())).c_str(), RPL_ENDOFNAMES(name.substr(0, name.length())).length(), 0);
}

void Server::PART(User &user)
{
	if (this->getUserbyFd(user.getFd()) == nullptr)
		return;
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
		channel->setTopic(topic);
	}
	else
	{
		send(user.getFd(), "Wrong channel name\n", 19, 0);
	}
}

void Server::QUIT(User &user)
{
	if (this->getUserbyFd(user.getFd()) == nullptr)
		return;
	User *userQuit = getUserbyFd(user.getFd());
	if (userQuit == nullptr)
		return;
	std::cout << "Client " << userQuit->getFd() << " disconnected" << std::endl;
	removeUser(userQuit->getFd());
}


void Server::LIST(User &user)
{
	std::string str;
	for (size_t i = 0; i < this->channels.size(); i++)
	{
		str += this->channels[i].getChannelName() + " " + this->channels[i].getTopic() + "\n";
	}
	send(user.getFd(), str.c_str(), str.length(), 0);
}

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
