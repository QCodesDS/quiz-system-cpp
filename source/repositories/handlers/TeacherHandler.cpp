#include "TeacherHandler.h"
#include "Teacher.h"
#include <memory>

TeacherHandler::TeacherHandler(const std::string &teacherFile)
    : filePath(teacherFile) {}

std::string TeacherHandler::getRole() const
{
    return "Teacher";
}

std::string TeacherHandler::getFilePath() const
{
    return filePath;
}

std::vector<std::string> TeacherHandler::serialize(
    const std::vector<std::unique_ptr<User>> &users)
{
    std::vector<std::string> result;
    for (const auto &u : users)
    {
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

        // Skip if first field is not "Teacher"
        if (fields.empty() || fields[0] != "Teacher")
            continue;

        // Parse: Teacher|id|username|password|fullName|subject|assignedClass
        if (fields.size() < 7)
            continue;

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
            // Skip malformed lines
            continue;
        }
    }

    return result;
}
