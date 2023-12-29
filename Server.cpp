#include "Server.hpp"



Server::Server(int port, std::string ip, std::string password)
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
	// this->buffer.clear();
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
			std::cout << this->fds[1].fd << std::endl;
	}
}

void Server::userAccept()
{
	int new_socket;

	if (this->getFds()[0].revents & POLLIN)
	{	
		if ((new_socket = accept(this->serverFd, (struct sockaddr *)&(this->address), &(this->addrlen))) > 0)
		{
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
	else
	{
		std::cout << "->" << buffer << std::endl;
	}
}



void Server::parseMessage()
{
	if(strlen(buffer) <= 0)
		return;
	std::istringstream iss(buffer);
    std::string token;
    while (std::getline(iss, token, ' ')) {
        this->setCommands(token);
    }
}

void Server::setCommands(std::string command){
	this->commands.push_back(command);
}

vector<std::string> Server::getCommands() const{
	return this->commands;
}


void Server::controlMessage(int fd)
{
	User *user = getUserbyFd(fd);

	void (Server::*tools[])(user) = {&Server::PRIVMSG, &Server::JOIN, &Server::PART, &Server::QUIT, &Server::NICK, &Server::USER};

	std::string commands[] = {"NICK", "USER", "PRIVMSG", "JOIN", "PART", "QUIT"};

	for(int i = 0; i < 6; i++){
		if (!user->getIsActive() && i >= 2)
			break;
		if(this->getCommands()[0] == commands[i]){
			(this->*tools[i])(fd);
			break;
		}
	}
	if (user->getNickName().length() == 0)
		send(fd, "Please enter your nickname: ", 28, 0);
	else if (user->getRealName().length() == 0)
		send(fd, "Please enter your real name: ", 28, 0);
	else if (user->getAltNickName().length() == 0)
		send(fd, "Please enter your alternative nickname: ", 28, 0);
	else
	{
		user->setIsActive(true);
		send(fd, "You are registered", 18, 0);
	}	
}

void Server::PRIVMSG(User &user)
{
		std::string name = this->getCommands()[1];
		std::string message = this->getCommands()[2];
		if (name[0] == '#')
		{
			//
			sendChannelMessage(user.getFd(), name, message);
		}
		else
		{
			sendPrivateMessage(user.getFd(), name, message);
		}
}

void Server::JOIN(User &user)
{
	
}

void Server::PART(User &user)
{

}

void Server::QUIT(User &user)
{

}

void Server::NICK(User &user)
{

}

void Server::USER(User &user)
{

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

int Server::getPort() const
{
	return (this->port);
}

std::string Server::getIp() const
{
	return (this->ip);
}

std::string Server::getPassword() const
{
	return (this->password);
}

int Server::getServerFd() const
{
	return (this->serverFd);
}

struct pollfd * Server::getFds() const
{
	return (this->fds);
}

const struct sockaddr_in& Server::getAddress() const {
    return this->address;
}

socklen_t &Server::getAddrlen()
{
	return (this->addrlen);
}

char * Server::getBuffer() const
{
	return (this->buffer);
}

void Server::setPort(int port)
{
	this->port = port;
}

void Server::setIp(int ip)
{
	this->ip = ip;
}


void Server::setPassword(int password)
{
	this->password = password;
}

void Server::setServerFd(int server_fd)
{
	this->serverFd = server_fd;
}

void Server::setAddress(struct sockaddr_in address)
{
	this->address = address;
}

void Server::setAddrlen(socklen_t addrlen)
{
	this->addrlen = addrlen;
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

