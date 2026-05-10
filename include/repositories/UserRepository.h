#ifndef USER_REPOSITORY_H
#define USER_REPOSITORY_H

#include "FileRepository.h"
#include "IUserRepository.h"
#include "IUserTypeHandler.h"

/**
 * @class UserRepository
 * @brief Lớp quản lý lưu trữ người dùng tập trung.
 *
 * Lớp này kế thừa FileRepository để sử dụng các công cụ đọc/ghi file cơ bản
 * và triển khai IUserRepository để expose các nghiệp vụ truy vấn dữ liệu.
 *
 * @note Sử dụng Handler Pattern: UserRepository không cần biết cách xử lý từng loại User cụ thể,
 *       nó chỉ lặp qua danh sách các Handlers được tiêm (inject) vào từ constructor.
 */
class UserRepository : public FileRepository, public IUserRepository
{
private:
    /**
     * @brief Danh sách các bộ xử lý loại người dùng.
     * UserRepository chỉ mượn (borrow) con trỏ thô vì vòng đời của các Handler
     * thường được quản lý bởi ServiceContainer.
     */
    std::vector<IUserTypeHandler *> _handlers;

public:
    /**
     * @brief Khởi tạo UserRepository với danh sách các bộ xử lý.
     * @param dataDir Thư mục chứa các tệp dữ liệu.
     * @param handlers Danh sách các Handler (Admin, Teacher, Student).
     */
    UserRepository(const std::string &dataDir,
                   std::vector<IUserTypeHandler *> handlers);

    // ------------------------------------------------------------
    //  Triển khai IUserRepository
    // ------------------------------------------------------------

    bool save(const std::vector<std::unique_ptr<User>> &users) override;
    std::vector<std::unique_ptr<User>> load() override;
    bool backup() override;

    std::unique_ptr<User> findById(UserId id) override;
    std::unique_ptr<User> findByUsername(const std::string &username) override;
    bool usernameExists(const std::string &username) override;
};

#endif // USER_REPOSITORY_H