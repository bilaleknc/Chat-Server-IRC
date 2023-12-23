#include <iostream>
#include <vector>
#include "Channel.hpp"
#include "User.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <string.h>	


class Server
{
	private:
		struct sockaddr_in address;
		vector<Channel> channels;
		std::string password;
		struct pollfd *fds;
		vector<User> users;
		socklen_t addrlen;
		std::string ip;
		char **command;
		int serverFd;
		char *buffer;
		int port;
		int opt;
	public:
		Server(int port, std::string ip, std::string password);
		~Server();
		Server(const Server &other);
		Server &operator=(const Server &other);

		void start();
		void createServer();

		void createUser(int fd);
		void createChannel(std::string name);
		
		void removeUser(int fd);
		void removeChannel(std::string name);

		int getPort() const;
		std::string getIp() const;
		std::string getPassword() const;
		int getServerFd() const;
		struct pollfd *getFds() const;
		const struct sockaddr_in &getAddress() const;
		socklen_t &getAddrlen();
		char *getBuffer() const;
		char **getCommand() const;

		void setPort(int port);
		void setIp(int ip);
		void setPassword(int password);
		void setServerFd(int server_fd);
		void setAddrlen(socklen_t addrlen);
		void setAddress(struct sockaddr_in address);
		void setCommand(char **command);

		void sendPrivateMessage(int fd, std::string nickName, std::string message);
		void sendChannelMessage(int fd, std::string channel, std::string message);
		void sendServerMessage(int fd, std::string message);
		
		Channel * getChannelbyName(std::string name);
		User * getUserbyNickName(std::string name);
		User * getUserbyFd(int fd);

		void userAccept();
		void readMessage(int fd);
		void parseMessage();
		void controlMessage(int fd);
		void sendMessage(int fd, std::string message);

		//channel isminden başka var mı kontrolü
		//user nickinden başka var mı kontrolü	
};