#include "UserService.h"
#include "IdGeneratorService.h"
#include "TeacherService.h"
#include "StudentService.h"
#include <algorithm>
#include <memory>

// ============================================================
//  services/UserService.cpp
//
//  REFACTORED: Facade pattern
//  Delegates to specialized services
//  Now has SINGLE responsibility: coordinate between services
// ============================================================

UserService::UserService(IUserRepository *userRepo,
                         IdGeneratorService *idGen,
                         TeacherService *teacherSvc,
                         StudentService *studentSvc)
    : userRepo(userRepo),
      idGen(idGen),
      teacherSvc(teacherSvc),
      studentSvc(studentSvc) {}

// ============================================================
//  Helper: Find user by ID (used by generic operations)
// ============================================================

User *UserService::findUserById(UserId id) const
{
    auto all = userRepo->load();
    for (auto &u : all)
    {
        if (u && u->getId() == id)
        {
            return static_cast<User *>(u.release());
        }
    }
    // Vector auto-cleans up if not found
    return nullptr;
}

// ============================================================
//  TEACHER CRUD: Delegate to TeacherService
// ============================================================

bool UserService::addTeacher(const std::string &username,
                             const std::string &password,
                             const std::string &fullName,
                             const std::string &subject,
                             const std::string &assignedClass)
{
    return teacherSvc->addTeacher(username, password, fullName, subject, assignedClass);
}

bool UserService::updateTeacher(UserId id,
                                const std::string &fullName,
                                const std::string &subject,
                                const std::string &assignedClass)
{
    return teacherSvc->updateTeacher(id, fullName, subject, assignedClass);
}

// ============================================================
//  STUDENT CRUD: Delegate to StudentService
// ============================================================

bool UserService::addStudent(const std::string &username,
                             const std::string &password,
                             const std::string &fullName,
                             const std::string &className,
                             Gender gender,
                             int age,
                             const std::string &phone)
{
    return studentSvc->addStudent(username, password, fullName, className, gender, age, phone);
}

bool UserService::updateStudent(UserId id,
                                const std::string &fullName,
                                const std::string &className,
                                Gender gender,
                                int age,
                                const std::string &phone)
{
    return studentSvc->updateStudent(id, fullName, className, gender, age, phone);
}

// ============================================================
//  GENERIC OPERATIONS: Remove user (delegates to appropriate service)
// ============================================================

bool UserService::removeUser(UserId id)
{
    // Find user to determine type
    std::unique_ptr<User> user(findUserById(id));
    if (!user)
        return false;

    std::string role = user->getRole();
    // user unique_ptr will auto-cleanup when it goes out of scope

    if (role == "Teacher")
    {
        return teacherSvc->removeTeacher(id);
    }
    else if (role == "Student")
    {
        return studentSvc->removeStudent(id);
    }
    // Admin users should not be removed through this path
    return false;
}

// ============================================================
//  GENERIC OPERATIONS: Reset password (delegates to appropriate service)
// ============================================================

bool UserService::resetPassword(UserId id, const std::string &newHashedPass)
{
    // Find user to determine type
    std::unique_ptr<User> user(findUserById(id));
    if (!user)
        return false;

    std::string role = user->getRole();
    // user unique_ptr will auto-cleanup when it goes out of scope

    if (role == "Teacher")
    {
        return teacherSvc->resetPassword(id, newHashedPass);
    }
    else if (role == "Student")
    {
        return studentSvc->resetPassword(id, newHashedPass);
    }
    // Admin password reset could be handled separately
    return false;
}

// ============================================================
//  QUERIES: Delegate to appropriate services
// ============================================================

std::vector<Teacher *> UserService::getAllTeachers() const
{
    return teacherSvc->getAllTeachers();
}

std::vector<Student *> UserService::getAllStudents() const
{
    return studentSvc->getAllStudents();
}

std::vector<Student *> UserService::getStudentsByClass(const std::string &className) const
{
    return studentSvc->getStudentsByClass(className);
}

User *UserService::findById(UserId id) const
{
    return findUserById(id);
}