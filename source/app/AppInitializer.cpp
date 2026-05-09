#include "AppInitializer.h"
#include "ServiceContainer.h"

// ============================================================
//  app/AppInitializer.cpp
//
//  Application bootstrap code - initializes all services
// ============================================================

std::unique_ptr<ServiceContainer> AppInitializer::initializeApp()
{
    // Create service container
    auto container = std::make_unique<ServiceContainer>();

    // Initialize all services and repositories
    container->initialize();

    return container;
}
