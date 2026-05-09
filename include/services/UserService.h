#ifndef USER_SERVICE_H
#define USER_SERVICE_H

#include <string>
#include <vector>
#include "IUserRepository.h"
#include "Teacher.h"
#include "Student.h"
#include "enums.h"

// Forward declarations to avoid circular includes
class IdGeneratorService;
class TeacherService;
class StudentService;

// ============================================================
//  services/UserService.h
//
//  REFACTORED: Facade pattern
//  Delegates to specialized services (SRP fix)
//
//  This service now has ONE responsibility:
//  - Coordinate between teacher/student services
//  - Maintain backward compatibility
//  - Generic user operations (remove by ID, reset password by ID)
//
//  EXTRACTED TO SEPARATE SERVICES:
//  - IdGeneratorService:  ID generation logic
//  - TeacherService:      Teacher CRUD & validation
//  - StudentService:      Student CRUD & validation
// ============================================================

class UserService
{
private:
    IUserRepository *userRepo;
    IdGeneratorService *idGen;
    TeacherService *teacherSvc;
    StudentService *studentSvc;

    // Helper: Find user by ID to determine type
    User *findUserById(UserId id) const;

public:
    // Constructor: inject all dependencies
    // NOTE: UserService does NOT own these - caller manages lifetime
    UserService(IUserRepository *userRepo,
                IdGeneratorService *idGen,
                TeacherService *teacherSvc,
                StudentService *studentSvc);

    // ========== TEACHER CRUD (delegates to TeacherService) ==========
    bool addTeacher(const std::string &username,
                    const std::string &password,
                    const std::string &fullName,
                    const std::string &subject,
                    const std::string &assignedClass = "");

    bool updateTeacher(UserId id,
                       const std::string &fullName,
                       const std::string &subject,
                       const std::string &assignedClass);

    // ========== STUDENT CRUD (delegates to StudentService) ==========
    bool addStudent(const std::string &username,
                    const std::string &password,
                    const std::string &fullName,
                    const std::string &className,
                    Gender gender,
                    int age,
                    const std::string &phone);

    bool updateStudent(UserId id,
                       const std::string &fullName,
                       const std::string &className,
                       Gender gender,
                       int age,
                       const std::string &phone);

    // ========== GENERIC OPERATIONS ==========
    // Remove any user by ID (determines type internally)
    // Returns: true if found and removed, false otherwise
    bool removeUser(UserId id);

    // Reset password for any user
    // Returns: true if found and updated, false otherwise
    bool resetPassword(UserId id, const std::string &newHashedPass);

    // ========== QUERIES ==========
    std::vector<Teacher *> getAllTeachers() const;
    std::vector<Student *> getAllStudents() const;
    std::vector<Student *> getStudentsByClass(const std::string &className) const;
    User *findById(UserId id) const;
};

#endif // USER_SERVICE_H