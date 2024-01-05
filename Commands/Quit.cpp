#include "../Server.hpp"

void Server::QUIT(User &user)
{
	if (this->getUserbyFd(user.getFd()) == nullptr)
		return;
	User *userQuit = getUserbyFd(user.getFd());
	if (userQuit == nullptr)
		return;
	std::cout << "Client " << userQuit->getFd() << " disconnected" << std::endl;
	removeUser(userQuit->getFd());
}