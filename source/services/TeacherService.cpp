#include "services/TeacherService.h"
#include "services/AuthService.h"
#include <algorithm>

TeacherService::TeacherService(IUserRepository *userRepo)
    : _userRepo(userRepo)
{
}

// ------------------------------------------------------------
// Validation
// ------------------------------------------------------------
bool TeacherService::usernameExists(const std::string &username) const
{
    return _userRepo->usernameExists(username);
}

bool TeacherService::isValidUsername(const std::string &username) const
{
    return !username.empty() && username.length() >= 3;
}

bool TeacherService::isValidPassword(const std::string &password) const
{
    return password.length() >= 6;
}

// ------------------------------------------------------------
// Create: Thêm giảng viên mới
// ------------------------------------------------------------
bool TeacherService::addTeacher(const std::string &username, const std::string &password,
                                const std::string &fullName, const std::string &subject,
                                const std::string &assignedClass)
{
    if (!isValidUsername(username) || !isValidPassword(password) || usernameExists(username))
    {
        return false;
    }

    auto users = _userRepo->load();

    // Tự động sinh ID cho Teacher (bắt đầu từ 2001)
    UserId nextId = 2000;
    for (const auto &u : users)
    {
        if (u && u->getRole() == "Teacher" && u->getId() > nextId)
        {
            nextId = u->getId();
        }
    }
    nextId++;

    users.push_back(std::make_unique<Teacher>(
        nextId, username, AuthService::hashPassword(password),
        fullName, subject, assignedClass));

    return _userRepo->save(users);
}

// ------------------------------------------------------------
// Update: Cập nhật thông tin giảng viên
// ------------------------------------------------------------
bool TeacherService::updateTeacher(UserId id, const std::string &fullName,
                                   const std::string &subject, const std::string &assignedClass)
{
    auto users = _userRepo->load();
    bool found = false;

    for (auto &u : users)
    {
        if (u && u->getId() == id && u->getRole() == "Teacher")
        {
            Teacher *t = static_cast<Teacher *>(u.get());
            // Cập nhật các trường thông tin cho phép
            t->setFullName(fullName);
            t->setSubject(subject);
            t->setAssignedClass(assignedClass);
            found = true;
            break;
        }
    }

    return found && _userRepo->save(users);
}

// ------------------------------------------------------------
// Delete
// ------------------------------------------------------------
bool TeacherService::removeTeacher(UserId id)
{
    auto users = _userRepo->load();
    auto it = std::find_if(users.begin(), users.end(), [id](const std::unique_ptr<User> &u)
                           { return u && u->getId() == id && u->getRole() == "Teacher"; });

    if (it == users.end())
        return false;

    users.erase(it);
    return _userRepo->save(users);
}

// ------------------------------------------------------------
// Query: Trả về danh sách raw pointers cho UI
// ------------------------------------------------------------
std::vector<Teacher *> TeacherService::getAllTeachers() const
{
    std::vector<Teacher *> result;
    auto users = _userRepo->load();

    for (auto &u : users)
    {
        if (u && u->getRole() == "Teacher")
        {
            // Sử dụng u.release() ở đây là nguy hiểm nếu không cẩn thận.
            // Giải pháp tốt nhất cho UI là trả về một bản copy hoặc trỏ vào data hiện có.
            // Ở đây ta dùng release() để chuyển quyền sở hữu tạm thời cho vector kết quả.
            result.push_back(static_cast<Teacher *>(u.release()));
        }
    }
    return result;
}

Teacher *TeacherService::findTeacherById(UserId id) const
{
    auto users = _userRepo->load();
    for (auto &u : users)
    {
        if (u && u->getId() == id && u->getRole() == "Teacher")
        {
            return static_cast<Teacher *>(u.release());
        }
    }
    return nullptr;
}

bool TeacherService::resetPassword(UserId id, const std::string &newHashedPassword)
{
    auto users = _userRepo->load();
    bool found = false;

    for (auto &u : users)
    {
        if (u && u->getId() == id && u->getRole() == "Teacher")
        {
            u->setPassword(newHashedPassword);
            found = true;
            break;
        }
    }
    return found && _userRepo->save(users);
}