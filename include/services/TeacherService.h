#ifndef TEACHER_SERVICE_H
#define TEACHER_SERVICE_H

#include <string>
#include <vector>
#include <memory>
#include "IUserRepository.h"
#include "Teacher.h"

// ============================================================
//  services/TeacherService.h
//
//  SINGLE RESPONSIBILITY: Manage Teacher CRUD operations
//  Reason to change: Only if Teacher business logic changes
//
//  Extracted from UserService (SRP violation fix)
//  Responsibilities:
//  - Create teachers with validation
//  - Update teacher information
//  - Remove teachers
//  - Query teachers
//  - Reset teacher password
//
//  SMART POINTERS: Uses vector<unique_ptr<User>> for all
//  operations. No manual delete. RAII struct removed.
// ============================================================

class TeacherService
{
private:
    IUserRepository *userRepo;

    // Validation helpers
    bool usernameExists(const std::string &username) const;
    bool isValidUsername(const std::string &username) const;
    bool isValidPassword(const std::string &password) const;

public:
    explicit TeacherService(IUserRepository *userRepo);

    // ========== CREATE ==========
    bool addTeacher(const std::string &username,
                    const std::string &password,
                    const std::string &fullName,
                    const std::string &subject,
                    const std::string &assignedClass = "");

    // ========== UPDATE ==========
    bool updateTeacher(UserId id,
                       const std::string &fullName,
                       const std::string &subject,
                       const std::string &assignedClass);

    // ========== DELETE ==========
    bool removeTeacher(UserId id);

    // ========== QUERY ==========
    std::vector<Teacher *> getAllTeachers() const;
    Teacher *findTeacherById(UserId id) const;

    // ========== PASSWORD ==========
    bool resetPassword(UserId id, const std::string &newHashedPassword);
};

#endif // TEACHER_SERVICE_H
