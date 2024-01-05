#include "Channel.hpp"

Channel::Channel()
{
    this->name = "";
    this->topic = "no topic";
}

Channel::Channel(std::string name)
{
    this->name = name;
}

Channel::~Channel()
{
}

std::string Channel::getChannelName() const
{
    return this->name;
}

std::string Channel::getTopic() const
{
    return this->topic;
}

std::vector<int> Channel::getUsers() const
{
    return this->users;
}

std::vector<int> Channel::getOperators() const
{
    return this->operators;
}

void Channel::setChannelName(std::string name)
{
    this->name = name;
}

void Channel::setTopic(std::string topic)
{
    this->topic = topic;
}

void Channel::addUser(int fd)
{
    this->users.push_back(fd);
}


void Channel::addOperator(int fd)
{
    this->operators.push_back(fd);
}

void Channel::removeUser(int fd)
{
    for (size_t i = 0; i < this->users.size(); i++)
    {
        if (this->users[i] == fd)
        {
            this->users.erase(this->users.begin() + i);
            break;
        }
    }
}

void Channel::removeOperator(int fd)
{
    for (size_t i = 0; i < this->operators.size(); i++)
    {
        if (this->operators[i] == fd)
        {
            this->operators.erase(this->operators.begin() + i);
            break;
        }
    }
}

void Channel::sendMessageAllUsers(int fd, std::string message)
{
    for (size_t i = 0; i < this->users.size(); i++)
    {
        if (this->users[i] != fd)
        {
            send(this->users[i], message.c_str(), message.size(), 0);
        }
    }
}

bool Channel::onTheChannel(int fd)
{
    for (size_t i = 0; i < this->users.size(); i++)
    {
        if (this->users[i] == fd)
        {
            return true;
        }
    }
    return false;
}