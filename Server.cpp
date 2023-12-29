#include "Server.hpp"



Server::Server(std::string ip, int port, std::string password)
{
	this->port = port;
	this->ip = ip;
	this->password = password;
	address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(this->port);
	this->fds = new struct pollfd[100];
	this->buffer = new char[1024];
	this->opt = 1;
}

Server::~Server()
{
	delete[] this->fds;
	free(buffer);
}

Server::Server(const Server &other)
{
	this->port = other.port;
	this->ip = other.ip;
	this->password = other.password;
}


Server &Server::operator=(const Server &other)
{
	this->port = other.port;
	this->ip = other.ip;
	this->password = other.password;
	return (*this);
}

void Server::createServer()
{
	this->setServerFd(socket(AF_INET, SOCK_STREAM, 0));
	fcntl(this->serverFd, F_SETFL, O_NONBLOCK);	
	setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &(this->opt), sizeof(opt));

	bind(this->serverFd, reinterpret_cast<const struct sockaddr *>(&(this->address)), sizeof(struct sockaddr_in));
	listen(this->serverFd, 3);
	memset(this->getFds(), 0, 100);
	this->fds[0].fd = this->serverFd;
	this->fds[0].events = POLLIN;
}

void Server::start()
{
	std::cout << "Server started" << std::endl;
	while (1)
	{
		poll(this->fds, 10, -1);
		userAccept();
		for(int fd = 4; fd < 10; fd++)
		{
			if (this->getFds()[fd].revents & POLLIN)
			{
				readMessage(fd);
				parseMessage();
				controlMessage(fd);
				//user isRegistered

				// sendMessage(fd);
			}
		}
	}
}

void Server::userAccept()
{
	int new_socket;

	if (this->getFds()[0].revents & POLLIN)
	{
		if ((new_socket = accept(this->serverFd, (struct sockaddr *)&(this->address), &(this->addrlen))) > 0)
		{
			std::cout << "New connection" << std::endl;
			this->createUser(new_socket);
			std::cout << "Client " << new_socket << " connected" << std::endl;
			send(new_socket, "Welcome to the server\n", 23, 0);
			fds[new_socket].fd = new_socket;
			fds[new_socket].events = POLLIN;
		}
	}
}

void Server::readMessage(int fd)
{
	memset(buffer, 0, 1024);
	int valread = recv(this->getFds()[fd].fd, buffer, 1024, 0);
	if (valread == 0)
	{
		close(this->getFds()[fd].fd);
		this->getFds()[fd].fd = 0;
	}
	// else
	// {
	// 	std::cout << "->" << buffer << std::endl;
	// }
}



void Server::parseMessage()
{
	size_t index = 0;

	if(strlen(buffer) <= 0)
		return;
	std::istringstream iss(buffer);
    std::string token;
    while (std::getline(iss, token, ' ')) {
		index = token.find("\n");
		if (index != std::string::npos)
		{
			this->setCommands(token.substr(0, index - 1));
			if (token.length() - index > 1)
				this->setCommands(token.substr(index + 1, token.length() - index - 1));
		}
		else
			this->setCommands(token);	
    }
}




void Server::controlMessage(int fd)
{
	User *user = getUserbyFd(fd);

	void (Server::*tools[])(User &user) = {&Server::PASS, &Server::PRIVMSG, &Server::JOIN, &Server::PART, &Server::QUIT, &Server::NICK, &Server::USER};

	std::string commands[] = {"PASS", "NICK", "USER", "PRIVMSG", "JOIN", "PART", "QUIT"};

	for(int i = 0; i < 6; i++){
		if ((!user->getIsLogin() || !user->getIsActive()) && i > 2)
			break;
		if(this->getCommands()[0].compare(commands[i]) == 0)
			(this->*tools[i])(*user);
	}
	if(!user->getIsLogin())
		send(user->getFd(), "Please login  :  ", 12, 0);
	else if (user->getNickName().length() == 0)
		send(user->getFd(), "Please enter your nickname  :  ", 28, 0);
	else if (user->getRealName().length() == 0)
		send(user->getFd(), "Please enter your real name  :  ", 28, 0);
	else
	{
		user->setIsActive(true);
		send(fd, "You are registered", 18, 0);
	}	
}

void Server::PASS(User &user)
{
	vector<std::string> com = this->getCommands();
	if (com.size() == 9 && !com[2].compare("NICK") && !com[4].compare("USER"))
	{
		if(!this->password.compare(com[1].substr(1, com[1].length() - 1)))
		{
			user.setIsLogin(true);
			user.setNickName(com[3]);
			user.setUserName(com[5]);
			user.setRealName(com[8].substr(1, com[9].length() - 1));
			user.setIsActive(true);
			
			std::cout << "User " << user.getNickName() << " registered" << std::endl;
		}
	}
	else
	{
		if (this->password.compare(this->getCommands()[1].substr(0, this->getCommands()[1].length() - 1)) == 0)
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

void Server::PRIVMSG(User &user)
{
	std::string name = this->getCommands()[1];
	std::string message = this->getCommands()[2];
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
	(void)user;
}

void Server::NICK(User &user)
{
	(void)user;
}

void Server::USER(User &user)
{
	(void)user;
}


void Server::createUser(int fd)
{
	User user(fd);
	this->users.push_back(user);
}

void Server::createChannel(std::string channelName)
{
	Channel channel(channelName);
	this->channels.push_back(channel);
	std::cout << channelName << " created successfully" << std::endl;
}

void Server::removeUser(int fd)
{
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (it->getFd() == fd)
		{
			users.erase(it);
			std::cout << it->getUserName() << " deleted" << std::endl;
			break;
		}
	}
}

void Server::removeChannel(std::string channelName)
{
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (it->getChannelName() == channelName)
		{
			channels.erase(it);
			std::cout << channelName << " deleted" << std::endl;
			break;
		}
	}
}



void Server::sendPrivateMessage(int fd, std::string nickName, std::string message)
{
	User *user = getUserbyFd(fd);
	for(vector<User>::iterator it = users.begin(); it != users.end(); ++it){
		if(it->getNickName() == nickName){
			message = "Client " + user->getUserName() + " : " + message;
			send(it->getFd(), message.c_str(), message.length(), 0);
			break;
		}
	}
}

void Server::sendChannelMessage(int fd, std::string channelName, std::string message)
{
	Channel *channel = getChannelbyName(channelName);
	if(channel != nullptr){
		message = "Client " + getUserbyFd(fd)->getUserName() + " : " + message;
		channel->sendMessageAllUsers(fd, message);
	}
}

void Server::sendServerMessage(int fd, std::string message){
	User *user = getUserbyFd(fd);
	if (user != nullptr){
		message = "Client " + user->getUserName() + " : " + message;
		send(this->getServerFd(), message.c_str(), message.length(), 0);
	}
}

Channel * Server::getChannelbyName(std::string name)
{
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (it->getChannelName() == name)
		{
			return (&(*it));
		}
	}
	return nullptr;	
}

User * Server::getUserbyNickName(std::string name)
{
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (it->getNickName() == name)
		{
			return (&(*it));
		}
	}
	return nullptr;	
} 

User * Server::getUserbyFd(int fd)
{
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (it->getFd() == fd)
		{
			return (&(*it));
		}
	}
	return nullptr;	
}

