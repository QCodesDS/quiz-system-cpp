#ifndef _IAUTHSERVICE_H_
#define _IAUTHSERVICE_H_

// ============================================================
//  core/interfaces/IAuthService.h
// ============================================================

#include <string>
#include "../models/User.h"

class IAuthService
{
public:
    virtual ~IAuthService() = default;

    // Trả nullptr nếu sai thông tin
    virtual User *login(const std::string &username, const std::string &password) = 0;
    virtual void logout() = 0;
    virtual bool changePassword(User *user, const std::string &newPass) = 0;
    virtual User *getCurrentUser() = 0;
    virtual bool validateCredentials(const std::string &username,
                                     const std::string &password) = 0;
};

#endif // !_IAUTHSERVICE_H_