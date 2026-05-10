#include "services/AuthService.h"
#include <functional>

AuthService::AuthService(IUserRepository *userRepo)
    : _userRepo(userRepo), _currentUser(nullptr)
{
}

// ------------------------------------------------------------
//  Password Hashing
//  Hiện tại dùng std::hash để demo, trong thực tế nên dùng thư viện chuyên dụng.
// ------------------------------------------------------------
std::string AuthService::hashPassword(const std::string &plain)
{
    std::size_t h = std::hash<std::string>{}(plain);
    return std::to_string(h);
}

// ------------------------------------------------------------
//  Triển khai IAuthService
// ------------------------------------------------------------

User *AuthService::login(const std::string &username, const std::string &password)
{
    // findByUsername trả về std::unique_ptr<User>
    auto user = _userRepo->findByUsername(username);

    if (!user)
        return nullptr;

    // Kiểm tra mật khẩu đã băm
    if (user->getPassword() != hashPassword(password))
    {
        // Khi 'user' (unique_ptr) ra khỏi phạm vi này, nó tự động bị hủy.
        return nullptr;
    }

    // Chuyển quyền sở hữu từ local variable sang session (Service)
    _currentUser = std::move(user);

    return _currentUser.get(); // Trả về raw pointer để UI sử dụng (View-only)
}

void AuthService::logout()
{
    // Giải phóng bộ nhớ và kết thúc phiên làm việc
    _currentUser.reset();
}

bool AuthService::changePassword(User *user, const std::string &newPass)
{
    if (!user || newPass.size() < 6)
        return false;

    // Cập nhật mật khẩu mới (đã hash) cho đối tượng
    user->setPassword(hashPassword(newPass));

    // Đồng bộ hóa với kho lưu trữ (Persistence)
    auto allUsers = _userRepo->load(); // std::vector<std::unique_ptr<User>>

    bool found = false;
    for (auto &u : allUsers)
    {
        if (u && u->getId() == user->getId())
        {
            u->setPassword(user->getPassword());
            found = true;
            break;
        }
    }

    if (!found)
        return false;

    // Ghi toàn bộ danh sách đã cập nhật xuống tệp thông qua Repository
    return _userRepo->save(allUsers);
}

User *AuthService::getCurrentUser()
{
    return _currentUser.get();
}

bool AuthService::validateCredentials(const std::string &username,
                                      const std::string &password)
{
    auto user = _userRepo->findByUsername(username);
    if (!user)
        return false;

    return (user->getPassword() == hashPassword(password));
}