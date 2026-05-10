/**
 * @file TeacherHandler.cpp
 * @brief Triển khai xử lý dữ liệu cho lớp Teacher.
 */

#include "TeacherHandler.h"
#include "Teacher.h"

TeacherHandler::TeacherHandler(const std::string &teacherFile)
    : _filePath(teacherFile)
{
}

// ------------------------------------------------------------
//  Interface Implementation
// ------------------------------------------------------------

std::string TeacherHandler::getRole() const
{
    return "Teacher";
}

std::string TeacherHandler::getFilePath() const
{
    return _filePath;
}

std::vector<std::string> TeacherHandler::serialize(
    const std::vector<std::unique_ptr<User>> &users)
{
    std::vector<std::string> result;
    for (const auto &u : users)
    {
        // Chỉ lưu các User có vai trò là Teacher vào tệp tương ứng.
        if (u && u->getRole() == "Teacher")
        {
            result.push_back(u->toFileString());
        }
    }
    return result;
}

std::vector<std::unique_ptr<User>> TeacherHandler::deserialize(
    const std::vector<std::string> &lines)
{
    std::vector<std::unique_ptr<User>> result;

    for (const auto &line : lines)
    {
        if (line.empty())
            continue;

        // --- Giai đoạn 1: Phân tách dữ liệu ---
        std::vector<std::string> fields;
        std::string temp = line;
        size_t pos = 0;
        while ((pos = temp.find('|')) != std::string::npos)
        {
            fields.push_back(temp.substr(0, pos));
            temp = temp.substr(pos + 1);
        }
        fields.push_back(temp);

        // --- Giai đoạn 2: Kiểm tra cấu trúc ---
        // Format yêu cầu 7 trường: Teacher|id|username|password|fullName|subject|assignedClass
        if (fields.empty() || fields[0] != "Teacher")
            continue;
        if (fields.size() < 7)
            continue;

        // --- Giai đoạn 3: Tái tạo đối tượng ---
        try
        {
            UserId id = std::stoi(fields[1]);
            std::string username = fields[2];
            std::string password = fields[3];
            std::string fullName = fields[4];
            std::string subject = fields[5];
            std::string assignedClass = fields[6];

            result.push_back(
                std::make_unique<Teacher>(id, username, password, fullName,
                                          subject, assignedClass));
        }
        catch (...)
        {
            // Bỏ qua các dòng bị lỗi định dạng dữ liệu (ví dụ: ID hỏng).
            continue;
        }
    }

    return result;
}