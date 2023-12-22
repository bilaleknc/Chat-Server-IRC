//  kanal classı ve içerisinde bulunan fonksiyonlar

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <vector>
#include "User.hpp"

using namespace std;

class Channel {
	private:
		std::string name;
		std::string topic;
		vector<int> users;
		vector<int> admins;
		vector<int> operators;

	public:
		Channel(std::string name);
		~Channel();
		void start();
		void accept_connection();
		void read_data(int fd);
		void send_data(int fd);
		void close_connection(int fd);
		void print_users();
		std::string get_name();	
		void set_name(std::string name);
		void add_user(User user);
		void remove_user(int fd);
		void setAdmin(int fd);
		void setOperator(int fd);
		void removeAdmin(int fd);
		void removeOperator(int fd);
		void sendMessage(int fd, std::string message);
		
		std::string getChannelName();
		int getFd();
		std::vector<int> getUsers();
		std::vector<int> getAdmins();
		std::vector<int> getOperators();
		std::string getTopic();
		void setTopic(std::string topic);
		void setFd(int fd);
		void setUsers(std::vector<int> users);
		void setAdmins(std::vector<int> admins);
		void setOperators(std::vector<int> operators);
};

#endif
