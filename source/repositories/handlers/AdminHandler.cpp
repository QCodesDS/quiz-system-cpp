/**
 * @file AdminHandler.cpp
 * @brief Triển khai chi tiết việc chuyển đổi dữ liệu cho lớp Admin.
 */

#include "AdminHandler.h"
#include "Admin.h"
#include <memory>

AdminHandler::AdminHandler(const std::string &adminFile)
    : _filePath(adminFile)
{
}

// ------------------------------------------------------------
//  Interface Implementation
// ------------------------------------------------------------

std::string AdminHandler::getRole() const
{
    return "Admin";
}

std::string AdminHandler::getFilePath() const
{
    return _filePath;
}

std::vector<std::string> AdminHandler::serialize(
    const std::vector<std::unique_ptr<User>> &users)
{
    std::vector<std::string> result;
    for (const auto &u : users)
    {
        /**
         * Lọc dữ liệu: Chỉ những đối tượng có Role là Admin
         * mới được ghi vào file tương ứng của Handler này.
         */
        if (u && u->getRole() == "Admin")
        {
            result.push_back(u->toFileString());
        }
    }
    return result;
}

std::vector<std::unique_ptr<User>> AdminHandler::deserialize(
    const std::vector<std::string> &lines)
{
    std::vector<std::unique_ptr<User>> result;

    for (const auto &line : lines)
    {
        if (line.empty())
            continue;

        /**
         * Bước 1: Phân tách dòng thành các trường (Fields) dựa trên ký tự '|'.
         * Ví dụ: Admin|1|admin_user|pass_hash|Nguyen Van A
         */
        std::vector<std::string> fields;
        std::string temp = line;
        size_t pos = 0;
        while ((pos = temp.find('|')) != std::string::npos)
        {
            fields.push_back(temp.substr(0, pos));
            temp = temp.substr(pos + 1);
        }
        fields.push_back(temp);

        /**
         * Bước 2: Kiểm tra tính hợp lệ của Header và cấu trúc dữ liệu.
         */
        if (fields.empty() || fields[0] != "Admin")
            continue;
        if (fields.size() < 5)
            continue;

        /**
         * Bước 3: Tái tạo đối tượng Admin từ dữ liệu đã phân tách.
         */
        try
        {
            UserId id = std::stoi(fields[1]);
            std::string username = fields[2];
            std::string password = fields[3];
            std::string fullName = fields[4];

            result.push_back(
                std::make_unique<Admin>(id, username, password, fullName));
        }
        catch (...)
        {
            // Bỏ qua các dòng dữ liệu lỗi định dạng (ví dụ: ID không phải là số).
            continue;
        }
    }

    return result;
}