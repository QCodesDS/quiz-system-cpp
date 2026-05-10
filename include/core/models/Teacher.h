#ifndef TEACHER_H
#define TEACHER_H

#include "User.h"

/**
 * @class Teacher
 * @brief Đại diện cho đối tượng Giáo viên trong hệ thống.
 *
 * Lớp này quản lý chuyên môn giảng dạy và thông tin lớp chủ nhiệm.
 * Một giáo viên có thể chưa được phân công lớp chủ nhiệm (assignedClass để trống).
 */
class Teacher : public User
{
private:
    std::string _subject;       ///< Môn học chuyên môn đảm nhận.
    std::string _assignedClass; ///< Lớp chủ nhiệm được phân công (rỗng nếu chưa có).

public:
    /**
     * @brief Khởi tạo một đối tượng Teacher.
     *
     * @param id Mã định danh duy nhất.
     * @param username Tên đăng nhập.
     * @param password Mật khẩu đã hash.
     * @param fullName Họ và tên đầy đủ.
     * @param subject Môn học giảng dạy.
     * @param assignedClass Tên lớp chủ nhiệm (mặc định là chuỗi rỗng).
     */
    Teacher(UserId id, const std::string &username,
            const std::string &password, const std::string &fullName,
            const std::string &subject, const std::string &assignedClass = "");

    // ------------------------------------------------------------
    //  Getters & Setters
    // ------------------------------------------------------------

    std::string getSubject() const;
    std::string getAssignedClass() const;

    /**
     * @brief Cập nhật hoặc phân công lớp chủ nhiệm mới.
     * @param cls Tên lớp học mới.
     */
    void setAssignedClass(const std::string &cls);

    // ------------------------------------------------------------
    //  Override từ User
    // ------------------------------------------------------------

    /**
     * @brief Trả về vai trò của đối tượng.
     * @return std::string — Luôn trả về "Teacher".
     */
    std::string getRole() const override;

    /**
     * @brief Chuyển đổi thông tin Teacher thành chuỗi định dạng lưu file.
     * @return std::string — Định dạng: "Teacher|id|user|pass|name|subject|assignedClass".
     */
    std::string toFileString() const override;

    /**
     * @brief Hiển thị thông tin giáo viên và trạng thái lớp chủ nhiệm.
     */
    void displayInfo() const override;
};

#endif // TEACHER_H