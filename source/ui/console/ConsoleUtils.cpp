#include "ui/console/ConsoleUtils.h"
#include "app/constants.h"
#include <iostream>
#include <cstdlib>

namespace ConsoleUtils
{
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

    bool askLoginAgainOrExit()
    {
        clearScreen();
        std::cout << "\n═══════════════════════════════════════════════════════════\n";
        std::cout << "  [L] Đăng nhập lại\n";
        std::cout << "  [E] Thoát ứng dụng\n";
        std::cout << "═══════════════════════════════════════════════════════════\n\n";

        std::cout << "Lựa chọn của bạn: ";
        char choice;
        std::cin >> choice;
        std::cin.ignore();

        return (choice == 'E' || choice == 'e');
    }
}
