#ifndef USER_REPOSITORY_H
#define USER_REPOSITORY_H

// ============================================================
//  repositories/UserRepository.h
//
//  Load từ 3 file riêng biệt (admin, teachers, students)
//  nhưng expose chung qua interface IUserRepository.
//
//  HANDLER PATTERN:
//  - Each handler knows its role, file path, and serialization logic
//  - UserRepository loops through handlers (NO if/else on role strings)
//  - Adding new user type = add new handler ONLY (OPEN/CLOSED)
//
//  SMART POINTER: All User* allocated with new → returned as unique_ptr
//  Ownership transferred to caller via unique_ptr
//  No manual delete in caller code
// ============================================================

#include "FileRepository.h"
#include "IUserRepository.h"
#include "handlers/IUserTypeHandler.h"
#include <memory>
#include <vector>

class UserRepository : public FileRepository, public IUserRepository
{
private:
    // Handler pattern: Each handler manages serialization of one user type
    // Eliminates if/else on role strings (OPEN/CLOSED PRINCIPLE)
    std::vector<IUserTypeHandler *> handlers;

public:
    // Constructor: Inject handlers for dependency injection
    // Handlers are owned elsewhere (typically ServiceContainer);
    // UserRepository only borrows raw pointers
    UserRepository(const std::string &dataDir,
                   std::vector<IUserTypeHandler *> handlers);

    // --- IUserRepository ---
    bool save(const std::vector<std::unique_ptr<User>> &users) override;
    std::vector<std::unique_ptr<User>> load() override;
    bool backup() override;

    std::unique_ptr<User> findById(UserId id) override;
    std::unique_ptr<User> findByUsername(const std::string &username) override;
    bool usernameExists(const std::string &username) override;
};

#endif // USER_REPOSITORY_H