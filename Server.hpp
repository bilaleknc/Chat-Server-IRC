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
#include <poll.h>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string> 
#include <iostream>
#include <vector>
#include <cstring>
#include <sstream>

// void sendmessage2(User &sender, int socketfd, std::string message);
// void sendmessage_privmsg(User &ite, User *clients, std::string message);
// void sendmessage(User &ite, std::string message);
// void sendmessage_for_topic(User &ite, std::string message);
// void sendmessage_for_part(User *ite, std::string message);


	class Server
{
	private:
		struct sockaddr_in address;
		vector<Channel> channels;
		vector<std::string> commands;
		std::string password;
		vector<User> users;
		struct pollfd *fds;
		socklen_t addrlen;
		std::string ip;
		int serverFd;
		char *buffer;
		int port;
		int opt;
	public:
		Server(std::string ip, int port, std::string password);
		~Server();
		Server(const Server &other);
		Server &operator=(const Server &other);

		void start();
		void createServer();

		void createUser(int fd);
		void createChannel(std::string name);
		
		void removeUser(int fd);
		void removeChannel(std::string name);

		void PRIVMSG(User &user);
		void JOIN(User &user);
		void PART(User &user);
		void QUIT(User &user);
		void NICK(User &user);
		void USER(User &user);
		void PING(User &user);
		void PONG(User &user);
		void LIST(User &user);
		void PASS(User &user);
		void TOPIC(User &user);
		void NOTICE(User &user);


		int getPort() const;
		std::string getIp() const;
		std::string getPassword() const;
		int getServerFd() const;
		struct pollfd *getFds() const;
		const struct sockaddr_in &getAddress() const;
		socklen_t &getAddrlen();
		char *getBuffer() const;
		vector<std::string> getCommands() const;

		void setPort(int port);
		void setIp(int ip);
		void setPassword(int password);
		void setServerFd(int server_fd);
		void setAddrlen(socklen_t addrlen);
		void setAddress(struct sockaddr_in address);
		void setCommands(std::string command);

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

		void sendmessage_join(User *ite, std::string message)
		{
			std::string a = ":" + ite->getNickName() + "!" + ite->getUserName() + "@" + this->ip + ":";
			std::string buffer = a + " " + message + "\r\n";
			send(ite->getFd(), buffer.c_str(), buffer.size(), 0);
		}

		void sendmessage_for_topic(User &ite, std::string message)
		{
			std::string a = ":" + ite.getNickName() + "!" + ite.getUserName() + "@" + this->ip + ":";
			std::string buffer = a + " " + message + "\r\n";
			send(ite.getFd(), buffer.c_str(), buffer.size(), 0);
		}

		void sendmessage(User &ite, std::string message)
		{
			std::string a = ":" + ite.getNickName() + "!" + ite.getUserName() + "@" + this->ip + ":";
			std::string buffer = a + " " + message + "\r\n";

			send(ite.getFd()  , buffer.c_str(), buffer.size(), 0);
		}
};