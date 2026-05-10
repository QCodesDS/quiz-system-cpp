#ifndef I_USERTYPE_HANDLER_H
#define I_USERTYPE_HANDLER_H

#include <string>
#include <vector>
#include <memory>

class User;

/**
 * @class IUserTypeHandler
 * @brief Giao diện (Interface) cho các bộ xử lý phân loại người dùng.
 *
 * Áp dụng Strategy/Handler Pattern để tách biệt logic xử lý lưu trữ (Serialization/Deserialization)
 * của từng loại User (Admin, Teacher, Student) ra khỏi lớp quản lý chung.
 *
 * @note Thiết kế này tuân thủ nguyên tắc Open/Closed: Khi thêm loại người dùng mới,
 *       chỉ cần tạo thêm một Handler mới mà không cần sửa đổi UserRepository.
 */
class IUserTypeHandler
{
public:
    /**
     * @brief Virtual destructor.
     * Đảm bảo việc giải phóng tài nguyên an toàn cho các lớp dẫn xuất.
     */
    virtual ~IUserTypeHandler() = default;

    /**
     * @brief Trả về vai trò mà Handler này quản lý.
     * @return std::string — "Admin", "Teacher" hoặc "Student".
     */
    virtual std::string getRole() const = 0;

    /**
     * @brief Trả về đường dẫn tệp tin tương ứng với loại người dùng này.
     * @return std::string — Đường dẫn tệp tin (ví dụ: "data/teachers.txt").
     */
    virtual std::string getFilePath() const = 0;

    /**
     * @brief Chuyển đổi danh sách đối tượng User thành định dạng chuỗi để lưu tệp.
     *
     * @param users Vector chứa các con trỏ unique_ptr<User> (tham chiếu hằng, không chuyển quyền sở hữu).
     * @return std::vector<std::string> — Danh sách các dòng văn bản đã được định dạng.
     */
    virtual std::vector<std::string> serialize(
        const std::vector<std::unique_ptr<User>> &users) = 0;

    /**
     * @brief Chuyển đổi các dòng văn bản từ tệp thành danh sách đối tượng User.
     *
     * @param lines Danh sách các dòng văn bản đọc được từ tệp.
     * @return std::vector<std::unique_ptr<User>> — Danh sách đối tượng (chuyển quyền sở hữu cho caller).
     */
    virtual std::vector<std::unique_ptr<User>> deserialize(
        const std::vector<std::string> &lines) = 0;
};

#endif // I_USERTYPE_HANDLER_H