#include "AppInitializer.h"
#include "ServiceContainer.h"
#include <iostream>
#include <stdexcept>

// ============================================================
//  app/main.cpp
//
//  Application entry point - minimal logic here
//
//  RESPONSIBILITY: Only program entry and error handling
//  All business logic is in services
//  All initialization is in AppInitializer
// ============================================================

int main()
{
    try
    {
        // Initialize the entire application
        auto container = AppInitializer::initializeApp();

        // TODO: Pass container to UI layer (StudentUI, TeacherUI, AdminUI, etc.)
        // For now, just verify services are available
        if (!container || !container->getUserService())
        {
            std::cerr << "ERROR: Failed to initialize services\n";
            return 1;
        }

        // Application is ready - hand control to UI layer
        // UI layer will use container->getXxxService() to access services
        // Example:
        //   UserService *userSvc = container->getUserService();
        //   AuthService *authSvc = container->getAuthService();
        //   UI::showLoginScreen(authSvc);

        std::cout << "Quiz System initialized successfully\n";
        std::cout << "Services ready: ";
        std::cout << "[UserService] ";
        std::cout << "[AuthService] ";
        std::cout << "[ExamService] ";
        std::cout << "[QuizService] ";
        std::cout << "[ReportService]\n";

        // Placeholder: Application loop would go here
        // For now, just exit cleanly
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
