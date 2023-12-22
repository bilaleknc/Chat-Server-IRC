#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <vector>


class User{
	private:
		std::string userName;
		std::string nickName;
		std::string realName;
		std::string altNickName;		
		int 		fd;
		bool		isActive;
		
	public:
		User(std::string userName, std::string nickName, std::string realName, std::string altNickName, int fd);
		~User();
		void setUserName(std::string userName);
		void setNickName(std::string nickName);
		void setRealName(std::string realName);
		void setAltNickName(std::string altNickName);
		void setFd(int fd);
		void setIsActive(bool isActive);
		std::string getUserName();
		std::string getNickName();
		std::string getRealName();
		std::string getAltNickName();
		int getFd();
		bool getIsActive();
		void printUser();
};

#endif