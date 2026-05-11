#include "ServiceContainer.h"
#include "UserService.h"
#include "AuthService.h"
#include "LoginScreen.h"
#include "AdminMenu.h"
#include "TeacherMenu.h"
#include "StudentMenu.h"
#include "User.h"
#include "enums.h"
#include <iostream>
#include <cstdlib>
#include "constants.h"
#include <memory>
#include <fstream>

// ============================================================
//  HELPERS
// ============================================================

void clearScreen()
{
    system("cls");
}

void printExit()
{
    auto row = [](const std::string &content)
    {
        std::cout << PURPLE << "║" << RESET << content << PURPLE << "║" << RESET << "\n";
    };

    clearScreen();
    std::cout << "\n";
    std::cout << PURPLE << "╔════════════════════════════════════════════════════════════╗" << RESET << "\n";
    row("                                                            ");
    row("                " + std::string(GREEN) + "✔  Cảm ơn bạn đã sử dụng!" + RESET + "                   ");
    row("             " + std::string(BOLD) + "Hệ thống quản lý đề thi trực tuyến" + RESET + "             ");
    row("                                                            ");
    std::cout << PURPLE << "╚════════════════════════════════════════════════════════════╝" << RESET << "\n\n";
}

// ============================================================
//  SEED TEST DATA
// ============================================================

/**
 * @brief Seed test users if they don't already exist.
 *
 * Test credentials:
 * - Admin: admin / admin123
 * - Teacher: teacher01 / teacher123
 * - Student: student01 / student123
 */
void seedTestData(ServiceContainer &container)
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
// ============================================================
//  MAIN APPLICATION LOOP
// ============================================================

int main()
{
    try
    {
        // Initialize ServiceContainer
        ServiceContainer container;
        container.initialize();

        std::cout << "✓ Hệ thống khởi tạo thành công.\n";

        // Seed test data if needed
        std::cout << "• Kiểm tra dữ liệu test...\n";
        seedTestData(container);

        // Main loop: login until user wants to exit
        bool shouldExit = false;

        while (!shouldExit)
        {
            // Show login screen
            User *currentUser = LoginScreen::show(container.getAuthService());

            if (!currentUser)
            {
                // Login failed or too many attempts
                std::cout << "Vui lòng thoát ứng dụng và thử lại.\n";
                break;
            }

            // Dispatch to appropriate menu based on role
            std::string role = currentUser->getRole();
            bool loggedOut = false;

            if (role == "Admin")
            {
                loggedOut = AdminMenu::show(&container, currentUser);
            }
            else if (role == "Teacher")
            {
                loggedOut = TeacherMenu::show(&container, currentUser);
            }
            else if (role == "Student")
            {
                loggedOut = StudentMenu::show(&container, currentUser);
            }
            else
            {
                std::cerr << "ERROR: Unknown role: " << role << "\n";
                break;
            }

            // If user chose logout or exit
            if (loggedOut)
            {
                container.getAuthService()->logout();

                // Ask if want to login again or exit
                clearScreen();
                std::cout << "\n═══════════════════════════════════════════════════════════\n";
                std::cout << "  [L] Đăng nhập lại\n";
                std::cout << "  [E] Thoát ứng dụng\n";
                std::cout << "═══════════════════════════════════════════════════════════\n\n";

                std::cout << "Lựa chọn của bạn: ";
                char choice;
                std::cin >> choice;
                std::cin.ignore();

                if (choice == 'E' || choice == 'e')
                {
                    shouldExit = true;
                }
            }
        }

        printExit();
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "ERROR: " << e.what() << "\n";
        return 1;
    }
    catch (...)
    {
        std::cerr << "ERROR: Unknown exception occurred\n";
        return 1;
    }
}
