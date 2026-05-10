#ifndef TEACHER_SERVICE_H
#define TEACHER_SERVICE_H

#include "core/interface/IUserRepository.h"
#include "core/models/Teacher.h"

/**
 * @class TeacherService
 * @brief Quản lý các nghiệp vụ liên quan riêng đến Giảng viên.
 *
 * Đã tách biệt hoàn toàn khỏi StudentService để tuân thủ SRP.
 */
class TeacherService
{
private:
    IUserRepository *_userRepo;

    // Helper kiểm tra tính hợp lệ
    bool usernameExists(const std::string &username) const;
    bool isValidUsername(const std::string &username) const;
    bool isValidPassword(const std::string &password) const;

public:
    explicit TeacherService(IUserRepository *userRepo);

    // --- CREATE ---
    bool addTeacher(const std::string &username, const std::string &password,
                    const std::string &fullName, const std::string &subject,
                    const std::string &assignedClass = "");

    // --- UPDATE ---
    bool updateTeacher(UserId id, const std::string &fullName,
                       const std::string &subject, const std::string &assignedClass);

    // --- DELETE ---
    bool removeTeacher(UserId id);

    // --- QUERY ---
    std::vector<Teacher *> getAllTeachers() const;
    Teacher *findTeacherById(UserId id) const;

    // --- AUTH/SECURITY ---
    bool resetPassword(UserId id, const std::string &newHashedPassword);
};

#endif // TEACHER_SERVICE_H