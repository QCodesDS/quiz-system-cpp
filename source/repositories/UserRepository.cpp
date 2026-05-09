#include "UserRepository.h"
#include <fstream>

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

Admin *UserRepository::parseAdmin(const std::vector<std::string> &f) const
{
    if (f.size() < 5)
        return nullptr;
    try
    {
        return new Admin(std::stoi(f[1]), f[2], f[3], f[4]);
    }
    catch (...)
    {
        return nullptr;
    }
}

Teacher *UserRepository::parseTeacher(const std::vector<std::string> &f) const
{
    if (f.size() < 7)
        return nullptr;
    try
    {
        return new Teacher(std::stoi(f[1]), f[2], f[3], f[4], f[5], f[6]);
    }
    catch (...)
    {
        return nullptr;
    }
}

Student *UserRepository::parseStudent(const std::vector<std::string> &f) const
{
    if (f.size() < 9)
        return nullptr;
    try
    {
        return new Student(std::stoi(f[1]), f[2], f[3], f[4],
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

std::vector<User *> UserRepository::loadAdmins() const
{
    std::vector<User *> result;
    for (const auto &line : readLinesFrom(adminFile))
    {
        auto f = splitLine(line);
        if (f.empty() || f[0] != "Admin")
            continue;
        Admin *a = parseAdmin(f);
        if (a)
            result.push_back(a);
    }
    return result;
}

std::vector<User *> UserRepository::loadTeachers() const
{
    std::vector<User *> result;
    for (const auto &line : readLinesFrom(teacherFile))
    {
        auto f = splitLine(line);
        if (f.empty() || f[0] != "Teacher")
            continue;
        Teacher *t = parseTeacher(f);
        if (t)
            result.push_back(t);
    }
    return result;
}

std::vector<User *> UserRepository::loadStudents() const
{
    std::vector<User *> result;
    for (const auto &line : readLines())
    {
        auto f = splitLine(line);
        if (f.empty() || f[0] != "Student")
            continue;
        Student *s = parseStudent(f);
        if (s)
            result.push_back(s);
    }
    return result;
}

// ------------------------------------------------------------
//  IUserRepository — public interface
// ------------------------------------------------------------

std::vector<User *> UserRepository::load()
{
    std::vector<User *> all;
    auto admins = loadAdmins();
    auto teachers = loadTeachers();
    auto students = loadStudents();
    all.insert(all.end(), admins.begin(), admins.end());
    all.insert(all.end(), teachers.begin(), teachers.end());
    all.insert(all.end(), students.begin(), students.end());
    return all;
}

bool UserRepository::save(const std::vector<User *> &users)
{
    std::vector<std::string> adminLines, teacherLines, studentLines;
    for (const auto *u : users)
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

User *UserRepository::findById(UserId id)
{
    auto all = load();
    User *found = nullptr;
    for (auto *u : all)
    {
        if (u && u->getId() == id)
        {
            found = u;
            break;
        }
    }
    for (auto *u : all)
    {
        if (u != found)
            delete u;
    }
    return found;
}

User *UserRepository::findByUsername(const std::string &username)
{
    auto all = load();
    User *found = nullptr;
    for (auto *u : all)
    {
        if (u && u->getUsername() == username)
        {
            found = u;
            break;
        }
    }
    for (auto *u : all)
    {
        if (u != found)
            delete u;
    }
    return found;
}

bool UserRepository::usernameExists(const std::string &username)
{
    auto all = load();
    bool found = false;
    for (auto *u : all)
    {
        if (u && u->getUsername() == username)
        {
            found = true;
            break;
        }
    }
    for (auto *u : all)
        delete u;
    return found;
}