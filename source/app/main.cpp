#include "ServiceContainer.h"
#include "UserService.h"
#include "AuthService.h"
#include "enums.h"
#include <iostream>
#include <iostream>

int main()
{
    ServiceContainer container;
    container.initialize();

    // Test 1: Services khởi tạo được
    std::cout << "[OK] ServiceContainer initialized\n";

    // Test 2: Thêm teacher thử
    auto *userSvc = container.getUserService();
    bool ok = userSvc->addTeacher(
        "teacher01", "pass123", "Nguyen Van A", "Math", "10A");
    std::cout << (ok ? "[OK]" : "[FAIL]") << " addTeacher\n";

    // Test 3: Thêm student thử
    bool ok2 = userSvc->addStudent(
        "student01", "pass123", "Tran Thi B",
        "10A", Gender::Female, 16, "0912345678");
    std::cout << (ok2 ? "[OK]" : "[FAIL]") << " addStudent\n";

    // Test 4: Login thử
    auto *authSvc = container.getAuthService();
    auto *user = authSvc->login("teacher01", "pass123");
    std::cout << (user ? "[OK]" : "[FAIL]") << " login\n";

    return 0;
}