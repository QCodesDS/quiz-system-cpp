#ifndef TEACHER_HANDLER_H
#define TEACHER_HANDLER_H

#include "repositories/handlers/IUserTypeHandler.h"

/**
 * @class TeacherHandler
 * @brief Bộ xử lý chuyên biệt cho đối tượng Giáo viên (Teacher).
 *
 * Quản lý việc lưu trữ và tái tạo dữ liệu giáo viên, bao gồm các thông tin
 * đặc thù như môn học phụ trách (subject) và lớp được phân công (assignedClass).
 */
class TeacherHandler : public IUserTypeHandler
{
private:
    std::string _filePath; ///< Đường dẫn tệp dữ liệu giáo viên.

public:
    /**
     * @brief Khởi tạo TeacherHandler.
     * @param teacherFile Đường dẫn tệp nguồn (ví dụ: "teachers.txt").
     */
    explicit TeacherHandler(const std::string &teacherFile);

    // ------------------------------------------------------------
    //  Interface Implementation
    // ------------------------------------------------------------

    std::string getRole() const override;
    std::string getFilePath() const override;

    /**
     * @brief Chuyển đổi danh sách Teacher thành các dòng văn bản lưu tệp.
     */
    std::vector<std::string> serialize(
        const std::vector<std::unique_ptr<User>> &users) override;

    /**
     * @brief Phân tích dữ liệu tệp để tạo danh sách đối tượng Teacher.
     */
    std::vector<std::unique_ptr<User>> deserialize(
        const std::vector<std::string> &lines) override;
};

#endif // TEACHER_HANDLER_H