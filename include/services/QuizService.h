#ifndef QUIZ_SERVICE_H
#define QUIZ_SERVICE_H

#include "IQuizService.h"
#include "IExamRepository.h"
#include "IResultRepository.h"

// ============================================================
//  services/QuizService.h
//
//  Toàn bộ logic làm bài thi:
//  - canAttempt()      : Official chỉ được làm 1 lần, Practice không giới hạn
//  - calculateScore()  : so đáp án, tính điểm thang 10
//  - createResult()    : tạo ExamResult + tự động lưu vào repo
//  - isPassed()        : điểm >= 5.0
// ============================================================

class QuizService : public IQuizService
{
private:
    IExamRepository *examRepo;
    IResultRepository *resultRepo;

    // Lấy timestamp hiện tại dạng "YYYY-MM-DD HH:MM:SS"
    static std::string currentTimestamp();

public:
    QuizService(IExamRepository *examRepo, IResultRepository *resultRepo);

    // --- IQuizService ---
    bool canAttempt(StudentId studentId, ExamId examId) override;

    double calculateScore(const Exam &exam,
                          const std::vector<int> &answers) override;

    ExamResult createResult(StudentId studentId,
                            const std::string &studentName,
                            const Exam &exam,
                            double score,
                            int durationSeconds) override;

    bool isPassed(double score) override;
};

#endif // QUIZ_SERVICE_H