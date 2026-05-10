#ifndef STUDENT_H
#define STUDENT_H

#include "core/models/User.h"
#include "core/types/enums.h"

/**
 * @class Student
 * @brief Đại diện cho đối tượng Học sinh trong hệ thống.
 *
 * Lớp này mở rộng từ User bằng cách bổ sung các thông tin định danh cá nhân
 * và học thuật cụ thể như lớp học, giới tính, độ tuổi và số điện thoại.
 */
class Student : public User
{
private:
    std::string _className; ///< Tên lớp sinh hoạt (ví dụ: 21KHTN).
    Gender _gender;         ///< Giới tính (Male/Female/Other).
    int _age;               ///< Độ tuổi hiện tại.
    std::string _phone;     ///< Số điện thoại liên lạc.

public:
    /**
     * @brief Khởi tạo một đối tượng Student với đầy đủ thông tin.
     *
     * @param id Mã định danh duy nhất.
     * @param username Tên đăng nhập.
     * @param password Mật khẩu đã hash.
     * @param fullName Họ và tên đầy đủ.
     * @param className Tên lớp học.
     * @param gender Giới tính.
     * @param age Tuổi.
     * @param phone Số điện thoại.
     */
    Student(UserId id, const std::string &username,
            const std::string &password, const std::string &fullName,
            const std::string &className, Gender gender, int age,
            const std::string &phone);

    // ------------------------------------------------------------
    //  Getters
    // ------------------------------------------------------------

    std::string getClassName() const;
    Gender getGender() const;
    int getAge() const;
    std::string getPhone() const;

    // ------------------------------------------------------------
    //  Setters
    // ------------------------------------------------------------

    void setFullName(const std::string &name);
    void setClassName(const std::string &className);
    void setGender(Gender gender);
    void setAge(int age);
    void setPhone(const std::string &phone);

    // ------------------------------------------------------------
    //  Overrides
    // ------------------------------------------------------------

    /**
     * @brief Trả về vai trò của đối tượng.
     * @return std::string — Luôn trả về "Student".
     */
    std::string getRole() const override;

    /**
     * @brief Chuyển đổi thông tin Student thành chuỗi định dạng lưu file.
     * @return std::string — Định dạng: "Student|id|user|pass|name|class|gender|age|phone".
     */
    std::string toFileString() const override;

    /**
     * @brief Hiển thị thông tin học sinh theo định dạng bảng điều khiển.
     */
    void displayInfo() const override;
};

#endif // STUDENT_H