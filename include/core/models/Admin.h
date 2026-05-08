#ifndef _ADMIN_H_
#define _ADMIN_H_

#include "User.h"

// ============================================================
//  core/models/Admin.h
//  Admin không có data riêng — chỉ override role và format
// ============================================================

class Admin : public User
{
public:
    Admin(UserId id, const std::string &username,
          const std::string &password, const std::string &fullName);

    std::string getRole() const override;      // "Admin"
    std::string toFileString() const override; // "Admin|id|user|pass|name"
    void displayInfo() const override;
};

#endif // !_ADMIN_H_