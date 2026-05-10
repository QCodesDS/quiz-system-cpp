#ifndef I_USER_REPOSITORY_H
#define I_USER_REPOSITORY_H

#include <memory>
#include "User.h"

/**
 * @brief Interface lưu trữ và truy vấn dữ liệu người dùng.
 *
 * @details Định nghĩa contract cho mọi implementation lưu trữ User
 * (Admin, Teacher, Student) — file-based, database, in-memory...
 *
 * Quy ước ownership:
 * - load(), findById(), findByUsername() → chuyển ownership về caller
 *   qua unique_ptr.
 * - save() → mượn const reference, không lấy ownership.
 * - Không có manual delete ở bất kỳ đâu trong codebase.
 *
 * @note UserRepository (repositories/) là implementation mặc định,
 * dùng Handler Pattern để loại bỏ if/else trên role string.
 */
class IUserRepository
{
public:
    virtual ~IUserRepository() = default;

    /**
     * @brief Ghi toàn bộ danh sách user ra storage.
     *
     * @details Rewrite toàn bộ file — dùng safe write (ghi file tạm
     * rồi rename) để tránh mất dữ liệu nếu crash giữa chừng.
     *
     * @param users Danh sách user cần lưu (mượn, không lấy ownership).
     * @return true nếu ghi thành công.
     */
    virtual bool save(const std::vector<std::unique_ptr<User>> &users) = 0;

    /**
     * @brief Tải toàn bộ user từ storage.
     *
     * @return Danh sách unique_ptr<User> — caller nhận ownership.
     *         Trả về vector rỗng nếu file không tồn tại hoặc rỗng.
     */
    virtual std::vector<std::unique_ptr<User>> load() = 0;

    /**
     * @brief Tạo bản sao lưu của file dữ liệu người dùng.
     *
     * @return true nếu backup thành công.
     */
    virtual bool backup() = 0;

    /**
     * @brief Tìm user theo ID.
     *
     * @param id ID cần tìm.
     * @return unique_ptr<User> nếu tìm thấy — caller nhận ownership.
     *         nullptr nếu không tìm thấy.
     */
    virtual std::unique_ptr<User> findById(UserId id) = 0;

    /**
     * @brief Tìm user theo tên đăng nhập.
     *
     * @details Dùng trong AuthService::login() để xác thực.
     *
     * @param username Tên đăng nhập cần tìm (case-sensitive).
     * @return unique_ptr<User> nếu tìm thấy — caller nhận ownership.
     *         nullptr nếu không tồn tại.
     */
    virtual std::unique_ptr<User> findByUsername(const std::string &username) = 0;

    /**
     * @brief Kiểm tra tên đăng nhập đã tồn tại trong hệ thống chưa.
     *
     * @details Dùng để validate trước khi tạo user mới,
     * tránh trùng username giữa các loại user khác nhau.
     *
     * @param username Tên đăng nhập cần kiểm tra.
     * @return true nếu đã tồn tại (ở bất kỳ role nào).
     */
    virtual bool usernameExists(const std::string &username) = 0;
};

#endif // I_USER_REPOSITORY_H