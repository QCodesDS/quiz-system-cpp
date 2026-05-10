#ifndef RESULT_REPOSITORY_H
#define RESULT_REPOSITORY_H

#include "repositories/FileRepository.h"
#include "core/interface/IResultRepository.h"

/**
 * @class ResultRepository
 * @brief Quản lý lưu trữ kết quả thi của sinh viên.
 *
 * Đặc điểm:
 * - Dữ liệu chỉ được thêm mới (Append-only), không sửa, không xóa.
 * - Đảm bảo an toàn tối đa cho lịch sử thi cử, tránh mất mát dữ liệu do ghi đè.
 */
class ResultRepository : public FileRepository, public IResultRepository
{
private:
    /**
     * @brief Chuyển đổi dòng văn bản thành đối tượng ExamResult.
     */
    ExamResult parseLine(const std::vector<std::string> &fields) const;

public:
    explicit ResultRepository(const std::string &resultFile);

    // ------------------------------------------------------------
    //  Triển khai IResultRepository
    // ------------------------------------------------------------

    /**
     * @brief Tải toàn bộ lịch sử kết quả thi (phục vụ thống kê/tra cứu).
     */
    std::vector<ExamResult> load() override;

    /**
     * @brief Ghi thêm một kết quả mới vào cuối tệp.
     */
    bool append(const ExamResult &r) override;

    /**
     * @brief Sao lưu tệp kết quả.
     */
    bool backup() override;

    /**
     * @brief Tìm kiếm lịch sử thi theo mã số sinh viên.
     */
    std::vector<ExamResult> findByStudent(StudentId sid) override;
};

#endif // RESULT_REPOSITORY_H