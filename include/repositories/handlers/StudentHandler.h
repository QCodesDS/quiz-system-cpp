#ifndef STUDENT_HANDLER_H
#define STUDENT_HANDLER_H

// ============================================================
//  repositories/handlers/StudentHandler.h
//
//  Concrete handler for Student type serialization/deserialization
// ============================================================

#include "IUserTypeHandler.h"
#include <string>

class StudentHandler : public IUserTypeHandler
{
private:
    std::string filePath;

public:
    explicit StudentHandler(const std::string &studentFile);

    std::string getRole() const override;
    std::string getFilePath() const override;

    std::vector<std::string> serialize(
        const std::vector<std::unique_ptr<User>> &users) override;

    std::vector<std::unique_ptr<User>> deserialize(
        const std::vector<std::string> &lines) override;
};

#endif // STUDENT_HANDLER_H
