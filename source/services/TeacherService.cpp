#include "TeacherService.h"
#include "IdGeneratorService.h"
#include <algorithm>

// ============================================================
//  services/TeacherService.cpp
//
//  SINGLE RESPONSIBILITY: Manage Teacher CRUD
//  Extracted from UserService (SRP fix)
// ============================================================

TeacherService::TeacherService(IUserRepository *userRepo)
    : userRepo(userRepo) {}

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

    // Load existing users
    UserList ul;
    ul.users = userRepo->load();

    // Generate new teacher ID
    UserId newId = 2000;
    for (const auto *u : ul.users)
    {
        if (u && u->getRole() == "Teacher" && u->getId() > newId)
            newId = u->getId();
    }
    newId++;

    // Create new teacher (caller via repository allocates, we add to list)
    ul.users.push_back(
        new Teacher(newId, username, password, fullName, subject, assignedClass));

    // Persist
    return userRepo->save(ul.users);
}

// ============================================================
//  UPDATE: Modify existing teacher
// ============================================================

bool TeacherService::updateTeacher(UserId id,
                                   const std::string &fullName,
                                   const std::string &subject,
                                   const std::string &assignedClass)
{
    UserList ul;
    ul.users = userRepo->load();

    bool found = false;
    for (std::size_t i = 0; i < ul.users.size(); ++i)
    {
        User *u = ul.users[i];
        if (!u || u->getId() != id || u->getRole() != "Teacher")
            continue;

        Teacher *t = static_cast<Teacher *>(u);
        // Create updated teacher with same id/username/password
        Teacher *updated = new Teacher(
            t->getId(),
            t->getUsername(),
            t->getPassword(),
            fullName,
            subject,
            assignedClass);

        delete ul.users[i];
        ul.users[i] = updated;
        found = true;
        break;
    }

    return found && userRepo->save(ul.users);
}

// ============================================================
//  DELETE: Remove teacher
// ============================================================

bool TeacherService::removeTeacher(UserId id)
{
    UserList ul;
    ul.users = userRepo->load();

    auto it = std::find_if(ul.users.begin(), ul.users.end(),
                           [id](User *u)
                           { return u && u->getId() == id && u->getRole() == "Teacher"; });

    if (it == ul.users.end())
        return false;

    delete *it;
    ul.users.erase(it);
    return userRepo->save(ul.users);
}

// ============================================================
//  QUERY: Get teachers
// ============================================================

std::vector<Teacher *> TeacherService::getAllTeachers() const
{
    std::vector<Teacher *> result;
    for (auto *u : userRepo->load())
    {
        if (u && u->getRole() == "Teacher")
        {
            result.push_back(static_cast<Teacher *>(u));
        }
        else
        {
            delete u;
        }
    }
    return result;
}

Teacher *TeacherService::findTeacherById(UserId id) const
{
    for (auto *u : userRepo->load())
    {
        if (u && u->getId() == id && u->getRole() == "Teacher")
        {
            Teacher *t = static_cast<Teacher *>(u);
            // Delete other users
            for (auto *other : userRepo->load())
                if (other && other->getId() != id)
                    delete other;
            return t;
        }
        else
        {
            delete u;
        }
    }
    return nullptr;
}

// ============================================================
//  PASSWORD: Reset teacher password
// ============================================================

bool TeacherService::resetPassword(UserId id, const std::string &newHashedPassword)
{
    UserList ul;
    ul.users = userRepo->load();

    bool found = false;
    for (auto *u : ul.users)
    {
        if (u && u->getId() == id && u->getRole() == "Teacher")
        {
            u->setPassword(newHashedPassword);
            found = true;
            break;
        }
    }

    return found && userRepo->save(ul.users);
}
