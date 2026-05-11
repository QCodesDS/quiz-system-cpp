#include "ServiceContainer.h"
#include "LoginScreen.h"
#include "User.h"
#include "core/interface/IAuthService.h"
#include "ui/console/ConsoleUtils.h"
#include "ui/console/MenuDispatcher.h"
#include "app/DataSeeder.h"
#include <iostream>
#include <memory>

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
        DataSeeder::seed(container);

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
            bool loggedOut = MenuDispatcher::dispatchToMenu(&container, currentUser);

            // If user chose logout or exit
            if (loggedOut)
            {
                container.getAuthService()->logout();

                // Ask if want to login again or exit
                shouldExit = ConsoleUtils::askLoginAgainOrExit();
            }
        }

        ConsoleUtils::printExit();
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
