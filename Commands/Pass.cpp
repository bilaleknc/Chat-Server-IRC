#include "../Server.hpp"

void Server::PASS(User &user)
{
	vector<std::string> com = this->getCommands();
	if (com.size() == 9 && !com[2].compare("NICK") && !com[4].compare("USER"))
	{
		if (!this->password.compare(com[1].substr(1, com[1].length() - 1)))
		{
			user.setIsLogin(true);
			user.setIsActive(true);
			user.setNickName(com[3]);
			user.setUserName(com[5]);
			user.setRealName(com[8].substr(1, com[9].length() - 1));
			std::string welcomeMessage = RPL_WELCOME(user.getNickName(), user.getUserName());
			std::string yourHost = RPL_YOURHOST(user.getNickName());
			std::string created = RPL_CREATED(user.getNickName());
			send(user.getFd(), welcomeMessage.c_str(), welcomeMessage.length(), 0);
			send(user.getFd(), yourHost.c_str(), yourHost.length(), 0);
			send(user.getFd(), created.c_str(), created.length(), 0);
			std::cout << "User " << user.getNickName() << " registered" << std::endl;
		}
	}
	else
	{
		if (this->password.compare(this->getCommands()[1]) == 0)
		{
			user.setIsLogin(true);
			send(user.getFd(), "You are logged in\n", 18, 0);
		}
		else
		{
			send(user.getFd(), "Wrong password\n", 15, 0);
		}
	}
}