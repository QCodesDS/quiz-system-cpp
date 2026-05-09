#ifndef AUTH_SERVICE_H
#define AUTH_SERVICE_H

// ============================================================
//  services/AuthService.h
//
//  Quản lý xác thực và session hiện tại.
//  Không có cout/cin. Không biết data lưu ở đâu.
//
//  SMART POINTER: currentUser is unique_ptr<User>
//  Owns the current user session. Uses .reset() for logout,
//  .move() for login. No manual delete anywhere.
// ============================================================

#include "IAuthService.h"
#include "IUserRepository.h"
#include <memory>

class AuthService : public IAuthService
{
private:
    IUserRepository *userRepo;
    std::unique_ptr<User> currentUser; // nullptr if not logged in

    // Hash đơn giản để minh hoạ — thay bằng bcrypt/SHA ở production
    static std::string hashPassword(const std::string &plain);

public:
    explicit AuthService(IUserRepository *userRepo);

    // --- IAuthService ---
    User *login(const std::string &username, const std::string &password) override;
    void logout() override;
    bool changePassword(User *user, const std::string &newPass) override;
    User *getCurrentUser() override;
    bool validateCredentials(const std::string &username,
                             const std::string &password) override;
};

#endif // AUTH_SERVICE_H