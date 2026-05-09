#ifndef TEACHER_HANDLER_H
#define TEACHER_HANDLER_H

// ============================================================
//  repositories/handlers/TeacherHandler.h
//
//  Concrete handler for Teacher type serialization/deserialization
// ============================================================

#include "IUserTypeHandler.h"
#include <string>

class TeacherHandler : public IUserTypeHandler
{
private:
    std::string filePath;

public:
    explicit TeacherHandler(const std::string &teacherFile);

    std::string getRole() const override;
    std::string getFilePath() const override;

    std::vector<std::string> serialize(
        const std::vector<std::unique_ptr<User>> &users) override;

    std::vector<std::unique_ptr<User>> deserialize(
        const std::vector<std::string> &lines) override;
};

#endif // TEACHER_HANDLER_H
