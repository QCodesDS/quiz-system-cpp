#ifndef I_EXAM_REPOSITORY_H
#define I_EXAM_REPOSITORY_H

#include "core/models/Exam.h"

/**
 * @brief Interface lưu trữ và truy vấn dữ liệu đề thi.
 *
 * @details Định nghĩa contract cho mọi implementation lưu trữ Exam
 * (file-based, database, in-memory cho testing...).
 *
 * @note Exam và Question là value type (không đa hình) nên
 * các method trả về theo giá trị hoặc raw pointer mượn tạm,
 * không dùng unique_ptr.
 */
class IExamRepository
{
public:
    virtual ~IExamRepository() = default;

    /**
     * @brief Ghi toàn bộ danh sách đề thi ra storage.
     *
     * @param exams Danh sách đề thi cần lưu (mượn, không lấy ownership).
     * @return true nếu ghi thành công.
     */
    virtual bool save(const std::vector<Exam> &exams) = 0;

    /**
     * @brief Tải toàn bộ đề thi từ storage (bao gồm câu hỏi).
     *
     * @return Danh sách Exam đã được gắn câu hỏi đầy đủ.
     */
    virtual std::vector<Exam> load() = 0;

    /**
     * @brief Tạo bản sao lưu của file dữ liệu đề thi.
     *
     * @return true nếu backup thành công.
     */
    virtual bool backup() = 0;

    /**
     * @brief Tìm đề thi theo ID.
     *
     * @param id ID đề thi cần tìm.
     * @return Con trỏ đến Exam trong cache nội bộ, hoặc nullptr nếu
     *         không tìm thấy. Caller không sở hữu — không được delete.
     */
    virtual Exam *findById(ExamId id) = 0;

    /**
     * @brief Lấy tất cả đề thi do một giáo viên tạo.
     *
     * @param teacherId ID giáo viên cần lọc.
     * @return Danh sách bản sao các Exam thuộc giáo viên đó.
     */
    virtual std::vector<Exam> findByTeacher(TeacherId teacherId) = 0;

    /**
     * @brief Kiểm tra ID đề thi đã tồn tại trong storage chưa.
     *
     * @param id ID cần kiểm tra.
     * @return true nếu đã tồn tại.
     */
    virtual bool examIdExists(ExamId id) = 0;
};

#endif // I_EXAM_REPOSITORY_H