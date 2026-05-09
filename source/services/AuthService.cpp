#include "AuthService.h"
#include <functional> // std::hash
#include <memory>

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
//  Returns raw pointer to currentUser (AuthService owns via unique_ptr)
// ------------------------------------------------------------
User *AuthService::login(const std::string &username, const std::string &password)
{
    auto user = userRepo->findByUsername(username);
    if (!user)
        return nullptr;

    if (user->getPassword() != hashPassword(password))
        return nullptr; // user unique_ptr auto-cleans up on return

    // Release old session and store new user
    currentUser = std::move(user);
    return currentUser.get();
}

void AuthService::logout()
{
    currentUser.reset(); // Use reset() instead of delete
}

// ------------------------------------------------------------
//  changePassword
//  Load all users, update target user, save all back
//  Works with unique_ptr vector from repository
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
    auto all = userRepo->load(); // Returns vector<unique_ptr<User>>
    for (auto &u : all)
    {
        if (u && u->getId() == user->getId())
        {
            u->setPassword(user->getPassword());
            break;
        }
    }
    bool ok = userRepo->save(all);
    // Vector auto-cleans up when it goes out of scope
    return ok;
}

User *AuthService::getCurrentUser()
{
    return currentUser.get(); // Return raw pointer (non-owning)
}

bool AuthService::validateCredentials(const std::string &username,
                                      const std::string &password)
{
    auto user = userRepo->findByUsername(username);
    if (!user)
        return false;
    bool valid = (user->getPassword() == hashPassword(password));
    // user unique_ptr auto-cleans up
    return valid;
}