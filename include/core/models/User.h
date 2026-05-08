#ifndef _USER_H_
#define _USER_H_

#include <string>
#include "../types/typedefs.h"

// ============================================================
//  core/models/User.h
//  Abstract base class cho Admin, Teacher, Student
//
//  Quy tắc:
//  - Fields private/protected, truy cập qua getter/setter
//  - Không có cout, cin, file I/O
//  - getRole() / toFileString() / displayInfo() là pure virtual
//    → mỗi subclass bắt buộc phải implement
// ============================================================

class User
{
protected:
    UserId _id;
    std::string _username;
    std::string _password; // lưu dạng hash
    std::string _fullName;

public:
    User(UserId id, const std::string &username,
         const std::string &password, const std::string &fullName);

    virtual ~User() = default;

    // --- Getters ---
    UserId getId() const;
    std::string getUsername() const;
    std::string getFullName() const;

    // --- Setter có validation ---
    // Trả false nếu newPass không hợp lệ (quá ngắn, ...)
    bool setPassword(const std::string &newPass);

    // Dùng nội bộ để xác thực login — trả hash của password
    std::string getPassword() const;

    // --- Pure virtual — subclass bắt buộc override ---
    virtual std::string getRole() const = 0;
    virtual std::string toFileString() const = 0;
    virtual void displayInfo() const = 0;
};

#endif // !_USER_H_