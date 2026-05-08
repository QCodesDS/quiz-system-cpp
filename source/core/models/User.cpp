#include "User.h"

User::User(UserId id, const std::string &username,
           const std::string &password, const std::string &fullName)
    : _id(id), _username(username), _password(password), _fullName(fullName) {}

UserId User::getId() const { return _id; }
std::string User::getUsername() const { return _username; }
std::string User::getFullName() const { return _fullName; }
std::string User::getPassword() const { return _password; }

bool User::setPassword(const std::string &newPass)
{
    // TODO: thay bằng hash thực khi tích hợp
    // Validation tối thiểu — độ dài kiểm tra ở Constants::MIN_PASS_LENGTH
    if (newPass.size() < 6)
        return false;
    _password = newPass;
    return true;
}