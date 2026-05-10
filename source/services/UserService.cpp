#include "services/UserService.h"
#include "services/IdGeneratorService.h"
#include "services/TeacherService.h"
#include "services/StudentService.h"

UserService::UserService(IUserRepository *userRepo,
                         IdGeneratorService *idGen,
                         TeacherService *teacherSvc,
                         StudentService *studentSvc)
    : _userRepo(userRepo), _idGen(idGen),
      _teacherSvc(teacherSvc), _studentSvc(studentSvc)
{
}

// ------------------------------------------------------------
// Helper: Tìm kiếm User và quản lý quyền sở hữu bộ nhớ tạm thời
// ------------------------------------------------------------
User *UserService::findUserById(UserId id) const
{
    // load() trả về vector<unique_ptr<User>>
    auto all = _userRepo->load();
    for (auto &u : all)
    {
        if (u && u->getId() == id)
        {
            // release() để lấy con trỏ thô mà không làm delete đối tượng khi unique_ptr hết hạn
            return u.release();
        }
    }
    return nullptr;
}

// ------------------------------------------------------------
// Thao tác với Giáo viên
// ------------------------------------------------------------
bool UserService::addTeacher(const std::string &username, const std::string &password,
                             const std::string &fullName, const std::string &subject,
                             const std::string &assignedClass)
{
    return _teacherSvc->addTeacher(username, password, fullName, subject, assignedClass);
}

bool UserService::updateTeacher(UserId id, const std::string &fullName,
                                const std::string &subject, const std::string &assignedClass)
{
    return _teacherSvc->updateTeacher(id, fullName, subject, assignedClass);
}

// ------------------------------------------------------------
// Thao tác với Sinh viên
// ------------------------------------------------------------
bool UserService::addStudent(const std::string &username, const std::string &password,
                             const std::string &fullName, const std::string &className,
                             Gender gender, int age, const std::string &phone)
{
    return _studentSvc->addStudent(username, password, fullName, className, gender, age, phone);
}

bool UserService::updateStudent(UserId id, const std::string &fullName,
                                const std::string &className, Gender gender,
                                int age, const std::string &phone)
{
    return _studentSvc->updateStudent(id, fullName, className, gender, age, phone);
}

// ------------------------------------------------------------
// Điều hướng xóa User (Xác định vai trò để gọi Service đúng)
// ------------------------------------------------------------
bool UserService::removeUser(UserId id)
{
    std::unique_ptr<User> user(findUserById(id));
    if (!user)
        return false;

    std::string role = user->getRole();

    if (role == "Teacher")
        return _teacherSvc->removeTeacher(id);
    if (role == "Student")
        return _studentSvc->removeStudent(id);

    return false;
}

// ------------------------------------------------------------
// Điều hướng Reset Password
// ------------------------------------------------------------
bool UserService::resetPassword(UserId id, const std::string &newHashedPass)
{
    std::unique_ptr<User> user(findUserById(id));
    if (!user)
        return false;

    std::string role = user->getRole();

    if (role == "Teacher")
        return _teacherSvc->resetPassword(id, newHashedPass);
    if (role == "Student")
        return _studentSvc->resetPassword(id, newHashedPass);

    return false;
}

// ------------------------------------------------------------
// Truy vấn dữ liệu (Delegation)
// ------------------------------------------------------------
std::vector<Teacher *> UserService::getAllTeachers() const
{
    return _teacherSvc->getAllTeachers();
}

std::vector<Student *> UserService::getAllStudents() const
{
    return _studentSvc->getAllStudents();
}

std::vector<Student *> UserService::getStudentsByClass(const std::string &className) const
{
    return _studentSvc->getStudentsByClass(className);
}

User *UserService::findById(UserId id) const
{
    return findUserById(id);
}