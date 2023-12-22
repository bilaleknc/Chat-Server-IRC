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
		vector<Channel> channels;
		vector<User> users;
		int port;
		std::string ip;
		int serverFd;
		std::string password;
		socklen_t addrlen;
		struct sockaddr_in address;
		struct pollfd fds[100];
		char buffer[1024];
		int opt;
		// int new_socket;
		// int valread;
		// struct sockaddr_in address;
		// char buffer[1024];

	public:
		Server(int port, std::string ip, std::string password);
		~Server();
		Server(const Server &other);
		Server &operator=(const Server &other);

		void start();
		void stop();
		void createServer();

		void createUser(std::string userName, std::string nickName, std::string realName, std::string altNickName, int fd);
		void createChannel(std::string name);
		
		void removeUser(int fd);
		void removeChannel(std::string name);

		int getPort() const;
		std::string getIp() const;
		std::string getPassword() const;
		int getServerFd() const;
		struct pollfd getFds() const;
		struct sockaddr_in getAddress() const;
		socklen_t getAddrlen() const;

		void setPort(int port);
		void setIp(int ip);
		void setPassword(int password);
		void setServerFd(int server_fd);
		void setAddrlen(socklen_t addrlen);
		void setAddress(struct sockaddr_in address);

		void sendPrivateMessage(int fd, std::string nickName, std::string message);
		void sendChannelMessage(int fd, std::string channel, std::string message);
		void sendServerMessage(int fd, std::string message);
		
		Channel getChannelbyName(std::string name);
		User getUserbyName(std::string name);
		Channel getChannelbyFd(int fd);
		User getUserbyFd(int fd);		
};