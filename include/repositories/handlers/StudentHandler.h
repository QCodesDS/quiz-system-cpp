#ifndef STUDENT_HANDLER_H
#define STUDENT_HANDLER_H

#include "IUserTypeHandler.h"

/**
 * @class StudentHandler
 * @brief Bộ xử lý chuyên biệt cho đối tượng Student.
 *
 * Đảm nhận việc chuyển đổi dữ liệu phức tạp của Student (bao gồm lớp học,
 * giới tính, tuổi, SĐT) giữa thực thể trong bộ nhớ và định dạng lưu trữ tệp.
 */
class StudentHandler : public IUserTypeHandler
{
private:
    std::string _filePath; ///< Đường dẫn tệp dữ liệu của học sinh.

public:
    /**
     * @brief Khởi tạo StudentHandler.
     * @param studentFile Đường dẫn tệp nguồn (ví dụ: "students.txt").
     */
    explicit StudentHandler(const std::string &studentFile);

    // ------------------------------------------------------------
    //  Interface Implementation
    // ------------------------------------------------------------

    std::string getRole() const override;
    std::string getFilePath() const override;

    /**
     * @brief Chuyển đổi danh sách Student thành các dòng văn bản định dạng '|'.
     */
    std::vector<std::string> serialize(
        const std::vector<std::unique_ptr<User>> &users) override;

    /**
     * @brief Phân tích các dòng văn bản để tái tạo danh sách đối tượng Student.
     * @throws std::invalid_argument hoặc std::out_of_range nếu parse số thất bại (được xử lý nội bộ).
     */
    std::vector<std::unique_ptr<User>> deserialize(
        const std::vector<std::string> &lines) override;
};

#endif // STUDENT_HANDLER_H