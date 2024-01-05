#include "../Server.hpp"

void Server::LIST(User &user)
{
	std::string str;
	for (size_t i = 0; i < this->channels.size(); i++)
	{
		str += this->channels[i].getChannelName() + " " + this->channels[i].getTopic() + "\n";
	}
	send(user.getFd(), str.c_str(), str.length(), 0);
}