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
		bool		isLogin;
		
	public:
		User();
		User(int fd);
		~User();

		void setUserName(std::string userName);
		void setNickName(std::string nickName);
		void setRealName(std::string realName);
		void setAltNickName(std::string altNickName);
		void setFd(int fd);
		void setIsActive(bool isActive);
		void setIsLogin(bool isLogin);

		std::string getUserName() const;
		std::string getNickName() const;
		std::string getRealName() const;
		std::string getAltNickName() const;
		int getFd() const;
		bool getIsActive() const;
		bool getIsLogin() const;

		void printUser();
		void userTakeInfo(std::string info);
	};

#endif