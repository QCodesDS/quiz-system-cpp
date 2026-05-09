#include "UserRepository.h"
#include "handlers/IUserTypeHandler.h"
#include <fstream>
#include <memory>

UserRepository::UserRepository(const std::string &dataDir,
                               std::vector<IUserTypeHandler *> handlers)
    : FileRepository(dataDir + "students.txt"), // Default file for base class
      handlers(handlers)
{
}

// ============================================================
//  IUserRepository — public interface
// ============================================================

// NO if/else on role strings — HANDLER PATTERN eliminates type checking
std::vector<std::unique_ptr<User>> UserRepository::load()
{
    std::vector<std::unique_ptr<User>> all;

    // Loop through handlers: each knows its file and how to deserialize
    for (auto *handler : handlers)
    {
        if (!handler)
            continue;

        // Read lines from handler's file
        std::vector<std::string> lines = readLinesFrom(handler->getFilePath());

        // Handler deserializes to User objects
        auto users = handler->deserialize(lines);

        // Merge into result
        all.insert(all.end(),
                   std::make_move_iterator(users.begin()),
                   std::make_move_iterator(users.end()));
    }

    return all;
}

// NO if/else on role strings — HANDLER PATTERN eliminates type checking
bool UserRepository::save(const std::vector<std::unique_ptr<User>> &users)
{
    bool ok = true;

    // Loop through handlers: each serializes its type and writes to its file
    for (auto *handler : handlers)
    {
        if (!handler)
            continue;

        // Handler serializes users of its type to strings
        auto lines = handler->serialize(users);

        // Write handler's type to its file
        bool result = safeWriteTo(handler->getFilePath(), lines);
        ok &= result;
    }

    return ok;
}

bool UserRepository::backup()
{
    bool ok = true;

    // Backup each handler's file
    for (auto *handler : handlers)
    {
        if (!handler)
            continue;
        ok &= backupFileTo(handler->getFilePath());
    }

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