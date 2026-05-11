#include "StudentMenu.h"
#include "ServiceContainer.h"
#include "User.h"
#include <iostream>
#include <cstdlib>

bool StudentMenu::show(ServiceContainer *container, User *currentUser)
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
    std::cout << "║                   STUDENT MENU                          ║\n";
    std::cout << "║                                                          ║\n";
    std::cout << "║  [COMING SOON]                                          ║\n";
    std::cout << "║  - Xem danh sách đề thi có sẵn                          ║\n";
    std::cout << "║  - Làm bài thi                                          ║\n";
    std::cout << "║  - Xem kết quả thi                                      ║\n";
    std::cout << "║  - Luyện tập                                            ║\n";
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
