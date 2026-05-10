#ifndef I_RESULT_REPOSITORY_H
#define I_RESULT_REPOSITORY_H

#include "ExamResult.h"

/**
 * @brief Interface lưu trữ kết quả thi theo mô hình append-only.
 *
 * @details Kết quả thi là dữ liệu lịch sử — không bao giờ bị sửa
 * hay xóa sau khi đã ghi. Vì vậy interface này chỉ cung cấp
 * append(), không có update() hay delete().
 *
 * Mô hình append-only giúp:
 * - Tránh mất dữ liệu do ghi đè toàn bộ file.
 * - Ghi nhanh hơn (chỉ thêm cuối file, không rewrite).
 * - Dễ audit — lịch sử thi không thể bị thay đổi.
 *
 * @note Thứ tự ưu tiên khi implement: append() phải là atomic —
 * nếu crash giữa chừng, file không được bị corrupt.
 */
class IResultRepository
{
public:
    virtual ~IResultRepository() = default;

    /**
     * @brief Tải toàn bộ kết quả thi từ storage.
     *
     * @return Danh sách ExamResult theo thứ tự thời gian ghi.
     */
    virtual std::vector<ExamResult> load() = 0;

    /**
     * @brief Ghi thêm một kết quả thi vào cuối storage.
     *
     * @details Không rewrite toàn bộ file — chỉ append một dòng.
     *
     * @param r Kết quả thi cần lưu (mượn, không lấy ownership).
     * @return true nếu ghi thành công.
     */
    virtual bool append(const ExamResult &r) = 0;

    /**
     * @brief Tạo bản sao lưu của file kết quả thi.
     *
     * @return true nếu backup thành công.
     */
    virtual bool backup() = 0;

    /**
     * @brief Lấy toàn bộ kết quả thi của một học sinh.
     *
     * @param sid ID học sinh cần lọc.
     * @return Danh sách ExamResult của học sinh đó,
     *         theo thứ tự thời gian nộp bài.
     */
    virtual std::vector<ExamResult> findByStudent(StudentId sid) = 0;
};

#endif // I_RESULT_REPOSITORY_H