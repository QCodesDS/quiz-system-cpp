/**
 * @file User.cpp
 * @brief Triển khai các hành vi chung của lớp cơ sở User.
 */

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
    // Thành viên được khởi tạo thông qua Member Initializer List.
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
    /**
     * Kiểm tra tính hợp lệ của mật khẩu (Validation logic).
     * Hiện tại chỉ kiểm tra độ dài tối thiểu là 6.
     */
    if (newPass.size() < 6)
    {
        return false;
    }

    /**
     * @todo Tích hợp thêm module mã hóa (hashing) trước khi gán
     * vào biến _password để tăng tính bảo mật cho hệ thống.
     */
    _password = newPass;

    return true;
}