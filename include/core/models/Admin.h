#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"

/**
 * @class Admin
 * @brief Đại diện cho người dùng có quyền quản trị cao nhất trong hệ thống.
 *
 * Lớp Admin kế thừa từ User, thực hiện các phương thức thuần ảo để
 * định nghĩa vai trò và định dạng lưu trữ riêng cho cấp quản trị.
 * Admin không giữ thêm dữ liệu thành viên so với User cơ bản.
 */
class Admin : public User
{
public:
    /**
     * @brief Khởi tạo một đối tượng Admin mới.
     *
     * @param id Mã định danh duy nhất (UserId).
     * @param username Tên đăng nhập.
     * @param password Mật khẩu đã mã hóa.
     * @param fullName Họ và tên đầy đủ.
     */
    Admin(UserId id, const std::string &username,
          const std::string &password, const std::string &fullName);

    /**
     * @brief Trả về vai trò của đối tượng.
     * @return std::string — Luôn trả về "Admin".
     */
    std::string getRole() const override;

    /**
     * @brief Chuyển đổi thông tin đối tượng thành chuỗi định dạng lưu file.
     * @return std::string — Chuỗi theo định dạng "Admin|id|user|pass|name".
     */
    std::string toFileString() const override;

    /**
     * @brief Hiển thị thông tin chi tiết của Admin ra console.
     */
    void displayInfo() const override;
};

#endif // ADMIN_H