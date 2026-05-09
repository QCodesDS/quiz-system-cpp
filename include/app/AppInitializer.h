#ifndef APP_INITIALIZER_H
#define APP_INITIALIZER_H

#include <memory>

class ServiceContainer;

// ============================================================
//  app/AppInitializer.h
//
//  SINGLE RESPONSIBILITY: Initialize the application
//  Reason to change: Only if initialization process changes
//
//  Handles:
//  - Creating the ServiceContainer
//  - Initializing all services and repositories
//  - Returning container for UI layer to access services
//
//  Usage:
//    auto container = AppInitializer::initializeApp();
//    UserService *userSvc = container->getUserService();
// ============================================================

class AppInitializer
{
public:
    // Initialize the entire application and return service container
    // Returns: unique_ptr to ServiceContainer (caller owns it)
    // Throws: std::exception if initialization fails
    static std::unique_ptr<ServiceContainer> initializeApp();

private:
    AppInitializer() = default;
};

#endif // APP_INITIALIZER_H
