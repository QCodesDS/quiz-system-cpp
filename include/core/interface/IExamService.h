#ifndef I_EXAM_SERVICE_H
#define I_EXAM_SERVICE_H

#include "core/models/Exam.h"

/**
 * @brief Interface quản lý CRUD đề thi và câu hỏi.
 *
 * @details Định nghĩa contract cho toàn bộ nghiệp vụ liên quan đến
 * đề thi: tạo, xóa đề thi, thêm/sửa/xóa câu hỏi, và kiểm tra
 * quyền chỉnh sửa.
 *
 * @note Giáo viên chỉ được sửa đề thi do chính mình tạo.
 * Kiểm tra quyền qua hasEditPermission() trước mọi thao tác sửa.
 */
class IExamService
{
public:
    virtual ~IExamService() = default;

    /**
     * @brief Tạo đề thi mới và lưu vào storage.
     *
     * @param subject         Môn học (vd: "Math", "Physics", "CPP").
     * @param durationMinutes Thời gian làm bài tính bằng phút.
     * @param type            Loại đề: Practice hoặc Official.
     * @param teacherId       ID giáo viên tạo đề.
     * @return true nếu tạo và lưu thành công.
     */
    virtual bool addExam(const std::string &subject,
                         int durationMinutes,
                         ExamType type,
                         TeacherId teacherId) = 0;

    /**
     * @brief Xóa đề thi và toàn bộ câu hỏi thuộc đề đó.
     *
     * @param examId ID đề thi cần xóa.
     * @return true nếu xóa thành công, false nếu không tìm thấy.
     */
    virtual bool removeExam(ExamId examId) = 0;

    /**
     * @brief Thêm câu hỏi vào đề thi.
     *
     * @param examId ID đề thi đích.
     * @param q      Câu hỏi cần thêm (ID sẽ được sinh tự động).
     * @return true nếu thêm thành công.
     */
    virtual bool addQuestion(ExamId examId, const Question &q) = 0;

    /**
     * @brief Cập nhật nội dung một câu hỏi trong đề thi.
     *
     * @param examId ID đề thi chứa câu hỏi.
     * @param qId    ID câu hỏi cần cập nhật.
     * @param newQ   Nội dung câu hỏi mới.
     * @return true nếu cập nhật thành công, false nếu không tìm thấy.
     */
    virtual bool updateQuestion(ExamId examId,
                                QuestionId qId,
                                const Question &newQ) = 0;

    /**
     * @brief Xóa một câu hỏi khỏi đề thi.
     *
     * @param examId ID đề thi chứa câu hỏi.
     * @param qId    ID câu hỏi cần xóa.
     * @return true nếu xóa thành công, false nếu không tìm thấy.
     */
    virtual bool removeQuestion(ExamId examId, QuestionId qId) = 0;

    /**
     * @brief Tìm đề thi theo ID.
     *
     * @param examId ID đề thi cần tìm.
     * @return Con trỏ mượn đến Exam, hoặc nullptr nếu không tìm thấy.
     *         Caller không sở hữu — không được delete.
     */
    virtual Exam *findExam(ExamId examId) = 0;

    /**
     * @brief Kiểm tra giáo viên có quyền chỉnh sửa đề thi không.
     *
     * @details Giáo viên chỉ được sửa đề do chính mình tạo.
     * Phải gọi hàm này trước addQuestion(), updateQuestion(),
     * removeQuestion(), và removeExam().
     *
     * @param exam      Đề thi cần kiểm tra.
     * @param teacherId ID giáo viên thực hiện thao tác.
     * @return true nếu teacherId khớp với người tạo đề.
     */
    virtual bool hasEditPermission(const Exam &exam,
                                   TeacherId teacherId) = 0;
};

#endif // I_EXAM_SERVICE_H