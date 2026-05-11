#include "app/DataSeeder.h"
#include "ServiceContainer.h"
#include "UserService.h"
#include "AuthService.h"
#include "enums.h"
#include <iostream>

void DataSeeder::seed(ServiceContainer &container)
{
    auto *userSvc = container.getUserService();
    auto *authSvc = container.getAuthService();

    // Try to login with test accounts
    // If login fails, it means accounts don't exist (or password is wrong)
    // For this test, we'll only seed if login fails

    // Test 1: Try admin account
    auto *testAdmin = authSvc->login("admin", "admin123");
    if (!testAdmin)
    {
        std::cout << "  • Seeding admin account...\n";
        // Can't directly add admin via UserService, so we'll add teachers/students instead
    }
    else
    {
        authSvc->logout();
    }

    // Test 2: Try teacher account
    auto *testTeacher = authSvc->login("teacher01", "teacher123");
    if (!testTeacher)
    {
        std::cout << "  • Seeding teacher01 account...\n";
        bool ok = userSvc->addTeacher(
            "teacher01",
            "teacher123",
            "Nguyen Van A",
            "Math",
            "10A");
        if (ok)
            std::cout << "    ✓ teacher01 created\n";
    }
    else
    {
        authSvc->logout();
    }

    // Test 3: Try student account
    auto *testStudent = authSvc->login("student01", "student123");
    if (!testStudent)
    {
        std::cout << "  • Seeding student01 account...\n";
        bool ok = userSvc->addStudent(
            "student01",
            "student123",
            "Tran Thi B",
            "10A",
            Gender::Female,
            16,
            "0912345678");
        if (ok)
            std::cout << "    ✓ student01 created\n";
    }
    else
    {
        authSvc->logout();
    }

    std::cout << "\n";
}
