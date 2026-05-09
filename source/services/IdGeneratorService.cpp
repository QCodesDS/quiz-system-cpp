#include "IdGeneratorService.h"
#include <algorithm>
#include <memory>

// ============================================================
//  services/IdGeneratorService.cpp
//
//  SINGLE RESPONSIBILITY: Generate sequential IDs
//  Extracted from UserService
// ============================================================

IdGeneratorService::IdGeneratorService(IUserRepository *userRepo)
    : userRepo(userRepo) {}

// ============================================================
//  Helper: Find max ID by role
// ============================================================

UserId IdGeneratorService::findMaxIdByRole(const std::string &role, UserId defaultMin) const
{
    auto all = userRepo->load();
    UserId maxId = defaultMin;

    for (const auto &u : all)
    {
        if (u && u->getRole() == role && u->getId() > maxId)
            maxId = u->getId();
    }

    // Vector auto-cleans up when it goes out of scope
    return maxId;
}

// ============================================================
//  Generate next Student ID
// ============================================================

UserId IdGeneratorService::nextStudentId() const
{
    UserId maxId = findMaxIdByRole("Student", 1000);
    return maxId + 1;
}

// ============================================================
//  Generate next Teacher ID
// ============================================================

UserId IdGeneratorService::nextTeacherId() const
{
    UserId maxId = findMaxIdByRole("Teacher", 2000);
    return maxId + 1;
}
