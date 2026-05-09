#ifndef USER_REPOSITORY_H
#define USER_REPOSITORY_H

// ============================================================
//  repositories/UserRepository.h
//
//  Load từ 3 file riêng biệt (admin, teachers, students)
//  nhưng expose chung qua interface IUserRepository.
//
//  Chú ý memory: load() cấp phát User* bằng new.
//  AppInitializer hoặc caller phải delete khi không dùng nữa.
//  (Hoặc dùng unique_ptr nếu muốn an toàn hơn — TODO sau)
// ============================================================

#include "FileRepository.h"
#include "IUserRepository.h"
#include "Admin.h"
#include "Teacher.h"
#include "Student.h"

class UserRepository : public FileRepository, public IUserRepository
{
private:
    std::string adminFile;
    std::string teacherFile;
    std::string studentFile;

    // Parse từng dòng → đúng loại object
    Admin *parseAdmin(const std::vector<std::string> &fields) const;
    Teacher *parseTeacher(const std::vector<std::string> &fields) const;
    Student *parseStudent(const std::vector<std::string> &fields) const;

    // Helper chuyển string → Gender enum
    static Gender parseGender(const std::string &s);

    // Load riêng từng file
    std::vector<User *> loadAdmins() const;
    std::vector<User *> loadTeachers() const;
    std::vector<User *> loadStudents() const;

public:
    // filePath truyền vào FileRepository không dùng trực tiếp ở đây
    // nhưng cần để giữ interface — truyền studentFile làm mặc định
    UserRepository(const std::string &adminFile,
                   const std::string &teacherFile,
                   const std::string &studentFile);

    // --- IUserRepository ---
    bool save(const std::vector<User *> &users) override;
    std::vector<User *> load() override;
    bool backup() override;

    User *findById(UserId id) override;
    User *findByUsername(const std::string &username) override;
    bool usernameExists(const std::string &username) override;
};

#endif // USER_REPOSITORY_H