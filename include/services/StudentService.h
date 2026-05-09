#ifndef STUDENT_SERVICE_H
#define STUDENT_SERVICE_H

#include <string>
#include <vector>
#include <memory>
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
//
//  SMART POINTERS: Uses vector<unique_ptr<User>> for all
//  operations. No manual delete. RAII struct removed.
// ============================================================

class StudentService
{
private:
    IUserRepository *userRepo;

    // Validation helpers
    bool usernameExists(const std::string &username) const;
    bool isValidUsername(const std::string &username) const;
    bool isValidPassword(const std::string &password) const;
    bool isValidAge(int age) const;
    bool isValidPhone(const std::string &phone) const;

public:
    explicit StudentService(IUserRepository *userRepo);

    // ========== CREATE ==========
    bool addStudent(const std::string &username,
                    const std::string &password,
                    const std::string &fullName,
                    const std::string &className,
                    Gender gender,
                    int age,
                    const std::string &phone);

    // ========== UPDATE ==========
    bool updateStudent(UserId id,
                       const std::string &fullName,
                       const std::string &className,
                       Gender gender,
                       int age,
                       const std::string &phone);

    // ========== DELETE ==========
    bool removeStudent(UserId id);

    // ========== QUERY ==========
    std::vector<Student *> getAllStudents() const;
    std::vector<Student *> getStudentsByClass(const std::string &className) const;
    Student *findStudentById(UserId id) const;

    // ========== PASSWORD ==========
    bool resetPassword(UserId id, const std::string &newHashedPassword);
};

#endif // STUDENT_SERVICE_H
