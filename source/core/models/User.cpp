#include "User.h"

User::User(UserId id,
           const std::string &username,
           const std::string &password,
           const std::string &fullName)
    : _id(id),
      _username(username),
      _password(password),
      _fullName(fullName)
{
}

UserId User::getId() const
{
    return _id;
}

std::string User::getUsername() const
{
    return _username;
}

std::string User::getFullName() const
{
    return _fullName;
}

std::string User::getPassword() const
{
    return _password;
}

bool User::setPassword(const std::string &newPass)
{
    // Kiểm tra độ dài mật khẩu tối thiểu
    if (newPass.size() < 6)
    {
        return false;
    }

    // TODO:
    // Thực hiện hash password trước khi lưu
    // khi tích hợp module bảo mật thực tế.
    _password = newPass;

    return true;
}