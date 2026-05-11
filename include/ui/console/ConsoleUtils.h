#ifndef CONSOLE_UTILS_H
#define CONSOLE_UTILS_H

#include <string>

/**
 * @namespace ConsoleUtils
 * @brief Console utilities for terminal UI operations.
 *
 * Provides common console operations like clearing screen,
 * displaying exit messages, and handling post-menu dialogs.
 */
namespace ConsoleUtils
{
    /**
     * @brief Clear the console screen (Windows: system("cls")).
     */
    void clearScreen();

    /**
     * @brief Display the exit message with purple/green colors.
     *
     * Shows a formatted box thanking the user for using the system.
     */
    void printExit();

    /**
     * @brief Ask user whether to login again or exit application.
     *
     * Displays a dialog with options:
     * - [L] Đăng nhập lại (login again)
     * - [E] Thoát ứng dụng (exit application)
     *
     * @return true if user chose [E] (exit), false if chose [L] (login again)
     */
    bool askLoginAgainOrExit();
}

#endif // CONSOLE_UTILS_H
