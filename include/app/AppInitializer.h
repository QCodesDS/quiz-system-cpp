#ifndef APP_INITIALIZER_H
#define APP_INITIALIZER_H

#include <memory>

class ServiceContainer;

/**
 * @brief Điểm khởi động duy nhất của ứng dụng.
 *
 * Chịu trách nhiệm tạo ServiceContainer đã được khởi tạo
 * đầy đủ theo thứ tự: repositories → handlers → services.
 *
 * @note Không thể khởi tạo trực tiếp. Chỉ dùng qua
 *       initializeApp().
 *
 * @example
 *   auto container = AppInitializer::initializeApp();
 *   auto* auth = container->getAuthService();
 */
class AppInitializer
{
public:
    /**
     * @brief Khởi tạo toàn bộ dependency graph.
     * @return unique_ptr<ServiceContainer> — ownership về caller.
     * @throws std::runtime_error nếu khởi tạo thất bại.
     */
    static std::unique_ptr<ServiceContainer> initializeApp();

private:
    AppInitializer() = default; ///< Chỉ dùng static method.
};

#endif // APP_INITIALIZER_H