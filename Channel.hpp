#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <vector>
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


using namespace std;

class Channel {
	private:
		std::string name;
		std::string topic;
		vector<int> users;
		vector<int> admins;
		vector<int> operators;

	public:
		Channel();
		Channel(std::string name);
		~Channel();

		void print_users();

		void addUser(int fd);
		void addAdmin(int fd);
		void addOperator(int fd);

		void removeUser(int fd);
		void removeAdmin(int fd);
		void removeOperator(int fd);

		void sendMessageAllUsers(int fd, std::string message);
		
		std::string getChannelName() const;
		std::string getTopic() const;
		std::vector<int> getUsers() const;
		std::vector<int> getAdmins() const;
		std::vector<int> getOperators() const;

		void setChannelName(std::string name);
		void setTopic(std::string topic);

		User findUserbyFd(int fd);
};

#endif