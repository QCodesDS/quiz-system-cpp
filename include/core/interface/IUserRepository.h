#ifndef _IUSERREPOSITORY_H_
#define _IUSERREPOSITORY_H_

// ============================================================
//  core/interfaces/IUserRepository.h
//  Contract cho mọi repository lưu trữ User (Admin/Teacher/Student)
//
//  Chỉ có pure virtual — không có data, không có implementation.
//  UserRepository (repositories/) implement interface này.
// ============================================================

#include <string>
#include <vector>
#include "User.h"

class IUserRepository
{
public:
    virtual ~IUserRepository() = default;

    virtual bool save(const std::vector<User *> &users) = 0;
    virtual std::vector<User *> load() = 0;
    virtual bool backup() = 0;

    virtual User *findById(UserId id) = 0;
    virtual User *findByUsername(const std::string &username) = 0;
    virtual bool usernameExists(const std::string &username) = 0;
};

#endif // !_IUSERREPOSITORY_H_