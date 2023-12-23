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
	delete[] this->buffer;
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
	fcntl(this->getServerFd(), F_SETFL, O_NONBLOCK);	
	setsockopt(this->getServerFd(), SOL_SOCKET, SO_REUSEADDR, &(this->opt), sizeof(opt));

	bind(this->getServerFd(), reinterpret_cast<const struct sockaddr *>(&(this->getAddress())), sizeof(struct sockaddr_in));
	listen(this->getServerFd(), 3);
	memset(this->getFds(), 0, 100);
	this->fds[0].fd = this->getServerFd();
	this->fds[0].events = POLLIN;
}

void Server::start()
{
	std::cout << "Server started" << std::endl;
	while (1)
	{
		poll(this->getFds(), 10, -1);
		userAccept();
		for(int fd = 4; fd < 10; fd++)
		{
			std::cout << "fd: " << fd << std::endl;
			readMessage(fd);
			std::cout << "buffer: " << this->getBuffer() << std::endl;
			parseMessage();
			//controlMessage(fd);
			//user isRegistered

			// sendMessage(fd);
		}
	}
}

void Server::userAccept()
{
	int new_socket;

	if (this->getFds()[0].revents & POLLIN)
	{ 
		if ((new_socket = accept(this->getServerFd(), (struct sockaddr *)&(this->getAddress()), &(this->getAddrlen()))) > 0)
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
	memset(this->getBuffer(), 0, 1024);
	if (this->getFds()[fd].revents & POLLIN)
	{
		int valread = read(this->getFds()[fd].fd, this->getBuffer(), 1024);
		std::cout << "valread: " << valread << std::endl;
		if (valread == 0)
		{
			close(this->getFds()[fd].fd);
			this->getFds()[fd].fd = 0;
		}
	}
}

void Server::parseMessage()
{
	if(this->getBuffer().length() == 0)
		return;
	this->getBuffer()[strlen(this->getBuffer()) - 1] = '\0';
	delete [] this->getCommand();
	this->setCommand(buffer.split(" "));
	

	
}

void Server::setCommand(char **command){
	this->command = command;
}

char **Server::getCommand() const{
	return this->command;
}


		// 	if(fds[i].revents & POLLIN && this->getUserbyFd(fds[i].fd)->getIsActive() == false)
		// 	{
		// 		if(this->getUserbyFd(fds[i].fd)->getNickName().length() == 0)
		// 		{
		// 			send(fds[i].fd, "Please enter your nickname: ", 28, 0);
		// 		}
		// 		send(fds[i].fd, "Please enter your nickname: ", 28, 0);
		// 	}
		// }



// void Server::controlMessage(int fd)
// {
// 	User *user = getUserbyFd(fd);
// 	if(user != nullptr){
// 		if(user->getIsRegistered() == false){
// 			if(user->getNickName().length() == 0){
// 				send(fd, "Please enter your nickname: ", 28, 0);
// 			}
// 			else if(user->getRealName().length() == 0){
// 				send(fd, "Please enter your real name: ", 28, 0);
// 			}
// 			else if(user->getAltNickName().length() == 0){
// 				send(fd, "Please enter your alternative nickname: ", 28, 0);
// 			}
// 			else{
// 				user->setIsRegistered(true);
// 				send(fd, "You are registered", 18, 0);
// 			}
// 		}
// 	}
// }





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

char *Server::getBuffer() const
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







	



















	// Creating socket file descriptorid 
	
		// fcntl: Dosya açıklaması için dosya durum bayrağını ayarlar
	// F_SETFL: Dosya durum bayrağını ayarlamak için kullanılan bir işleve işaret ediyor
	// O_NONBLOCK: Bloklama olmadan açar

	// SO_REUSEADDR: Bu seçenek, aynı adresi
	// ve portu bir süre boyunca tekrar kullanma izni verir.
	// &opt : SO_REUSEADDR seçeneğini etkinleştirmek için kullanılan değer(genellikle 1).
		// IPv4 adreslerinin bir sunucuda dinlenmesi için kullanılan bir işleve işaret ediyor
	// inet_pton(AF_INET, "10.10.10.11", &address.sin_addr);
	// address.sin_addr.s_addr = inet_addr("10.10.10.11");
	



	// poll(this->getFds(), 100, -1);
	// 	for (int i = 0; i < 100; i++)
	// 	{
	// 		if (this->getFds()[i].revents & POLLIN)
	// 		{
	// 			if (this->getFds()[i].fd == this->getServerFd())
	// 			{
	// 				this->getFds()[i].fd = accept(this->getServerFd(), (struct sockaddr *)&(this->getAddress()), &(this->getAddrlen()));
	// 				this->getFds()[i].events = POLLIN;
	// 			}
	// 			else
	// 			{
	// 				memset(this->getBuffer(), 0, 1024);
	// 				int valread = read(this->getFds()[i].fd, this->getBuffer(), 1024);
	// 				if (valread == 0)
	// 				{
	// 					close(this->getFds()[i].fd);
	// 					this->getFds()[i].fd = 0;
	// 				}
	// 				else
	// 				{
	// 					std::cout << this->getBuffer() << std::endl;
	// 					std::string message(this->getBuffer());
	// 					std::string command = message.substr(0, message.find(" "));
	// 					std::string userName = message.substr(message.find(" ") + 1, message.find(" ", message.find(" ") + 1) - message.find(" ") - 1);
	// 					std::string nickName = message.substr(message.find(" ", message.find(" ") + 1) + 1, message.find(" ", message.find(" ", message.find(" ") + 1) + 1) - message.find(" ", message.find(" ") + 1) - 1);
	// 					std::string realName = message.substr(message.find(" ", message.find(" ", message.find(" ") + 1) + 1) + 1, message.find(" ", message.find(" ", message.find(" ", message.find(" ") + 1) + 1) + 1) - message.find(" ", message.find(" ", message.find(" ") + 1) + 1) - 1);
	// 					std::string altNickName = message.substr(message.find(" ", message.find(" ", message.find(" ", message.find(" ") + 1) + 1) + 1) + 1, message.find("
	// 					", message.find(" ", message.find(" ", message.find(" ", message.find(" ") + 1) + 1) + 1) + 1) - message.find(" ", message.find(" ", message.find(" ", message.find(" ") + 1) + 1) + 1) - 1);
	// 					if (command == "USER")
	// 					{
	// 						this->createUser(userName, nickName, realName, altNickName, this->getFds()[i].fd);
	// 					}
	// 					else if (command == "NICK")
	// 					{
	// 						this->addUser(userName, nickName, realName, altNickName, this->getFds()[i].fd);
	// 					}
	// 					else if (command == "JOIN")
	// 					{
	// 						this->createChannel(userName);
	// 					}
	// 					else if (command == "PART")
	// 					{
	// 						this->removeChannel(userName);
	// 					}
	// 					else if (command == "PRIVMSG")
	// 					{
	// 						this->sendPrivateMessage(this->getFds()[i].fd, userName, message);
	// 					}
	// 					else if (command == "QUIT")
	// 					{
	// 						this->removeUser(this->getFds()[i].fd);
	// 					}
	// 				}
	// 			}
	// 		}
	// 	}




	// for (int j = 1; j < 100; j++)
	// 			{
	// 				if (fds[j].fd != 0 && fds[j].fd != fds[i].fd)
	// 				{
	// 					User *user = getUserbyFd(fds[i].fd);
	// 					send(fds[j].fd, user->getNickName().c_str(), user->getNickName().length(), 0);
	// 					send(fds[j].fd, std::to_string(fds[i].fd).c_str(), std::to_string(fds[i].fd).length(), 0);
	// 					send(fds[j].fd, "  : ", 4, 0);
	// 					send(fds[j].fd, buffer, valread, 0);
	// 				}
	// 			}
	// 			std::cout << buffer << std::endl;