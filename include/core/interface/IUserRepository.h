#ifndef _IUSERREPOSITORY_H_
#define _IUSERREPOSITORY_H_

// ============================================================
//  core/interfaces/IUserRepository.h
//  Contract cho mọi repository lưu trữ User (Admin/Teacher/Student)
//
//  Chỉ có pure virtual — không có data, không có implementation.
//  UserRepository (repositories/) implement interface này.
//
//  SMART POINTER COMPLIANCE:
//  - load() returns owned data via unique_ptr
//  - save() borrows const reference (caller retains ownership)
//  - findById/findByUsername return owned data via unique_ptr
//  - No manual delete anywhere
// ============================================================

#include <string>
#include <vector>
#include <memory>
#include "User.h"

class IUserRepository
{
public:
    virtual ~IUserRepository() = default;

    // Save users to persistent storage (borrows, does not take ownership)
    virtual bool save(const std::vector<std::unique_ptr<User>> &users) = 0;

    // Load all users from persistent storage (transfers ownership)
    virtual std::vector<std::unique_ptr<User>> load() = 0;

    virtual bool backup() = 0;

    // Find user by ID (transfers ownership of found user)
    virtual std::unique_ptr<User> findById(UserId id) = 0;

    // Find user by username (transfers ownership of found user)
    virtual std::unique_ptr<User> findByUsername(const std::string &username) = 0;

    virtual bool usernameExists(const std::string &username) = 0;
};

#endif // !_IUSERREPOSITORY_H_