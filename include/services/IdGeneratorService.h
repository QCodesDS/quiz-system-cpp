#ifndef ID_GENERATOR_SERVICE_H
#define ID_GENERATOR_SERVICE_H

#include "IUserRepository.h"
#include "typedefs.h"

// ============================================================
//  services/IdGeneratorService.h
//
//  SINGLE RESPONSIBILITY: Generate sequential user IDs
//  Reason to change: Only if ID generation strategy changes
//
//  Extracted from UserService (SRP violation fix)
//  - nextStudentId: generates IDs >= 1000
//  - nextTeacherId: generates IDs >= 2000
// ============================================================

class IdGeneratorService
{
private:
    IUserRepository *userRepo;

    // Helper: find max ID matching a role
    UserId findMaxIdByRole(const std::string &role, UserId defaultMin) const;

public:
    explicit IdGeneratorService(IUserRepository *userRepo);

    // Generate next Student ID (1001, 1002, ...)
    UserId nextStudentId() const;

    // Generate next Teacher ID (2001, 2002, ...)
    UserId nextTeacherId() const;
};

#endif // ID_GENERATOR_SERVICE_H
