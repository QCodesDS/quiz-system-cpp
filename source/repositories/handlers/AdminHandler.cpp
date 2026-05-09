#include "AdminHandler.h"
#include "Admin.h"
#include <memory>

AdminHandler::AdminHandler(const std::string &adminFile)
    : filePath(adminFile) {}

std::string AdminHandler::getRole() const
{
    return "Admin";
}

std::string AdminHandler::getFilePath() const
{
    return filePath;
}

std::vector<std::string> AdminHandler::serialize(
    const std::vector<std::unique_ptr<User>> &users)
{
    std::vector<std::string> result;
    for (const auto &u : users)
    {
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

        // Skip if first field is not "Admin"
        if (fields.empty() || fields[0] != "Admin")
            continue;

        // Parse: Admin|id|username|password|fullName
        if (fields.size() < 5)
            continue;

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
            // Skip malformed lines
            continue;
        }
    }

    return result;
}
