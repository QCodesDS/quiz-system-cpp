#ifndef EXAM_SERVICE_H
#define EXAM_SERVICE_H

#include "core/interface/IExamService.h"
#include "core/interface/IExamRepository.h"

/**
 * @class ExamService
 * @brief Tầng nghiệp vụ xử lý logic Đề thi và Câu hỏi.
 *
 * Các trách nhiệm chính:
 * - Quản lý vòng đời Exam và Question (CRUD).
 * - Đảm bảo tính toàn vẹn dữ liệu thông qua ID tự tăng.
 * - Kiểm tra quyền sở hữu đối với Giảng viên.
 */
class ExamService : public IExamService
{
private:
    IExamRepository *_examRepo;

    // ------------------------------------------------------------
    //  ID Generation Logic
    // ------------------------------------------------------------

    ExamId nextExamId() const;
    QuestionId nextQuestionId() const;

public:
    explicit ExamService(IExamRepository *examRepo);

    // ------------------------------------------------------------
    //  Triển khai IExamService
    // ------------------------------------------------------------

    bool addExam(const std::string &subject, int durationMinutes,
                 ExamType type, TeacherId teacherId) override;
    bool removeExam(ExamId examId) override;

    bool addQuestion(ExamId examId, const Question &q) override;
    bool updateQuestion(ExamId examId, QuestionId qId,
                        const Question &newQ) override;
    bool removeQuestion(ExamId examId, QuestionId qId) override;

    Exam *findExam(ExamId examId) override;

    /**
     * @brief Kiểm tra xem Giảng viên có quyền sửa đề thi này không.
     * @return true nếu TeacherId trùng khớp với chủ sở hữu đề thi.
     */
    bool hasEditPermission(const Exam &exam, TeacherId tid) override;
};

#endif // EXAM_SERVICE_H