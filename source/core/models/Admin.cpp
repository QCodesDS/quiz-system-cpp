/**
 * @file Admin.cpp
 * @brief Triển khai các phương thức của lớp Admin.
 *
 * File này định nghĩa các hành vi cụ thể của Admin như định dạng lưu trữ
 * và cách thức hiển thị thông tin, đảm bảo tính đa hình khi làm việc
 * với lớp cơ sở User.
 */

#include "Admin.h"
#include <iostream>

Admin::Admin(UserId id, const std::string &username,
             const std::string &password, const std::string &fullName)
    : User(id, username, password, fullName)
{
    // Khởi tạo thông qua constructor của lớp cha User.
}

std::string Admin::getRole() const
{
    return "Admin";
}

std::string Admin::toFileString() const
{
    /**
     * Định dạng chuỗi để lưu xuống file cơ sở dữ liệu (.txt).
     * Cấu trúc: Role|ID|Username|Password|FullName
     */
    return "Admin|" + std::to_string(_id) + "|" + _username + "|" + _password + "|" + _fullName;
}

void Admin::displayInfo() const
{
    // Hiển thị format đặc trưng của Admin để phân biệt trong giao diện Console.
    std::cout << "[Admin] " << _fullName
              << " (ID: " << _id
              << ", @" << _username << ")\n";
}