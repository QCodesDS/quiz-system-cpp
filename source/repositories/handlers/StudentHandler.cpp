/**
 * @file StudentHandler.cpp
 * @brief Triển khai logic Serialization và Deserialization cho Student.
 */

#include "StudentHandler.h"
#include "Student.h"
#include "enums.h"

StudentHandler::StudentHandler(const std::string &studentFile)
    : _filePath(studentFile)
{
}

// ------------------------------------------------------------
//  Interface Implementation
// ------------------------------------------------------------

std::string StudentHandler::getRole() const
{
    return "Student";
}

std::string StudentHandler::getFilePath() const
{
    return _filePath;
}

std::vector<std::string> StudentHandler::serialize(
    const std::vector<std::unique_ptr<User>> &users)
{
    std::vector<std::string> result;
    for (const auto &u : users)
    {
        // Lọc và chỉ serialize những đối tượng thực sự là Student.
        if (u && u->getRole() == "Student")
        {
            result.push_back(u->toFileString());
        }
    }
    return result;
}

/**
 * @brief Helper tĩnh để chuyển đổi chuỗi văn bản thành Enum Gender.
 * Giúp mã nguồn chính trong deserialize gọn gàng hơn.
 */
static Gender parseGender(const std::string &s)
{
    if (s == "Female")
        return Gender::Female;
    if (s == "Other")
        return Gender::Other;
    return Gender::Male;
}

std::vector<std::unique_ptr<User>> StudentHandler::deserialize(
    const std::vector<std::string> &lines)
{
    std::vector<std::unique_ptr<User>> result;

    for (const auto &line : lines)
    {
        if (line.empty())
            continue;

        // --- Giai đoạn 1: Tokenizing (Phân tách chuỗi) ---
        std::vector<std::string> fields;
        std::string temp = line;
        size_t pos = 0;
        while ((pos = temp.find('|')) != std::string::npos)
        {
            fields.push_back(temp.substr(0, pos));
            temp = temp.substr(pos + 1);
        }
        fields.push_back(temp);

        // --- Giai đoạn 2: Validation (Kiểm tra cấu trúc) ---
        // Format yêu cầu 9 trường: Role|Id|User|Pass|Name|Class|Gender|Age|Phone
        if (fields.empty() || fields[0] != "Student")
            continue;
        if (fields.size() < 9)
            continue;

        // --- Giai đoạn 3: Parsing & Object Creation ---
        try
        {
            UserId id = std::stoi(fields[1]);
            std::string username = fields[2];
            std::string password = fields[3];
            std::string fullName = fields[4];
            std::string className = fields[5];
            Gender gender = parseGender(fields[6]);
            int age = std::stoi(fields[7]);
            std::string phone = fields[8];

            result.push_back(
                std::make_unique<Student>(id, username, password, fullName,
                                          className, gender, age, phone));
        }
        catch (...)
        {
            // Bỏ qua các bản ghi lỗi dữ liệu để tránh làm gián đoạn việc load toàn bộ file.
            continue;
        }
    }

    return result;
}