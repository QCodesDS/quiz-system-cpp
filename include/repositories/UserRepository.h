#ifndef USER_REPOSITORY_H
#define USER_REPOSITORY_H

// ============================================================
//  repositories/UserRepository.h
//
//  Load từ 3 file riêng biệt (admin, teachers, students)
//  nhưng expose chung qua interface IUserRepository.
//
//  SMART POINTER: All User* allocated with new → returned as unique_ptr
//  Ownership transferred to caller via unique_ptr
//  No manual delete in caller code
// ============================================================

#include "FileRepository.h"
#include "IUserRepository.h"
#include "Admin.h"
#include "Teacher.h"
#include "Student.h"
#include <memory>

class UserRepository : public FileRepository, public IUserRepository
{
private:
    std::string adminFile;
    std::string teacherFile;
    std::string studentFile;

    // Parse từng dòng → đúng loại object (returns unique_ptr)
    std::unique_ptr<Admin> parseAdmin(const std::vector<std::string> &fields) const;
    std::unique_ptr<Teacher> parseTeacher(const std::vector<std::string> &fields) const;
    std::unique_ptr<Student> parseStudent(const std::vector<std::string> &fields) const;

    // Helper chuyển string → Gender enum
    static Gender parseGender(const std::string &s);

    // Load riêng từng file
    std::vector<std::unique_ptr<User>> loadAdmins() const;
    std::vector<std::unique_ptr<User>> loadTeachers() const;
    std::vector<std::unique_ptr<User>> loadStudents() const;

public:
    UserRepository(const std::string &adminFile,
                   const std::string &teacherFile,
                   const std::string &studentFile);

    // --- IUserRepository ---
    bool save(const std::vector<std::unique_ptr<User>> &users) override;
    std::vector<std::unique_ptr<User>> load() override;
    bool backup() override;

    std::unique_ptr<User> findById(UserId id) override;
    std::unique_ptr<User> findByUsername(const std::string &username) override;
    bool usernameExists(const std::string &username) override;
};

#endif // USER_REPOSITORY_H