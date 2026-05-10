#ifndef QUIZ_SERVICE_H
#define QUIZ_SERVICE_H

#include "core/interface/IQuizService.h"
#include "core/interface/IExamRepository.h"
#include "core/interface/IResultRepository.h"

/**
 * @class QuizService
 * @brief Điều phối logic thực hiện bài thi và chấm điểm.
 *
 * Trách nhiệm:
 * - Kiểm soát quyền làm bài (chống thi lại bài thi chính thức).
 * - Tính toán điểm số chính xác dựa trên đáp án.
 * - Ghi nhận và lưu trữ kết quả cuối cùng.
 */
class QuizService : public IQuizService
{
private:
    IExamRepository *_examRepo;
    IResultRepository *_resultRepo;

    /**
     * @brief Tiện ích lấy thời gian thực hệ thống.
     * @return Chuỗi định dạng "YYYY-MM-DD HH:MM:SS"
     */
    static std::string currentTimestamp();

public:
    QuizService(IExamRepository *examRepo, IResultRepository *resultRepo);

    // ------------------------------------------------------------
    //  Triển khai IQuizService
    // ------------------------------------------------------------

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