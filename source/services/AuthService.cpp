#include "AuthService.h"
#include <functional> // std::hash

// ============================================================
//  services/AuthService.cpp
// ============================================================

AuthService::AuthService(IUserRepository *userRepo)
    : userRepo(userRepo), currentUser(nullptr) {}

// ------------------------------------------------------------
//  hashPassword
//  Dùng std::hash làm placeholder — đủ để project chạy.
//  TODO: thay bằng SHA-256 hoặc bcrypt khi deploy thật.
// ------------------------------------------------------------
std::string AuthService::hashPassword(const std::string &plain)
{
    std::size_t h = std::hash<std::string>{}(plain);
    return std::to_string(h);
}

// ------------------------------------------------------------
//  login
//  Trả nullptr nếu username không tồn tại hoặc sai password.
//  currentUser được set — gọi logout() để clear.
// ------------------------------------------------------------
User *AuthService::login(const std::string &username, const std::string &password)
{
    User *user = userRepo->findByUsername(username);
    if (!user)
        return nullptr;

    if (user->getPassword() != hashPassword(password))
    {
        delete user;
        return nullptr;
    }

    // Giải phóng session cũ nếu có
    delete currentUser;
    currentUser = user;
    return currentUser;
}

void AuthService::logout()
{
    delete currentUser;
    currentUser = nullptr;
}

// ------------------------------------------------------------
//  changePassword
//  Validate → update object → persist toàn bộ user list.
// ------------------------------------------------------------
bool AuthService::changePassword(User *user, const std::string &newPass)
{
    if (!user)
        return false;
    if (newPass.size() < 6)
        return false;

    // setPassword validate thêm ở model layer
    if (!user->setPassword(hashPassword(newPass)))
        return false;

    // Persist — load all, thay đổi đúng user, save lại
    auto all = userRepo->load();
    for (auto *u : all)
    {
        if (u && u->getId() == user->getId())
        {
            u->setPassword(user->getPassword());
            break;
        }
    }
    bool ok = userRepo->save(all);
    for (auto *u : all)
        delete u;
    return ok;
}

User *AuthService::getCurrentUser()
{
    return currentUser;
}

bool AuthService::validateCredentials(const std::string &username,
                                      const std::string &password)
{
    User *user = userRepo->findByUsername(username);
    if (!user)
        return false;
    bool valid = (user->getPassword() == hashPassword(password));
    delete user;
    return valid;
}