#ifndef AUTH_SERVICE_H
#define AUTH_SERVICE_H

// ============================================================
//  services/AuthService.h
//
//  Quản lý xác thực và session hiện tại.
//  Không có cout/cin. Không biết data lưu ở đâu.
//
//  currentUser là raw pointer — AuthService KHÔNG sở hữu,
//  memory được quản lý bởi caller (AppInitializer).
// ============================================================

#include "IAuthService.h"
#include "IUserRepository.h"

class AuthService : public IAuthService
{
private:
    IUserRepository *userRepo;
    User *currentUser; // nullptr nếu chưa login

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