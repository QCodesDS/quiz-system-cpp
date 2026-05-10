#ifndef I_AUTH_SERVICE_H
#define I_AUTH_SERVICE_H

#include "User.h"

/**
 * @brief Interface xác thực người dùng và quản lý phiên đăng nhập.
 *
 * @details Định nghĩa contract cho toàn bộ logic xác thực:
 * đăng nhập, đăng xuất, đổi mật khẩu, và truy cập user hiện tại.
 *
 * @note Caller không sở hữu con trỏ trả về từ login() và
 * getCurrentUser() — AuthService giữ ownership qua unique_ptr.
 */
class IAuthService
{
public:
    virtual ~IAuthService() = default;

    /**
     * @brief Xác thực thông tin đăng nhập và bắt đầu phiên làm việc.
     *
     * @param username Tên đăng nhập.
     * @param password Mật khẩu dạng plain text (sẽ được hash nội bộ).
     * @return Con trỏ đến User đang đăng nhập, hoặc nullptr nếu sai thông tin.
     */
    virtual User *login(const std::string &username,
                        const std::string &password) = 0;

    /**
     * @brief Kết thúc phiên làm việc và giải phóng currentUser.
     */
    virtual void logout() = 0;

    /**
     * @brief Đổi mật khẩu cho user được chỉ định.
     *
     * @param user    Con trỏ đến user cần đổi mật khẩu (không được null).
     * @param newPass Mật khẩu mới dạng plain text.
     * @return true nếu đổi thành công, false nếu mật khẩu không hợp lệ
     *         hoặc lưu thất bại.
     */
    virtual bool changePassword(User *user, const std::string &newPass) = 0;

    /**
     * @brief Trả về user đang đăng nhập trong phiên hiện tại.
     *
     * @return Con trỏ đến User hiện tại, hoặc nullptr nếu chưa đăng nhập.
     */
    virtual User *getCurrentUser() = 0;

    /**
     * @brief Kiểm tra thông tin đăng nhập mà không tạo phiên làm việc.
     *
     * @details Dùng để xác nhận mật khẩu hiện tại trước khi
     * thực hiện thao tác nhạy cảm (vd: đổi mật khẩu).
     *
     * @param username Tên đăng nhập.
     * @param password Mật khẩu dạng plain text.
     * @return true nếu thông tin hợp lệ.
     */
    virtual bool validateCredentials(const std::string &username,
                                     const std::string &password) = 0;
};

#endif // I_AUTH_SERVICE_H