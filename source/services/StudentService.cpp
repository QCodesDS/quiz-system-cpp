#include "services/StudentService.h"
#include <algorithm>

StudentService::StudentService(IUserRepository *userRepo)
    : _userRepo(userRepo)
{
}

// ------------------------------------------------------------
// Validation Logic
// ------------------------------------------------------------
bool StudentService::usernameExists(const std::string &username) const
{
    return _userRepo->usernameExists(username);
}

bool StudentService::isValidUsername(const std::string &username) const
{
    return !username.empty() && username.length() >= 3;
}

bool StudentService::isValidPassword(const std::string &password) const
{
    return password.length() >= 6;
}

bool StudentService::isValidAge(int age) const
{
    return age >= 10 && age <= 25; // Giới hạn độ tuổi học sinh/sinh viên
}

bool StudentService::isValidPhone(const std::string &phone) const
{
    // Kiểm tra độ dài và đảm bảo toàn chữ số
    return phone.length() == 10 &&
           std::all_of(phone.begin(), phone.end(), ::isdigit);
}

// ------------------------------------------------------------
// Create: Thêm sinh viên mới
// ------------------------------------------------------------
bool StudentService::addStudent(const std::string &username, const std::string &password,
                                const std::string &fullName, const std::string &className,
                                Gender gender, int age, const std::string &phone)
{
    if (!isValidUsername(username) || !isValidPassword(password) ||
        !isValidAge(age) || !isValidPhone(phone) || usernameExists(username))
    {
        return false;
    }

    auto users = _userRepo->load();

    // Tự động sinh ID cho Student (bắt đầu từ 1001)
    UserId nextId = 1000;
    for (const auto &u : users)
    {
        if (u && u->getRole() == "Student" && u->getId() > nextId)
        {
            nextId = u->getId();
        }
    }
    nextId++;

    users.push_back(std::make_unique<Student>(
        nextId, username, password, fullName, className, gender, age, phone));

    return _userRepo->save(users);
}

// ------------------------------------------------------------
// Update: Cập nhật thông tin (vẫn giữ ID và Password cũ)
// ------------------------------------------------------------
bool StudentService::updateStudent(UserId id, const std::string &fullName,
                                   const std::string &className, Gender gender,
                                   int age, const std::string &phone)
{
    if (!isValidAge(age) || !isValidPhone(phone))
        return false;

    auto users = _userRepo->load();
    bool found = false;

    for (auto &u : users)
    {
        if (u && u->getId() == id && u->getRole() == "Student")
        {
            Student *s = static_cast<Student *>(u.get());
            // Cập nhật thông tin chi tiết
            s->setFullName(fullName);
            s->setClassName(className);
            s->setGender(gender);
            s->setAge(age);
            s->setPhone(phone);
            found = true;
            break;
        }
    }

    return found && _userRepo->save(users);
}

// ------------------------------------------------------------
// Delete
// ------------------------------------------------------------
bool StudentService::removeStudent(UserId id)
{
    auto users = _userRepo->load();
    auto it = std::find_if(users.begin(), users.end(), [id](const std::unique_ptr<User> &u)
                           { return u && u->getId() == id && u->getRole() == "Student"; });

    if (it == users.end())
        return false;

    users.erase(it);
    return _userRepo->save(users);
}

// ------------------------------------------------------------
// Query: Các hàm truy vấn trả về raw pointers cho UI
// ------------------------------------------------------------
std::vector<Student *> StudentService::getAllStudents() const
{
    std::vector<Student *> result;
    auto users = _userRepo->load();
    for (auto &u : users)
    {
        if (u && u->getRole() == "Student")
        {
            result.push_back(static_cast<Student *>(u.release()));
        }
    }
    return result;
}

std::vector<Student *> StudentService::getStudentsByClass(const std::string &className) const
{
    std::vector<Student *> result;
    auto users = _userRepo->load();
    for (auto &u : users)
    {
        if (u && u->getRole() == "Student")
        {
            Student *s = static_cast<Student *>(u.get());
            if (s->getClassName() == className)
            {
                result.push_back(static_cast<Student *>(u.release()));
            }
        }
    }
    return result;
}

Student *StudentService::findStudentById(UserId id) const
{
    auto users = _userRepo->load();
    for (auto &u : users)
    {
        if (u && u->getId() == id && u->getRole() == "Student")
        {
            return static_cast<Student *>(u.release());
        }
    }
    return nullptr;
}

bool StudentService::resetPassword(UserId id, const std::string &newHashedPassword)
{
    auto users = _userRepo->load();
    bool found = false;
    for (auto &u : users)
    {
        if (u && u->getId() == id && u->getRole() == "Student")
        {
            u->setPassword(newHashedPassword);
            found = true;
            break;
        }
    }
    return found && _userRepo->save(users);
}