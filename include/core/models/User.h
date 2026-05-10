#ifndef USER_H
#define USER_H

#include "typedefs.h"

/**
 * @class User
 * @brief Lớp cơ sở trừu tượng (Abstract Base Class) cho mọi người dùng trong hệ thống.
 *
 * Lớp này định nghĩa các thuộc tính và hành vi chung nhất của một tài khoản.
 * Các lớp dẫn xuất (Admin, Teacher, Student) bắt buộc phải thực thi các phương thức
 * thuần ảo để xác định vai trò và cách thức lưu trữ riêng biệt.
 */
class User
{
protected:
    UserId _id;            ///< Mã định danh duy nhất của người dùng.
    std::string _username; ///< Tên đăng nhập dùng để định danh khi xác thực.
    std::string _password; ///< Mật khẩu (khuyến nghị lưu trữ dưới dạng hash).
    std::string _fullName; ///< Họ và tên đầy đủ của người dùng.

public:
    /**
     * @brief Khởi tạo đối tượng User cơ bản.
     * @param id Mã định danh.
     * @param username Tên đăng nhập.
     * @param password Mật khẩu đã mã hóa.
     * @param fullName Họ và tên.
     */
    User(UserId id,
         const std::string &username,
         const std::string &password,
         const std::string &fullName);

    /**
     * @brief Virtual destructor.
     * Đảm bảo việc giải phóng vùng nhớ của các lớp con được thực thi đầy đủ
     * khi hủy đối tượng thông qua con trỏ lớp cơ sở.
     */
    virtual ~User() = default;

    // ------------------------------------------------------------
    //  Getters
    // ------------------------------------------------------------

    UserId getId() const;
    std::string getUsername() const;
    std::string getFullName() const;

    /**
     * @brief Lấy mật khẩu phục vụ mục đích so khớp logic.
     * @return std::string — Chuỗi password hash.
     */
    std::string getPassword() const;

    // ------------------------------------------------------------
    //  Setters & Business Logic
    // ------------------------------------------------------------

    /**
     * @brief Cập nhật mật khẩu mới với các điều kiện ràng buộc.
     * @param newPass Mật khẩu mới cần thiết lập.
     * @return true nếu mật khẩu hợp lệ (>= 6 ký tự), false nếu ngược lại.
     */
    bool setPassword(const std::string &newPass);

    // ------------------------------------------------------------
    //  Pure Virtual Functions (Phải override ở lớp con)
    // ------------------------------------------------------------

    /**
     * @brief Trả về định danh vai trò của người dùng.
     * @return std::string — "Admin", "Teacher", hoặc "Student".
     */
    virtual std::string getRole() const = 0;

    /**
     * @brief Chuyển đổi dữ liệu đối tượng sang định dạng chuỗi lưu file.
     */
    virtual std::string toFileString() const = 0;

    /**
     * @brief Hiển thị thông tin tổng quan của người dùng ra console.
     */
    virtual void displayInfo() const = 0;
};

#endif // USER_H