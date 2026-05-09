#ifndef USER_H
#define USER_H

#include <string>
#include "typedefs.h"

/**
 * @file User.h
 * @brief Khai báo lớp cơ sở trừu tượng User.
 *
 * User là abstract base class đại diện cho tài khoản
 * trong hệ thống. Các lớp như Admin, Teacher, Student
 * sẽ kế thừa từ lớp này.
 *
 * Responsibilities:
 * - Quản lý thông tin cơ bản của người dùng
 * - Cung cấp interface chung cho các subclass
 * - Định nghĩa contract cho polymorphism
 */
class User
{
protected:
    /// Mã định danh của người dùng
    UserId _id;

    /// Username dùng cho xác thực đăng nhập
    std::string _username;

    /// Password đã được hash
    std::string _password;

    /// Họ tên đầy đủ
    std::string _fullName;

public:
    /**
     * @brief Khởi tạo đối tượng User.
     *
     * @param id         Mã định danh người dùng
     * @param username   Tên đăng nhập
     * @param password   Mật khẩu đã hash
     * @param fullName   Họ tên đầy đủ
     */
    User(UserId id,
         const std::string &username,
         const std::string &password,
         const std::string &fullName);

    /**
     * @brief Virtual destructor để hỗ trợ polymorphism an toàn.
     */
    virtual ~User() = default;

    // ============================================================
    //    Getters
    // ============================================================

    /**
     * @brief Lấy ID của người dùng.
     * @return UserId
     */
    UserId getId() const;

    /**
     * @brief Lấy username của người dùng.
     * @return std::string
     */
    std::string getUsername() const;

    /**
     * @brief Lấy họ tên đầy đủ của người dùng.
     * @return std::string
     */
    std::string getFullName() const;

    /**
     * @brief Trả về password hash phục vụ xác thực nội bộ.
     */
    std::string getPassword() const;

    // ============================================================
    //    Setters
    // ============================================================

    /**
     * @brief Cập nhật mật khẩu mới.
     *
     * Validation hiện tại:
     * - Độ dài tối thiểu: 6 ký tự
     *
     * @param newPass Mật khẩu mới
     * @return true  Nếu cập nhật thành công
     * @return false Nếu mật khẩu không hợp lệ
     */
    bool setPassword(const std::string &newPass);

    // ============================================================
    //    Pure Virtual Functions
    // ============================================================

    /**
     * @brief Trả về role của người dùng.
     *
     * Ví dụ:
     * - Admin
     * - Teacher
     * - Student
     */
    virtual std::string getRole() const = 0;

    /**
     * @brief Chuyển object thành chuỗi string để lưu file.
     */
    virtual std::string toFileString() const = 0;

    /**
     * @brief Hiển thị thông tin người dùng.
     */
    virtual void displayInfo() const = 0;
};

#endif // USER_H