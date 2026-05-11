#ifndef LOGIN_SCREEN_H
#define LOGIN_SCREEN_H

#include <string>
#include <memory>

// Forward declaration
class IAuthService;
class ServiceContainer;

/**
 * @class LoginScreen
 * @brief Màn hình đăng nhập với xác thực 3 lần.
 *
 * Chức năng:
 * - Nhập username/password (ẩn password)
 * - Xác thực qua AuthService
 * - Tối đa 3 lần sai → thoát
 * - Đúng → trả về User* và role
 *
 * @note Màu sắc và clear screen là Windows-specific
 */
class LoginScreen
{
public:
    /**
     * @brief Hiển thị màn hình đăng nhập và xử lý xác thực.
     *
     * @param authService Dịch vụ xác thực từ ServiceContainer
     * @return Con trỏ User nếu đăng nhập thành công, nullptr nếu người dùng
     *         chọn thoát hoặc vượt quá 3 lần sai.
     *
     * @details
     * - In banner ASCII art
     * - Vòng lặp: nhập username → nhập password → xác thực
     * - Nếu sai: cộng fail counter, cho thử lại
     * - Nếu fail >= 3: in "Too many failed attempts" → return nullptr
     * - Nếu OK: return user pointer
     */
    static class User *show(IAuthService *authService);

private:
    /**
     * @brief In banner chào mừng ASCII art.
     */
    static void printBanner();

    /**
     * @brief Xóa màn hình (Windows specific: system("cls")).
     */
    static void clearScreen();

    /**
     * @brief Nhập password với ẩn ký tự (Windows: _getch()).
     *
     * @return Chuỗi password nhập vào
     */
    static std::string getHiddenPassword();

    /**
     * @brief In thông báo lỗi đăng nhập.
     *
     * @param attemptsLeft Số lần thử còn lại
     */
    static void printLoginError(int attemptsLeft);

    /**
     * @brief In thông báo quá nhiều lần sai.
     */
    static void printTooManyAttempts();

    /**
     * @brief In thông báo đăng nhập thành công.
     *
     * @param fullName Tên người dùng để hiển thị
     */
    static void printLoginSuccess(const std::string &fullName);
};

#endif // LOGIN_SCREEN_H
