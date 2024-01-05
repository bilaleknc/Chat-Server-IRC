#include "../Server.hpp"

void Server::USER(User &user)
{
	if (this->getCommands().size() == 5)
	{
		user.setUserName(this->getCommands()[1]);
		user.setRealName(this->getCommands()[4]);
		user.setIsActive(true);
	}
}