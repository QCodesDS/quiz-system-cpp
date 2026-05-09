#ifndef EXAM_SERVICE_H
#define EXAM_SERVICE_H

#include "IExamService.h"
#include "IExamRepository.h"

// ============================================================
//  services/ExamService.h
//
//  CRUD đề thi và câu hỏi.
//  Kiểm tra quyền: GV chỉ được sửa/xoá đề của chính mình.
//  Admin không đi qua đây — Admin dùng trực tiếp repo nếu cần.
// ============================================================

class ExamService : public IExamService
{
private:
    IExamRepository *examRepo;

    // ID tự tăng
    ExamId nextExamId() const;
    QuestionId nextQuestionId() const;

public:
    explicit ExamService(IExamRepository *examRepo);

    // --- IExamService ---
    bool addExam(const std::string &subject, int durationMinutes,
                 ExamType type, TeacherId teacherId) override;
    bool removeExam(ExamId examId) override;

    bool addQuestion(ExamId examId, const Question &q) override;
    bool updateQuestion(ExamId examId, QuestionId qId,
                        const Question &newQ) override;
    bool removeQuestion(ExamId examId, QuestionId qId) override;

    Exam *findExam(ExamId examId) override;

    bool hasEditPermission(const Exam &exam, TeacherId tid) override;
};

#endif // EXAM_SERVICE_H