#include "QuizService.h"
#include <ctime>
#include <sstream>
#include <iomanip>

// ============================================================
//  services/QuizService.cpp
// ============================================================

QuizService::QuizService(IExamRepository *examRepo,
                         IResultRepository *resultRepo)
    : examRepo(examRepo), resultRepo(resultRepo) {}

// ------------------------------------------------------------
//  currentTimestamp — "YYYY-MM-DD HH:MM:SS"
// ------------------------------------------------------------
std::string QuizService::currentTimestamp()
{
    std::time_t now = std::time(nullptr);
    std::tm *t = std::localtime(&now);

    std::ostringstream oss;
    oss << std::put_time(t, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// ------------------------------------------------------------
//  canAttempt
//  Official → kiểm tra kết quả cũ, nếu đã có thì false
//  Practice → luôn true
// ------------------------------------------------------------
bool QuizService::canAttempt(StudentId studentId, ExamId examId)
{
    Exam *exam = examRepo->findById(examId);
    if (!exam)
        return false;

    if (exam->getType() == ExamType::Practice)
        return true;

    // Official: kiểm tra đã làm chưa
    auto history = resultRepo->findByStudent(studentId);
    for (const auto &r : history)
    {
        if (r.getExamId() == examId && r.getExamType() == ExamType::Official)
        {
            return false; // đã làm rồi
        }
    }
    return true;
}

// ------------------------------------------------------------
//  calculateScore
//  answers[i] = đáp án học sinh cho câu i (1–4, 0 = bỏ qua)
//  Mỗi câu đúng được 10 / tổng số câu điểm
// ------------------------------------------------------------
double QuizService::calculateScore(const Exam &exam,
                                   const std::vector<int> &answers)
{
    const auto &questions = exam.getQuestions();
    if (questions.empty())
        return 0.0;

    int correct = 0;
    int total = static_cast<int>(questions.size());

    for (int i = 0; i < total && i < static_cast<int>(answers.size()); ++i)
    {
        if (answers[i] == questions[i].getCorrectChoice())
            ++correct;
    }

    return (static_cast<double>(correct) / total) * 10.0;
}

// ------------------------------------------------------------
//  createResult
//  Tạo ExamResult và tự động append vào resultRepo.
//  Trả về object để UI có thể hiển thị ngay.
// ------------------------------------------------------------
ExamResult QuizService::createResult(StudentId studentId,
                                     const std::string &studentName,
                                     const Exam &exam,
                                     double score,
                                     int durationSeconds)
{
    ExamResult result(
        studentId,
        studentName,
        exam.getExamId(),
        exam.getSubject(),
        score,
        durationSeconds,
        exam.getType(),
        currentTimestamp());

    resultRepo->append(result);
    return result;
}

bool QuizService::isPassed(double score)
{
    return score >= 5.0;
}