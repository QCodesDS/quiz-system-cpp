#ifndef IUSERTYPE_HANDLER_H
#define IUSERTYPE_HANDLER_H

// ============================================================
//  repositories/handlers/IUserTypeHandler.h
//
//  HANDLER PATTERN interface to eliminate if/else on role strings
//  in UserRepository.
//
//  Each concrete handler knows:
//  - Its role (Admin/Teacher/Student)
//  - Its file path
//  - How to parse its type (deserialize)
//  - How to format its type (serialize)
//
//  OPEN/CLOSED PRINCIPLE:
//  - Open for extension: Add new handler → new type supported
//  - Closed for modification: UserRepository unchanged
// ============================================================

#include <string>
#include <vector>
#include <memory>

class User;

class IUserTypeHandler
{
public:
    virtual ~IUserTypeHandler() = default;

    /**
     * @brief Get the role this handler manages (Admin/Teacher/Student)
     * @return std::string role string
     */
    virtual std::string getRole() const = 0;

    /**
     * @brief Get the file path this handler reads from/writes to
     * @return std::string file path
     */
    virtual std::string getFilePath() const = 0;

    /**
     * @brief Serialize users to file format strings
     * @param users Vector of unique_ptr<User> (borrowed reference, not transferred)
     * @return Vector of formatted strings ready to write to file
     */
    virtual std::vector<std::string> serialize(
        const std::vector<std::unique_ptr<User>> &users) = 0;

    /**
     * @brief Deserialize file lines to User objects
     * @param lines Vector of lines read from file
     * @return Vector of unique_ptr<User> (ownership transferred to caller)
     */
    virtual std::vector<std::unique_ptr<User>> deserialize(
        const std::vector<std::string> &lines) = 0;
};

#endif // IUSERTYPE_HANDLER_H
