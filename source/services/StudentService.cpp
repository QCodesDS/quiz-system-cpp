#include "StudentService.h"
#include "IdGeneratorService.h"
#include <algorithm>

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

    // Load existing users
    UserList ul;
    ul.users = userRepo->load();

    // Generate new student ID
    UserId newId = 1000;
    for (const auto *u : ul.users)
    {
        if (u && u->getRole() == "Student" && u->getId() > newId)
            newId = u->getId();
    }
    newId++;

    // Create new student
    ul.users.push_back(
        new Student(newId, username, password, fullName, className, gender, age, phone));

    // Persist
    return userRepo->save(ul.users);
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

    UserList ul;
    ul.users = userRepo->load();

    bool found = false;
    for (std::size_t i = 0; i < ul.users.size(); ++i)
    {
        User *u = ul.users[i];
        if (!u || u->getId() != id || u->getRole() != "Student")
            continue;

        Student *s = static_cast<Student *>(u);
        // Create updated student with same id/username/password
        Student *updated = new Student(
            s->getId(),
            s->getUsername(),
            s->getPassword(),
            fullName,
            className,
            gender,
            age,
            phone);

        delete ul.users[i];
        ul.users[i] = updated;
        found = true;
        break;
    }

    return found && userRepo->save(ul.users);
}

// ============================================================
//  DELETE: Remove student
// ============================================================

bool StudentService::removeStudent(UserId id)
{
    UserList ul;
    ul.users = userRepo->load();

    auto it = std::find_if(ul.users.begin(), ul.users.end(),
                           [id](User *u)
                           { return u && u->getId() == id && u->getRole() == "Student"; });

    if (it == ul.users.end())
        return false;

    delete *it;
    ul.users.erase(it);
    return userRepo->save(ul.users);
}

// ============================================================
//  QUERY: Get students
// ============================================================

std::vector<Student *> StudentService::getAllStudents() const
{
    std::vector<Student *> result;
    for (auto *u : userRepo->load())
    {
        if (u && u->getRole() == "Student")
        {
            result.push_back(static_cast<Student *>(u));
        }
        else
        {
            delete u;
        }
    }
    return result;
}

std::vector<Student *> StudentService::getStudentsByClass(const std::string &className) const
{
    std::vector<Student *> result;
    for (auto *u : userRepo->load())
    {
        if (u && u->getRole() == "Student")
        {
            Student *s = static_cast<Student *>(u);
            if (s->getClassName() == className)
            {
                result.push_back(s);
            }
            else
            {
                delete s;
            }
        }
        else
        {
            delete u;
        }
    }
    return result;
}

Student *StudentService::findStudentById(UserId id) const
{
    auto all = userRepo->load();
    for (auto *u : all)
    {
        if (u && u->getId() == id && u->getRole() == "Student")
        {
            Student *s = static_cast<Student *>(u);
            // Delete other users
            for (auto *other : all)
                if (other && other->getId() != id)
                    delete other;
            return s;
        }
    }
    // Delete all if not found
    for (auto *u : all)
        delete u;
    return nullptr;
}

// ============================================================
//  PASSWORD: Reset student password
// ============================================================

bool StudentService::resetPassword(UserId id, const std::string &newHashedPassword)
{
    UserList ul;
    ul.users = userRepo->load();

    bool found = false;
    for (auto *u : ul.users)
    {
        if (u && u->getId() == id && u->getRole() == "Student")
        {
            u->setPassword(newHashedPassword);
            found = true;
            break;
        }
    }

    return found && userRepo->save(ul.users);
}
