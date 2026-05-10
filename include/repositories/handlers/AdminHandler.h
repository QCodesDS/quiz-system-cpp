#ifndef ADMIN_HANDLER_H
#define ADMIN_HANDLER_H

#include "IUserTypeHandler.h"
#include <string>

/**
 * @class AdminHandler
 * @brief Bộ xử lý cụ thể cho kiểu dữ liệu Admin.
 *
 * Đảm nhận trách nhiệm chuyển đổi danh sách đối tượng Admin thành chuỗi văn bản
 * để lưu trữ và ngược lại (parsing) từ file dữ liệu chuyên biệt của Admin.
 */
class AdminHandler : public IUserTypeHandler
{
private:
    std::string _filePath; ///< Đường dẫn tới file lưu trữ dữ liệu Admin (ví dụ: admins.txt).

public:
    /**
     * @brief Khởi tạo AdminHandler với đường dẫn file cụ thể.
     * @param adminFile Đường dẫn file nguồn.
     */
    explicit AdminHandler(const std::string &adminFile);

    // ------------------------------------------------------------
    //  Interface Implementation
    // ------------------------------------------------------------

    std::string getRole() const override;
    std::string getFilePath() const override;

    /**
     * @brief Chuyển đổi danh sách User (chỉ lọc Admin) thành các dòng văn bản.
     * @param users Danh sách con trỏ thông minh quản lý các đối tượng User.
     * @return std::vector<std::string> — Danh sách các dòng đã được định dạng.
     */
    std::vector<std::string> serialize(
        const std::vector<std::unique_ptr<User>> &users) override;

    /**
     * @brief Phân tích dữ liệu từ các dòng văn bản để tái tạo đối tượng Admin.
     * @param lines Danh sách các dòng đọc được từ file.
     * @return std::vector<std::unique_ptr<User>> — Danh sách các đối tượng Admin được tái tạo.
     */
    std::vector<std::unique_ptr<User>> deserialize(
        const std::vector<std::string> &lines) override;
};

#endif // ADMIN_HANDLER_H