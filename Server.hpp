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

#define RPL_WELCOME(nickname, username) (":irc.example.com 001 " + nickname + " :Welcome to the IRC Network " + nickname + "!" + username + "\r\n")
#define RPL_YOURHOST(nickname) (":irc.example.com 002 " + nickname + " :Your host is irc.example.com, running version 1.0\r\n")
#define RPL_CREATED(nickname) (":irc.example.com 003 " + nickname + " :This server was created sometime\r\n")
#define RPL_NOTOPIC(channel) (":irc.example.com 331 " + channel + " :No topic is set\r\n")
#define RPL_TOPIC(channel, topic) (":irc.example.com 332 " + channel + " :" + topic + "\r\n")
#define RPL_NAMREPLY(channel, nickname) (":irc.example.com 353 " + channel + " :@" + nickname + "\r\n")
#define RPL_ENDOFNAMES(channel) (":irc.example.com 366 " + channel + " :End of NAMES list\r\n")
#define RPL_JOIN(channel) (":irc.example.com 332 " + channel + " :Welcome to the channel " + channel + "\r\n")
#define RPL_WHOREPLY(channel, nickname, username, hostname, servername, nickname2, hopcount, realname) (":irc.example.com 352 " + channel + " " + nickname + " " + username + " " + hostname + " " + servername + " " + nickname2 + " " + hopcount + " " + realname + " H :0 " + realname + "\r\n")
#define RPL_ENDOFWHO(channel) (":irc.example.com 315 " + channel + " :End of WHO list\r\n")

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
		void PONG(User &user);
		void LIST(User &user);
		void PASS(User &user);
		void TOPIC(User &user);
		void NOTICE(User &user);
		void PING(User &user);
		void WHO(User &user);
		void MODE(User &user);
		void KICK(User &user);

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
};