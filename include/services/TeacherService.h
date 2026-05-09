#ifndef TEACHER_SERVICE_H
#define TEACHER_SERVICE_H

#include <string>
#include <vector>
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
// ============================================================

class TeacherService
{
private:
    IUserRepository *userRepo;

    // RAII wrapper for automatic cleanup
    struct UserList
    {
        std::vector<User *> users;
        ~UserList()
        {
            for (auto *u : users)
                delete u;
        }
    };

    // Validation helpers
    bool usernameExists(const std::string &username) const;
    bool isValidUsername(const std::string &username) const;
    bool isValidPassword(const std::string &password) const;

public:
    explicit TeacherService(IUserRepository *userRepo);

    // ========== CREATE ==========
    // Add new teacher with validation
    // Returns: true if success, false if username exists or validation fails
    bool addTeacher(const std::string &username,
                    const std::string &password,
                    const std::string &fullName,
                    const std::string &subject,
                    const std::string &assignedClass = "");

    // ========== UPDATE ==========
    // Update teacher information (keeps id/username/password)
    // Returns: true if found and updated, false otherwise
    bool updateTeacher(UserId id,
                       const std::string &fullName,
                       const std::string &subject,
                       const std::string &assignedClass);

    // ========== DELETE ==========
    // Remove teacher by ID
    // Returns: true if found and removed, false otherwise
    bool removeTeacher(UserId id);

    // ========== QUERY ==========
    // Get all teachers (caller responsible for cleanup)
    std::vector<Teacher *> getAllTeachers() const;

    // Find teacher by ID (returns nullptr if not found, caller responsible for cleanup)
    Teacher *findTeacherById(UserId id) const;

    // ========== PASSWORD ==========
    // Reset teacher password (already hashed)
    // Returns: true if found and updated, false otherwise
    bool resetPassword(UserId id, const std::string &newHashedPassword);
};

#endif // TEACHER_SERVICE_H
