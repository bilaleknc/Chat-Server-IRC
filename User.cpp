#include "User.hpp"


User::User()
{
	this->userName = "";
	this->nickName = "";
	this->realName = "";
	this->altNickName = "";
	this->fd = 0;
	this->isActive = false;
	this->isLogin = false;
}

User::User(int fd)
{
    this->userName = "";
	this->nickName = "";
	this->realName = "";
	this->altNickName = "";
    this->fd = fd;
    this->isActive = false;
	this->isLogin = false;
}

User::~User()
{
	
}

bool User::getIsLogin() const
{
	return this->isLogin;
}

void User::setIsLogin(bool isLogin)
{
	this->isLogin = isLogin;
}

std::string User::getUserName() const
{
    return this->userName;
}

std::string User::getNickName() const
{
    return this->nickName;
}

std::string User::getRealName() const
{
    return this->realName;
}

std::string User::getAltNickName() const
{
    return this->altNickName;
}

int User::getFd() const
{
    return this->fd;
}

bool User::getIsActive() const
{
    return this->isActive;
}

void User::setUserName(std::string userName)
{
    this->userName = userName;
}

void User::setNickName(std::string nickName)
{
    this->nickName = nickName;
}

void User::setRealName(std::string realName)
{
    this->realName = realName;
}

void User::setAltNickName(std::string altNickName)
{
    this->altNickName = altNickName;
}

void User::setFd(int fd)
{
    this->fd = fd;
}

void User::setIsActive(bool isActive)
{
    this->isActive = isActive;
}

void User::printUser()
{
    std::cout << "User: " << this->userName << std::endl;
    std::cout << "Nick: " << this->nickName << std::endl;
    std::cout << "Real: " << this->realName << std::endl;
    std::cout << "Alt: " << this->altNickName << std::endl;
    std::cout << "Fd: " << this->fd << std::endl;
    std::cout << "Active: " << this->isActive << std::endl;
}
