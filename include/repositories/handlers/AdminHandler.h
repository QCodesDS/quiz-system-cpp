#ifndef ADMIN_HANDLER_H
#define ADMIN_HANDLER_H

// ============================================================
//  repositories/handlers/AdminHandler.h
//
//  Concrete handler for Admin type serialization/deserialization
// ============================================================

#include "IUserTypeHandler.h"
#include <string>

class AdminHandler : public IUserTypeHandler
{
private:
    std::string filePath;

public:
    explicit AdminHandler(const std::string &adminFile);

    std::string getRole() const override;
    std::string getFilePath() const override;

    std::vector<std::string> serialize(
        const std::vector<std::unique_ptr<User>> &users) override;

    std::vector<std::unique_ptr<User>> deserialize(
        const std::vector<std::string> &lines) override;
};

#endif // ADMIN_HANDLER_H
