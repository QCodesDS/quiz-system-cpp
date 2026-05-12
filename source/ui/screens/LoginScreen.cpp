#include "LoginScreen.h"
#include "AuthService.h"
#include "User.h"
#include "constants.h"
#include <iostream>
#include <cstdlib>
#include <conio.h> // _getch() for Windows

// ============================================================
//  LoginScreen Implementation
// ============================================================

void LoginScreen::clearScreen()
{
    system("cls");
}

void LoginScreen::printBanner()
{
    auto row = [](const std::string &content)
    {
        std::cout << Color::PURPLE << "║" << Color::RESET << content << Color::PURPLE << "║" << Color::RESET << "\n";
    };

    std::cout << "\n";
    std::cout << Color::PURPLE << "╔══════════════════════════════════════════════════════════════╗" << Color::RESET << "\n";
    row("                                                              ");
    row("           ★  " + std::string(Color::BOLD) + "HỆ THỐNG QUẢN LÝ ĐỀ THI TRỰC TUYẾN" + Color::RESET + "  ★           ");
    row("                                                              ");
    std::cout << Color::PURPLE << "╠══════════════════════════════════════════════════════════════╣" << Color::RESET << "\n";
    row("                                                              ");
    row("                  " + std::string(Color::GREEN) + "▶  ĐĂNG NHẬP HỆ THỐNG" + Color::RESET + "                       ");
    row("                                                              ");
    std::cout << Color::PURPLE << "╚══════════════════════════════════════════════════════════════╝" << Color::RESET << "\n\n";
}

std::string LoginScreen::getHiddenPassword()
{
    std::string password;
    char ch;
    while ((ch = _getch()) != '\r') // Enter key
    {
        if (ch == '\b') // Backspace
        {
            if (!password.empty())
            {
                password.pop_back();
                std::cout << "\b \b"; // Erase from console
            }
        }
        else
        {
            password += ch;
            std::cout << '*'; // Display asterisk instead of actual character
        }
    }
    std::cout << "\n";
    return password;
}

void LoginScreen::printLoginError(int attemptsLeft)
{
    std::cout << "✗ Tên đăng nhập hoặc mật khẩu không chính xác!\n";
    std::cout << "  Số lần thử còn lại: " << attemptsLeft << "\n\n";
}

void LoginScreen::printTooManyAttempts()
{
    std::cout << "\n✗ Quá nhiều lần đăng nhập thất bại.\n";
    std::cout << "  Tài khoản bị khóa tạm thời. Vui lòng thử lại sau.\n\n";
}

void LoginScreen::printLoginSuccess(const std::string &fullName)
{
    std::cout << "✓ Đăng nhập thành công! Chào mừng " << fullName << ".\n\n";
}

User *LoginScreen::show(IAuthService *authService)
{
    if (!authService)
    {
        std::cerr << "ERROR: AuthService is null\n";
        return nullptr;
    }

    clearScreen();
    printBanner();

    int failCount = 0;
    const int MAX_ATTEMPTS = 3;

    while (failCount < MAX_ATTEMPTS)
    {
        std::string username, password;

        // Nhập username
        std::cout << "Tên đăng nhập: ";
        std::getline(std::cin, username);

        if (username.empty())
        {
            std::cout << "✗ Vui lòng nhập tên đăng nhập.\n\n";
            continue;
        }

        // Nhập password
        std::cout << "Mật khẩu: ";
        password = getHiddenPassword();

        if (password.empty())
        {
            std::cout << "✗ Vui lòng nhập mật khẩu.\n\n";
            continue;
        }

        // Xác thực
        User *user = authService->login(username, password);

        if (user)
        {
            // Đăng nhập thành công
            printLoginSuccess(user->getFullName());
            return user;
        }
        else
        {
            // Đăng nhập thất bại
            failCount++;
            int attemptsLeft = MAX_ATTEMPTS - failCount;

            if (attemptsLeft <= 0)
            {
                clearScreen();
                printBanner();
                printTooManyAttempts();
                return nullptr;
            }

            printLoginError(attemptsLeft);
        }
    }

    return nullptr;
}
