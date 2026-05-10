/**
 * @file AppInitializer.cpp
 * @brief Triển khai bootstrap ứng dụng.
 */

#include "app/AppInitializer.h"
#include "app/ServiceContainer.h"

std::unique_ptr<ServiceContainer> AppInitializer::initializeApp()
{
    // Tạo container rồi wire toàn bộ dependency graph.
    // Ownership được chuyển về caller (main.cpp) qua unique_ptr.
    auto container = std::make_unique<ServiceContainer>();
    container->initialize();
    return container;
}