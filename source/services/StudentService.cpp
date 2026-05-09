#include "StudentService.h"
#include "IdGeneratorService.h"
#include <algorithm>
#include <memory>

// ============================================================
//  services/StudentService.cpp
//
//  SINGLE RESPONSIBILITY: Manage Student CRUD
//  Extracted from UserService (SRP fix)
// ============================================================

StudentService::StudentService(IUserRepository *userRepo)
    : userRepo(userRepo) {}

// ============================================================
//  Validation Helpers
// ============================================================

bool StudentService::usernameExists(const std::string &username) const
{
    return userRepo->usernameExists(username);
}

bool StudentService::isValidUsername(const std::string &username) const
{
    return !username.empty();
}

bool StudentService::isValidPassword(const std::string &password) const
{
    return password.size() >= 6;
}

bool StudentService::isValidAge(int age) const
{
    return age >= 10 && age <= 25;
}

bool StudentService::isValidPhone(const std::string &phone) const
{
    return phone.size() == 10;
}

// ============================================================
//  CREATE: Add new student
// ============================================================

bool StudentService::addStudent(const std::string &username,
                                const std::string &password,
                                const std::string &fullName,
                                const std::string &className,
                                Gender gender,
                                int age,
                                const std::string &phone)
{
    // Validation
    if (!isValidUsername(username))
        return false;
    if (!isValidPassword(password))
        return false;
    if (!isValidAge(age))
        return false;
    if (!isValidPhone(phone))
        return false;
    if (usernameExists(username))
        return false;

    // Load existing users (vector<unique_ptr<User>>)
    auto users = userRepo->load();

    // Generate new student ID
    UserId newId = 1000;
    for (const auto &u : users)
    {
        if (u && u->getRole() == "Student" && u->getId() > newId)
            newId = u->getId();
    }
    newId++;

    // Create new student
    users.push_back(
        std::make_unique<Student>(newId, username, password, fullName, className, gender, age, phone));

    // Persist
    return userRepo->save(users);
    // Vector auto-cleans up remaining unique_ptrs
}

// ============================================================
//  UPDATE: Modify existing student
// ============================================================

bool StudentService::updateStudent(UserId id,
                                   const std::string &fullName,
                                   const std::string &className,
                                   Gender gender,
                                   int age,
                                   const std::string &phone)
{
    // Validation
    if (!isValidAge(age))
        return false;
    if (!isValidPhone(phone))
        return false;

    auto users = userRepo->load();

    bool found = false;
    for (auto &u : users)
    {
        if (!u || u->getId() != id || u->getRole() != "Student")
            continue;

        Student *s = static_cast<Student *>(u.get());
        // Create updated student with same id/username/password
        // Reassign unique_ptr (old one auto-cleans)
        u = std::make_unique<Student>(
            s->getId(),
            s->getUsername(),
            s->getPassword(),
            fullName,
            className,
            gender,
            age,
            phone);
        found = true;
        break;
    }

    return found && userRepo->save(users);
    // Vector auto-cleans up
}

// ============================================================
//  DELETE: Remove student
// ============================================================

bool StudentService::removeStudent(UserId id)
{
    auto users = userRepo->load();

    auto it = std::find_if(users.begin(), users.end(),
                           [id](const std::unique_ptr<User> &u)
                           { return u && u->getId() == id && u->getRole() == "Student"; });

    if (it == users.end())
        return false;

    users.erase(it); // unique_ptr auto-cleans up
    return userRepo->save(users);
}
// Vector auto-cleans up remaining

// ============================================================
//  QUERY: Get students
// ============================================================

std::vector<Student *> StudentService::getAllStudents() const
{
    std::vector<Student *> result;
    auto users = userRepo->load();
    for (auto &u : users)
    {
        if (u && u->getRole() == "Student")
        {
            result.push_back(static_cast<Student *>(u.release()));
        }
    }
    return result;
}
// Remaining unique_ptrs auto-clean up

std::vector<Student *> StudentService::getStudentsByClass(const std::string &className) const
{
    std::vector<Student *> result;
    auto users = userRepo->load();
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
} // Remaining unique_ptrs auto-clean up

Student *StudentService::findStudentById(UserId id) const
{
    auto all = userRepo->load();
    for (auto &u : all)
    {
        if (u && u->getId() == id && u->getRole() == "Student")
        {
            return static_cast<Student *>(u.release());
        }
    }
    // Vector auto-cleans up if not found
    return nullptr;
}

// ============================================================
//  PASSWORD: Reset student password
// ============================================================

bool StudentService::resetPassword(UserId id, const std::string &newHashedPassword)
{
    auto users = userRepo->load();

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

    return found && userRepo->save(users);
    // Vector auto-cleans up
}
