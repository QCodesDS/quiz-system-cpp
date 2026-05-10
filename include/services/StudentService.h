#ifndef STUDENT_SERVICE_H
#define STUDENT_SERVICE_H

#include "core/interface/IUserRepository.h"
#include "core/models/Student.h"
#include "core/types/enums.h"

/**
 * @class StudentService
 * @brief Quản lý toàn bộ nghiệp vụ liên quan đến Sinh viên.
 *
 * Đảm bảo các ràng buộc về dữ liệu (tuổi, số điện thoại)
 * trước khi lưu xuống Repository.
 */
class StudentService
{
private:
    IUserRepository *_userRepo;

    // Các hàm kiểm tra logic nghiệp vụ (Internal Validation)
    bool usernameExists(const std::string &username) const;
    bool isValidUsername(const std::string &username) const;
    bool isValidPassword(const std::string &password) const;
    bool isValidAge(int age) const;
    bool isValidPhone(const std::string &phone) const;

public:
    explicit StudentService(IUserRepository *userRepo);

    // --- CREATE ---
    bool addStudent(const std::string &username, const std::string &password,
                    const std::string &fullName, const std::string &className,
                    Gender gender, int age, const std::string &phone);

    // --- UPDATE ---
    bool updateStudent(UserId id, const std::string &fullName,
                       const std::string &className, Gender gender,
                       int age, const std::string &phone);

    // --- DELETE ---
    bool removeStudent(UserId id);

    // --- QUERY ---
    std::vector<Student *> getAllStudents() const;
    std::vector<Student *> getStudentsByClass(const std::string &className) const;
    Student *findStudentById(UserId id) const;

    // --- AUTH ---
    bool resetPassword(UserId id, const std::string &newHashedPassword);
};

#endif // STUDENT_SERVICE_H