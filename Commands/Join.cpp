#include "../Server.hpp"

void Server::JOIN(User &user)
{
    if (this->getCommands().size() < 2 || this->getUserbyFd(user.getFd()) == nullptr)
        return;
    std::string name = this->getCommands()[1];
	if (name[0] == '#')
    {
        Channel *channel = getChannelbyName(name);
        if (channel == nullptr)
        {
            this->createChannel(name);
            channel = getChannelbyName(name);
            channel->addUser(user.getFd());
            channel->setTopic("No topic");
            channel->addAdmin(user.getFd());
			std::string join = ":" + user.getNickName() + "!" + user.getUserName() + "@" + this->ip + " JOIN " + name + "\r\n";
			send(user.getFd(), join.c_str(), join.length(), 0);
		}
        else
        {
			for (size_t i = 0; i < channel->getUsers().size(); i++)
			{
				if (channel->getUsers()[i] == user.getFd())
					return;
			}
            std::cout << user.getNickName() << " joined channel " << name << std::endl;
            channel->addUser(user.getFd());
			std::string join = ":" + user.getNickName() + "!" + user.getUserName() + "@" + this->ip + " JOIN " + name + "\r\n";
			send(user.getFd(), join.c_str(), join.length(), 0);
        }
		Channel *channel2 = getChannelbyName(name);
		if (channel2 == nullptr)
			return;
		for(size_t i = 0; i < channel->getUsers().size(); i++)
		{
			User *dstUser = getUserbyFd(channel->getUsers()[i]);
			send(user.getFd(), RPL_NAMREPLY(name, dstUser->getNickName()).c_str(), RPL_NAMREPLY(name, dstUser->getNickName()).length(), 0);
		}
		send(user.getFd(), RPL_ENDOFNAMES(name).c_str(), RPL_ENDOFNAMES(name).length(), 0);
	}
    else
    {
        send(user.getFd(), "Wrong channel name\n", 19, 0);
    }
}