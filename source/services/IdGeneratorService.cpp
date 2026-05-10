#include "services/IdGeneratorService.h"
#include <algorithm>

IdGeneratorService::IdGeneratorService(IUserRepository *userRepo)
    : _userRepo(userRepo)
{
}

// ------------------------------------------------------------
// Helper: Tìm ID lớn nhất để đảm bảo tính tuần tự
// ------------------------------------------------------------
UserId IdGeneratorService::findMaxIdByRole(const std::string &role, UserId defaultMin) const
{
    // load() trả về vector<unique_ptr<User>>
    auto allUsers = _userRepo->load();
    UserId maxId = defaultMin;

    for (const auto &u : allUsers)
    {
        // Kiểm tra tính hợp lệ và đúng vai trò trước khi so sánh ID
        if (u && u->getRole() == role)
        {
            if (u->getId() > maxId)
            {
                maxId = u->getId();
            }
        }
    }

    return maxId;
}

// ------------------------------------------------------------
// Public API
// ------------------------------------------------------------

UserId IdGeneratorService::nextStudentId() const
{
    // Ví dụ: Nếu max hiện tại là 1005 -> trả về 1006
    return findMaxIdByRole("Student", 1000) + 1;
}

UserId IdGeneratorService::nextTeacherId() const
{
    // Ví dụ: Nếu max hiện tại là 2000 -> trả về 2001
    return findMaxIdByRole("Teacher", 2000) + 1;
}