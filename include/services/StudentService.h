#ifndef STUDENT_SERVICE_H
#define STUDENT_SERVICE_H

#include <string>
#include <vector>
#include "IUserRepository.h"
#include "Student.h"
#include "enums.h"

// ============================================================
//  services/StudentService.h
//
//  SINGLE RESPONSIBILITY: Manage Student CRUD operations
//  Reason to change: Only if Student business logic changes
//
//  Extracted from UserService (SRP violation fix)
//  Responsibilities:
//  - Create students with validation (age, phone)
//  - Update student information
//  - Remove students
//  - Query students (by ID, by class)
//  - Reset student password
// ============================================================

class StudentService
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
    bool isValidAge(int age) const;
    bool isValidPhone(const std::string &phone) const;

public:
    explicit StudentService(IUserRepository *userRepo);

    // ========== CREATE ==========
    // Add new student with validation
    // Returns: true if success, false if username exists or validation fails
    bool addStudent(const std::string &username,
                    const std::string &password,
                    const std::string &fullName,
                    const std::string &className,
                    Gender gender,
                    int age,
                    const std::string &phone);

    // ========== UPDATE ==========
    // Update student information (keeps id/username/password)
    // Returns: true if found and updated, false otherwise
    bool updateStudent(UserId id,
                       const std::string &fullName,
                       const std::string &className,
                       Gender gender,
                       int age,
                       const std::string &phone);

    // ========== DELETE ==========
    // Remove student by ID
    // Returns: true if found and removed, false otherwise
    bool removeStudent(UserId id);

    // ========== QUERY ==========
    // Get all students (caller responsible for cleanup)
    std::vector<Student *> getAllStudents() const;

    // Get students by class (caller responsible for cleanup)
    std::vector<Student *> getStudentsByClass(const std::string &className) const;

    // Find student by ID (returns nullptr if not found, caller responsible for cleanup)
    Student *findStudentById(UserId id) const;

    // ========== PASSWORD ==========
    // Reset student password (already hashed)
    // Returns: true if found and updated, false otherwise
    bool resetPassword(UserId id, const std::string &newHashedPassword);
};

#endif // STUDENT_SERVICE_H
