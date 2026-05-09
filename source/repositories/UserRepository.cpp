#include "UserRepository.h"
#include <fstream>
#include <memory>

UserRepository::UserRepository(const std::string &adminFile,
                               const std::string &teacherFile,
                               const std::string &studentFile)
    : FileRepository(studentFile),
      adminFile(adminFile),
      teacherFile(teacherFile),
      studentFile(studentFile) {}

// ------------------------------------------------------------
//  Parse helpers
// ------------------------------------------------------------

Gender UserRepository::parseGender(const std::string &s)
{
    if (s == "Female")
        return Gender::Female;
    if (s == "Other")
        return Gender::Other;
    return Gender::Male;
}

std::unique_ptr<Admin> UserRepository::parseAdmin(const std::vector<std::string> &f) const
{
    if (f.size() < 5)
        return nullptr;
    try
    {
        return std::make_unique<Admin>(std::stoi(f[1]), f[2], f[3], f[4]);
    }
    catch (...)
    {
        return nullptr;
    }
}

std::unique_ptr<Teacher> UserRepository::parseTeacher(const std::vector<std::string> &f) const
{
    if (f.size() < 7)
        return nullptr;
    try
    {
        return std::make_unique<Teacher>(std::stoi(f[1]), f[2], f[3], f[4], f[5], f[6]);
    }
    catch (...)
    {
        return nullptr;
    }
}

std::unique_ptr<Student> UserRepository::parseStudent(const std::vector<std::string> &f) const
{
    if (f.size() < 9)
        return nullptr;
    try
    {
        return std::make_unique<Student>(std::stoi(f[1]), f[2], f[3], f[4],
                                         f[5], parseGender(f[6]), std::stoi(f[7]), f[8]);
    }
    catch (...)
    {
        return nullptr;
    }
}

// ------------------------------------------------------------
//  Load từng loại file
// ------------------------------------------------------------

std::vector<std::unique_ptr<User>> UserRepository::loadAdmins() const
{
    std::vector<std::unique_ptr<User>> result;
    for (const auto &line : readLinesFrom(adminFile))
    {
        auto f = splitLine(line);
        if (f.empty() || f[0] != "Admin")
            continue;
        auto a = parseAdmin(f);
        if (a)
            result.push_back(std::move(a));
    }
    return result;
}

std::vector<std::unique_ptr<User>> UserRepository::loadTeachers() const
{
    std::vector<std::unique_ptr<User>> result;
    for (const auto &line : readLinesFrom(teacherFile))
    {
        auto f = splitLine(line);
        if (f.empty() || f[0] != "Teacher")
            continue;
        auto t = parseTeacher(f);
        if (t)
            result.push_back(std::move(t));
    }
    return result;
}

std::vector<std::unique_ptr<User>> UserRepository::loadStudents() const
{
    std::vector<std::unique_ptr<User>> result;
    for (const auto &line : readLines())
    {
        auto f = splitLine(line);
        if (f.empty() || f[0] != "Student")
            continue;
        auto s = parseStudent(f);
        if (s)
            result.push_back(std::move(s));
    }
    return result;
}

// ------------------------------------------------------------
//  IUserRepository — public interface
// ------------------------------------------------------------

std::vector<std::unique_ptr<User>> UserRepository::load()
{
    std::vector<std::unique_ptr<User>> all;
    auto admins = loadAdmins();
    auto teachers = loadTeachers();
    auto students = loadStudents();
    all.insert(all.end(), std::make_move_iterator(admins.begin()), std::make_move_iterator(admins.end()));
    all.insert(all.end(), std::make_move_iterator(teachers.begin()), std::make_move_iterator(teachers.end()));
    all.insert(all.end(), std::make_move_iterator(students.begin()), std::make_move_iterator(students.end()));
    return all;
}

bool UserRepository::save(const std::vector<std::unique_ptr<User>> &users)
{
    std::vector<std::string> adminLines, teacherLines, studentLines;
    for (const auto &u : users)
    {
        if (!u)
            continue;
        std::string role = u->getRole();
        if (role == "Admin")
            adminLines.push_back(u->toFileString());
        else if (role == "Teacher")
            teacherLines.push_back(u->toFileString());
        else if (role == "Student")
            studentLines.push_back(u->toFileString());
    }
    bool ok = true;
    ok &= safeWriteTo(adminFile, adminLines);
    ok &= safeWriteTo(teacherFile, teacherLines);
    ok &= safeWrite(studentLines);
    return ok;
}

bool UserRepository::backup()
{
    bool ok = true;
    ok &= backupFileTo(adminFile);
    ok &= backupFileTo(teacherFile);
    ok &= backupFile();
    return ok;
}

std::unique_ptr<User> UserRepository::findById(UserId id)
{
    auto all = load();
    std::unique_ptr<User> found = nullptr;

    for (auto &u : all)
    {
        if (u && u->getId() == id)
        {
            // Move ownership of found user out of vector
            found = std::move(u);
            break;
        }
    }

    // Vector goes out of scope and cleans up remaining users
    return found;
}

std::unique_ptr<User> UserRepository::findByUsername(const std::string &username)
{
    auto all = load();
    std::unique_ptr<User> found = nullptr;

    for (auto &u : all)
    {
        if (u && u->getUsername() == username)
        {
            // Move ownership of found user out of vector
            found = std::move(u);
            break;
        }
    }

    // Vector goes out of scope and cleans up remaining users
    return found;
}

bool UserRepository::usernameExists(const std::string &username)
{
    auto all = load();
    for (const auto &u : all)
    {
        if (u && u->getUsername() == username)
            return true;
    }
    // Vector goes out of scope and cleans up all users
    return false;
}