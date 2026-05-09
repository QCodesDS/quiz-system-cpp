#include "TeacherService.h"
#include "IdGeneratorService.h"
#include <algorithm>
#include <memory>

// ============================================================
//  services/TeacherService.cpp
//
//  SINGLE RESPONSIBILITY: Manage Teacher CRUD
//  Extracted from UserService (SRP fix)
// ============================================================

// ============================================================
//  Validation Helpers
// ============================================================

bool TeacherService::usernameExists(const std::string &username) const
{
    return userRepo->usernameExists(username);
}

bool TeacherService::isValidUsername(const std::string &username) const
{
    return !username.empty();
}

bool TeacherService::isValidPassword(const std::string &password) const
{
    return password.size() >= 6;
}

// ============================================================
//  CREATE: Add new teacher
// ============================================================

bool TeacherService::addTeacher(const std::string &username,
                                const std::string &password,
                                const std::string &fullName,
                                const std::string &subject,
                                const std::string &assignedClass)
{
    // Validation
    if (!isValidUsername(username))
        return false;
    if (!isValidPassword(password))
        return false;
    if (usernameExists(username))
        return false;

    // Load existing users (unique_ptr managed)
    auto users = userRepo->load();

    // Generate new teacher ID
    UserId newId = 2000;
    for (const auto &u : users)
    {
        if (u && u->getRole() == "Teacher" && u->getId() > newId)
            newId = u->getId();
    }
    newId++;

    // Create new teacher
    users.push_back(
        std::make_unique<Teacher>(newId, username, password, fullName, subject, assignedClass));

    // Persist
    return userRepo->save(users);
    // Vector goes out of scope and cleans up all unique_ptrs
}

// ============================================================
//  UPDATE: Modify existing teacher
// ============================================================

bool TeacherService::updateTeacher(UserId id,
                                   const std::string &fullName,
                                   const std::string &subject,
                                   const std::string &assignedClass)
{
    auto users = userRepo->load();

    bool found = false;
    for (auto &u : users)
    {
        if (!u || u->getId() != id || u->getRole() != "Teacher")
            continue;

        Teacher *t = static_cast<Teacher *>(u.get());
        // Create updated teacher with same id/username/password
        // Release from unique_ptr to create new one
        u = std::make_unique<Teacher>(
            t->getId(),
            t->getUsername(),
            t->getPassword(),
            fullName,
            subject,
            assignedClass);
        found = true;
        break;
    }

    return found && userRepo->save(users);
    // Vector auto-cleans up
}

// ============================================================
//  DELETE: Remove teacher
// ============================================================

bool TeacherService::removeTeacher(UserId id)
{
    auto users = userRepo->load();

    auto it = std::find_if(users.begin(), users.end(),
                           [id](const std::unique_ptr<User> &u)
                           { return u && u->getId() == id && u->getRole() == "Teacher"; });

    if (it == users.end())
        return false;

    users.erase(it); // unique_ptr auto-cleans up
    return userRepo->save(users);
    // Vector goes out of scope and cleans up remaining
}

// ============================================================
//  QUERY: Get teachers
// ============================================================

std::vector<Teacher *> TeacherService::getAllTeachers() const
{
    std::vector<Teacher *> result;
    auto users = userRepo->load();

    for (auto &u : users)
    {
        if (u && u->getRole() == "Teacher")
        {
            // Release ownership from unique_ptr, give raw pointer to caller
            result.push_back(static_cast<Teacher *>(u.release()));
        }
    }
    return result;
    // Remaining users in vector auto-clean up
}

Teacher *TeacherService::findTeacherById(UserId id) const
{
    auto users = userRepo->load();

    for (auto &u : users)
    {
        if (u && u->getId() == id && u->getRole() == "Teacher")
        {
            // Release and return raw pointer
            return static_cast<Teacher *>(u.release());
        }
    }
    return nullptr;
    // Remaining users auto-clean up
}

// ============================================================
//  PASSWORD: Reset teacher password
// ============================================================

bool TeacherService::resetPassword(UserId id, const std::string &newHashedPassword)
{
    auto users = userRepo->load();

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

    return found && userRepo->save(users);
    // Vector auto-cleans up
}
