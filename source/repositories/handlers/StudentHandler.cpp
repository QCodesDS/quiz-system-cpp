#include "StudentHandler.h"
#include "Student.h"
#include "enums.h"
#include <memory>

StudentHandler::StudentHandler(const std::string &studentFile)
    : filePath(studentFile) {}

std::string StudentHandler::getRole() const
{
    return "Student";
}

std::string StudentHandler::getFilePath() const
{
    return filePath;
}

std::vector<std::string> StudentHandler::serialize(
    const std::vector<std::unique_ptr<User>> &users)
{
    std::vector<std::string> result;
    for (const auto &u : users)
    {
        if (u && u->getRole() == "Student")
        {
            result.push_back(u->toFileString());
        }
    }
    return result;
}

Gender parseGender(const std::string &s)
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
        // Skip empty lines
        if (line.empty())
            continue;

        // Split line by '|'
        std::vector<std::string> fields;
        std::string temp = line;
        size_t pos = 0;
        while ((pos = temp.find('|')) != std::string::npos)
        {
            fields.push_back(temp.substr(0, pos));
            temp = temp.substr(pos + 1);
        }
        fields.push_back(temp); // Last field

        // Skip if first field is not "Student"
        if (fields.empty() || fields[0] != "Student")
            continue;

        // Parse: Student|id|username|password|fullName|className|gender|age|phone
        if (fields.size() < 9)
            continue;

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
            // Skip malformed lines
            continue;
        }
    }

    return result;
}
