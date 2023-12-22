#include "Server.hpp"



Server::Server(int port, std::string ip, std::string password)
{
	this->port = port;
	this->ip = ip;
	this->password = password;
	this->opt = 1;
}

Server::~Server()
{
	std::cout << "Server destructor called" << std::endl;
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

void Server::start()
{
	std::cout << "Server started" << std::endl;
}

void Server::stop()
{
	std::cout << "Server stopped" << std::endl;
}

void Server::createServer()
{
	std::cout << "Server created" << std::endl;

	this->setServerFd(socket(AF_INET, SOCK_STREAM, 0));
	fcntl(this->getServerFd(), F_SETFL, O_NONBLOCK);	
	setsockopt(this->getServerFd(), SOL_SOCKET, SO_REUSEADDR, &(this->opt), sizeof(opt));

	bind(this->getServerFd(), (struct sockaddr *)&(this->getAddress()), sizeof(address));
	listen(this->getServerFd(), 3);
	struct pollfd fds[100];
	memset(fds, 0, sizeof(fds));
	this->fds[0].fd = this->getServerFd();
	this->fds[0].events = POLLIN;
}

void Server::start()
{
	while (1)
	{
		poll(this->getFds(), 100, -1);
		for (int i = 0; i < 100; i++)
		{
			// if (this->getFds()[i].revents & POLLIN)
			// {
			// 	if (this->getFds()[i].fd == this->getServer_fd())
			// 	{
			// 		this->getFds()[i].fd = accept(this->getServer_fd(), (struct sockaddr *)&(this->getAddress()), &(this->getAddrlen()));
			// 		this->getFds()[i].events = POLLIN;
			// 	}
			// 	else
			// 	{
			// 		memset(this->getBuffer(), 0, 1024);
			// 		int valread = read(this->getFds()[i].fd, this->getBuffer(), 1024);
			// 		if (valread == 0)
			// 		{
			// 			close(this->getFds()[i].fd);
			// 			this->getFds()[i].fd = 0;
			// 		}
			// 		else
			// 		{
			// 			std::cout << this->getBuffer() << std::endl;
			// 			std::string message = this->getBuffer();
			// 			std::string command = message.substr(0, message.find(" "));
			// 			std::string userName = message.substr(message.find(" ") + 1, message.find(" ", message.find(" ") + 1) - message.find(" ") - 1);
			// 			std::string nickName = message.substr(message.find(" ", message.find(" ") + 1) + 1, message.find(" ", message.find(" ", message.find(" ") + 1) + 1) - message.find(" ", message.find(" ") + 1) - 1);
			// 			std::string realName = message.substr(message.find(" ", message.find(" ", message.find(" ") + 1) + 1) + 1, message.find(" ", message.find(" ", message.find(" ", message.find(" ") + 1) + 1) + 1) - message.find(" ", message.find(" ", message.find(" ") + 1) + 1) - 1);
			// 			std::string altNickName = message.substr(message.find(" ", message.find(" ", message.find(" ", message.find(" ") + 1) + 1) + 1) + 1, message.find("
			// 			", message.find(" ", message.find(" ", message.find(" ", message.find(" ") + 1) + 1) + 1) + 1) - message.find(" ", message.find(" ", message.find(" ", message.find(" ") + 1) + 1) + 1) - 1);
			// 			if (command == "USER")
			// 			{
			// 				this->createUser(userName, nickName, realName, altNickName, this->getFds()[i].fd);
			// 			}
			// 			else if (command == "NICK")
			// 			{
			// 				this->addUser(userName, nickName, realName, altNickName, this->getFds()[i].fd);
			// 			}
			// 			else if (command == "JOIN")
			// 			{
			// 				this->createChannel(userName);
			// 			}
			// 			else if (command == "PART")
			// 			{
			// 				this->removeChannel(userName);
			// 			}
			// 			else if (command == "PRIVMSG")
			// 			{
			// 				this->sendPrivateMessage(this->getFds()[i].fd, userName, message);
			// 			}
			// 			else if (command == "QUIT")
			// 			{
			// 				this->removeUser(this->getFds()[i].fd);
			// 			}
			// 		}
			// 	}
			// }
		}
	}
}


		// poll(server->getFds(), 100, -1);

		// if (server->getFds()[0].revents & POLLIN)
		// { // yeni bir bağlantı olduğunda giriyor revents yeni bir bağlantı olmazsa 0 oluyor
		// 	if ((new_socket = accept(server->getFds()[0].revents, (struct sockaddr *)&(server->getAddress()), &(server->getAddrlen))) > 0)
		// 	{
		// 		std::cout << "Client " << new_socket << " connected" << std::endl;
		// 		send(new_socket, buffer, valread, 0);
		// 		fds[new_socket].fd = new_socket;
		// 		fds[new_socket].events = POLLIN;
		// 	}
		// }
		// for (int i = 1; i < 100; i++)
		// {
		// 	if (fds[i].revents & POLLIN)
		// 	{ // bağlantıdan veri geldiğinde giriyor
		// 		std::cout << "Client " << new_socket << " :  ";
		// 		valread = read(fds[i].fd, buffer, 1024 - 1);
		// 		if (strcmp(buffer, "exit\n") == 0)
		// 		{
		// 			std::cout << "Client " << new_socket << " disconnected" << std::endl;
		// 			close(fds[i].fd);
		// 			fds[i].fd = 0;
		// 			fds[i].events = 0;
		// 			memset(buffer, 0, 1024);
		// 			continue;
		// 		}
		// 		for (int j = 1; j < 100; j++)
		// 		{
		// 			if (fds[j].fd != 0 && fds[j].fd != fds[i].fd)
		// 			{
		// 				send(fds[j].fd, "Client ", 7, 0);
		// 				send(fds[j].fd, std::to_string(fds[j].fd).c_str(), std::to_string(fds[j].fd).length(), 0);
		// 				send(fds[j].fd, " : ", 3, 0);
		// 				send(fds[j].fd, buffer, valread, 0);
		// 			}
		// 		}
		// 		printf("%s", buffer);
		// 		memset(buffer, 0, 1024);
		// 	}
		// }


void Server::createUser(std::string userName, std::string nickName, std::string realName, std::string altNickName, int fd)
{
	User user(userName, nickName, realName, altNickName, fd);
	this->users.push_back(user);
	std::cout << userName << "created successfully" << std::endl;
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

struct sockaddr_in Server::getAddress() const
{
	return (this->address);
}

socklen_t Server::getAddrlen() const
{
	return (this->addrlen);
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
    for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
    {
        if (it->getNickName() == nickName)
        {
            message = "Client " + getUserbyFd(fd).getUserName() + " : " + message;
            send(fd, message.c_str(), message.length(), 0);
            break;
        }
    }
}

void Server::sendChannelMessage(int fd, std::string channelName, std::string message)
{
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (it->getChannelName() == channelName)
		{
			message = "Client " + getUserbyFd(fd).getUserName() + " : " + message;
			it->sendMessage(fd, message);
			break;
		}
	}
}

void Server::sendServerMessage(int fd, std::string message){
	message = "Client " + getUserbyFd(fd).getUserName() + " : " + message;
	send(this->getServerFd(), message.c_str(), message.length(), 0);
}

Channel Server::getChannelbyName(std::string name)
{
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (it->getChannelName() == name)
		{
			return (*it);
		}
	}
}

User Server::getUserbyName(std::string name)
{
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (it->getUserName() == name)
		{
			return (*it);
		}
	}
}

Channel Server::getChannelbyFd(int fd)
{
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (it->getFd() == fd)
		{
			return (*it);
		}
	}
}


User Server::getUserbyFd(int fd)
{
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (it->getFd() == fd)
		{
			return (*it);
		}
	}
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
	