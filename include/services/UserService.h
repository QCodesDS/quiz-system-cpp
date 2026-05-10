#ifndef USER_SERVICE_H
#define USER_SERVICE_H

#include "core/interface/IUserRepository.h"
#include "core/models/Teacher.h"
#include "core/models/Student.h"

// Forward declarations
class IdGeneratorService;
class TeacherService;
class StudentService;

/**
 * @class UserService
 * @brief Facade Service - Điểm truy cập duy nhất cho các tác vụ liên quan đến người dùng.
 *
 * Tuân thủ SRP bằng cách ủy quyền logic chi tiết cho TeacherService và StudentService.
 */
class UserService
{
private:
    IUserRepository *_userRepo;
    IdGeneratorService *_idGen;
    TeacherService *_teacherSvc;
    StudentService *_studentSvc;

    // Helper nội bộ để xác định loại người dùng trước khi điều hướng
    User *findUserById(UserId id) const;

public:
    UserService(IUserRepository *userRepo,
                IdGeneratorService *idGen,
                TeacherService *teacherSvc,
                StudentService *studentSvc);

    // --- Quản lý Giáo viên (Delegates to TeacherService) ---
    bool addTeacher(const std::string &username, const std::string &password,
                    const std::string &fullName, const std::string &subject,
                    const std::string &assignedClass = "");

    bool updateTeacher(UserId id, const std::string &fullName,
                       const std::string &subject, const std::string &assignedClass);

    // --- Quản lý Sinh viên (Delegates to StudentService) ---
    bool addStudent(const std::string &username, const std::string &password,
                    const std::string &fullName, const std::string &className,
                    Gender gender, int age, const std::string &phone);

    bool updateStudent(UserId id, const std::string &fullName,
                       const std::string &className, Gender gender,
                       int age, const std::string &phone);

    // --- Các thao tác dùng chung (Generic) ---
    bool removeUser(UserId id);
    bool resetPassword(UserId id, const std::string &newHashedPass);

    // --- Truy vấn dữ liệu ---
    std::vector<Teacher *> getAllTeachers() const;
    std::vector<Student *> getAllStudents() const;
    std::vector<Student *> getStudentsByClass(const std::string &className) const;
    User *findById(UserId id) const;
};

#endif // USER_SERVICE_H