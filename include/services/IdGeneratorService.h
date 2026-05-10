#ifndef ID_GENERATOR_SERVICE_H
#define ID_GENERATOR_SERVICE_H

#include "core/interface/IUserRepository.h"
#include "core/types/typedefs.h"

/**
 * @class IdGeneratorService
 * @brief Chuyên trách việc tạo mã định danh (ID) tuần tự cho người dùng.
 *
 * Quy tắc cấp phát:
 * - Student ID: Bắt đầu từ 1001.
 * - Teacher ID: Bắt đầu từ 2001.
 */
class IdGeneratorService
{
private:
    IUserRepository *_userRepo;

    /**
     * @brief Tìm ID lớn nhất hiện có trong hệ thống dựa trên vai trò.
     * @param role Vai trò cần lọc ("Student" hoặc "Teacher").
     * @param defaultMin Giá trị mặc định nếu chưa có người dùng nào.
     */
    UserId findMaxIdByRole(const std::string &role, UserId defaultMin) const;

public:
    explicit IdGeneratorService(IUserRepository *userRepo);

    /**
     * @brief Sinh ID tiếp theo cho Sinh viên.
     */
    UserId nextStudentId() const;

    /**
     * @brief Sinh ID tiếp theo cho Giảng viên.
     */
    UserId nextTeacherId() const;
};

#endif // ID_GENERATOR_SERVICE_H