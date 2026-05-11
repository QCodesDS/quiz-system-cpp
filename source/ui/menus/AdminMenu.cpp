#include "AdminMenu.h"
#include "ServiceContainer.h"
#include "User.h"
#include <iostream>
#include <cstdlib>

bool AdminMenu::show(ServiceContainer *container, User *currentUser)
{
    if (!container || !currentUser)
    {
        std::cerr << "ERROR: Container or User is null\n";
        return false;
    }

    // Xóa màn hình
    system("cls");

    // In stub message
    std::cout << "\n╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║                     ADMIN MENU                          ║\n";
    std::cout << "║                                                          ║\n";
    std::cout << "║  [COMING SOON]                                          ║\n";
    std::cout << "║  - Quản lý người dùng                                   ║\n";
    std::cout << "║  - Quản lý đề thi                                       ║\n";
    std::cout << "║  - Xem báo cáo thống kê                                 ║\n";
    std::cout << "║  - Cài đặt hệ thống                                     ║\n";
    std::cout << "║                                                          ║\n";
    std::cout << "║  Chào mừng: " << currentUser->getFullName() << "\n";
    std::cout << "║                                                          ║\n";
    std::cout << "║  [L] Đăng xuất  [E] Thoát                               ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n\n";

    std::cout << "Lựa chọn của bạn: ";
    char choice;
    std::cin >> choice;
    std::cin.ignore(); // Clear input buffer

    if (choice == 'L' || choice == 'l')
    {
        return true; // Logout
    }
    else if (choice == 'E' || choice == 'e')
    {
        return true; // Exit (will be handled by main)
    }

    return false; // Stay in menu
}
