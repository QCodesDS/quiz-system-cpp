#ifndef AUTH_SERVICE_H
#define AUTH_SERVICE_H

#include "core/interface/IAuthService.h"
#include "core/interface/IUserRepository.h"

/**
 * @class AuthService
 * @brief Chịu trách nhiệm xác thực người dùng và duy trì trạng thái đăng nhập.
 *
 * Ưu điểm thiết kế:
 * - Sử dụng Smart Pointers để quản lý vòng đời User (RAII).
 * - Tách biệt hoàn toàn logic xác thực với tầng dữ liệu (Repository).
 * - Chỉ cung cấp Raw Pointer cho các tầng trên (UI/App) để xem dữ liệu,
 *   không cho phép can thiệp vào quyền sở hữu (ownership).
 */
class AuthService : public IAuthService
{
private:
    IUserRepository *_userRepo;
    std::unique_ptr<User> _currentUser; ///< Nullptr nếu chưa đăng nhập.

public:
    /**
     * @brief Mã hóa mật khẩu (Placeholder cho bcrypt/SHA-256).
     */
    static std::string hashPassword(const std::string &plain);

    explicit AuthService(IUserRepository *userRepo);

    // ------------------------------------------------------------
    //  Triển khai IAuthService
    // ------------------------------------------------------------

    User *login(const std::string &username, const std::string &password) override;
    void logout() override;
    bool changePassword(User *user, const std::string &newPass) override;
    User *getCurrentUser() override;

    bool validateCredentials(const std::string &username,
                             const std::string &password) override;
};

#endif // AUTH_SERVICE_H