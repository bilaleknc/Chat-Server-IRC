#include "Server.hpp"

// void sendmessage2(User &sender, int socketfd, std::string message)
// {
// 	std::string a = ":" + sender.getNickName() + "!" + sender.getUserName() + "@" + sender.hostname + ":";
// 	std::string buffer = a + " " + message + "\r\n";
// 	send(socketfd, buffer.c_str(), buffer.size(), 0);
// }

// void sendmessage_privmsg(User &ite, User *clients, std::string message)
// {
// 	std::string a = ":" + ite.getNickName() + "!" + ite.getUserName() + "@" + ite.hostname + ":";
// 	std::string buffer = a + " " + message + "\r\n";
// 	send(clients->getFd(), buffer.c_str(), buffer.size(), 0);
// }

// void sendmessage(User &ite, std::string message)
// {
// 	//std::string a = ":" + ite.get_nickname() + "!" + ite.get_username() + "@" + ite.hostname + ":";
// 	std::string buffer = a + " " + message + "\r\n";

// 	send(ite.socket_fd, buffer.c_str(), buffer.size(), 0);
// }

// void sendmessage_join(User *ite, std::string message)
// {
// 	std::string a = ":" + ite->getNickName() + "!" + ite->getUserName() + "@" + ite-> + ":";
// 	std::string buffer = a + " " + message + "\r\n";
// 	send(ite->getFd(), buffer.c_str(), buffer.size(), 0);
// }

// void sendmessage_for_topic(User &ite, std::string message)
// {
// 	std::string a = ":" + ite.get_nickname() + "!" + ite.get_username() + "@" + ite.hostname + ":";
// 	std::string buffer = a + " " + message + "\r\n";
// 	;
// 	send(ite.socket_fd, buffer.c_str(), buffer.size(), 0);
// }
// void sendmessage_for_part(User *ite, std::string message)
// {
// 	std::string a = ":" + ite->get_nickname() + "!" + ite->get_username() + "@" + ite->hostname + ":";
// 	std::string buffer = a + " " + message + "\r\n";
// 	;
// 	send(ite->socket_fd, buffer.c_str(), buffer.size(), 0);
// }