#include "Admin.h"
#include <iostream>

Admin::Admin(UserId id, const std::string &username,
             const std::string &password, const std::string &fullName)
    : User(id, username, password, fullName) {}

std::string Admin::getRole() const
{
    return "Admin";
}

std::string Admin::toFileString() const
{
    // Format: Admin|id|username|password|fullName
    return "Admin|" + std::to_string(_id) + "|" + _username + "|" + _password + "|" + _fullName;
}

void Admin::displayInfo() const
{
    std::cout << "[Admin] " << _fullName << " (ID: " << _id << ", @" << _username << ")\n";
}