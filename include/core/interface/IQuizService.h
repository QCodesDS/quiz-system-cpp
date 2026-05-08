#ifndef _IQUIZSERVICE_H_
#define _IQUIZSERVICE_H_

// ============================================================
//  core/interfaces/IQuizService.h
// ============================================================

#include <vector>
#include "Exam.h"
#include "ExamResult.h"

class IQuizService
{
public:
    virtual ~IQuizService() = default;

    // Kiểm tra HS có được làm bài không
    // (Official: chưa làm lần nào; Practice: luôn true)
    virtual bool canAttempt(StudentId studentId, ExamId examId) = 0;

    // answers: index 0..n-1 tương ứng câu hỏi, giá trị 1–4
    virtual double calculateScore(const Exam &exam,
                                  const std::vector<int> &answers) = 0;

    virtual ExamResult createResult(StudentId studentId,
                                    const std::string &studentName,
                                    const Exam &exam,
                                    double score,
                                    int durationSeconds) = 0;

    virtual bool isPassed(double score) = 0;
};

#endif // _IQUIZSERVICE_H_