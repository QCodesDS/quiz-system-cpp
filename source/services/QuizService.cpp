#include "services/QuizService.h"
#include <ctime>
#include <sstream>
#include <iomanip>

QuizService::QuizService(IExamRepository *examRepo,
                         IResultRepository *resultRepo)
    : _examRepo(examRepo), _resultRepo(resultRepo)
{
}

// ------------------------------------------------------------
//  Helper: Timestamp Generation
// ------------------------------------------------------------
std::string QuizService::currentTimestamp()
{
    std::time_t now = std::time(nullptr);
    std::tm *t = std::localtime(&now);

    std::ostringstream oss;
    // Sử dụng put_time để đảm bảo định dạng chuẩn ISO cho cơ sở dữ liệu file
    oss << std::put_time(t, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// ------------------------------------------------------------
//  Quyền làm bài
// ------------------------------------------------------------
bool QuizService::canAttempt(StudentId studentId, ExamId examId)
{
    Exam *exam = _examRepo->findById(examId);
    if (!exam)
        return false;

    // Chế độ luyện tập: Luôn cho phép
    if (exam->getType() == ExamType::Practice)
        return true;

    // Chế độ thi thật: Kiểm tra lịch sử trong ResultRepository
    auto history = _resultRepo->findByStudent(studentId);
    for (const auto &res : history)
    {
        if (res.getExamId() == examId && res.getExamType() == ExamType::Official)
        {
            return false; // Đã tồn tại kết quả thi chính thức
        }
    }
    return true;
}

// ------------------------------------------------------------
//  Logic chấm điểm (Thang điểm 10)
// ------------------------------------------------------------
double QuizService::calculateScore(const Exam &exam,
                                   const std::vector<int> &answers)
{
    const auto &questions = exam.getQuestions();
    if (questions.empty())
        return 0.0;

    int correctCount = 0;
    int totalQuestions = static_cast<int>(questions.size());

    // So khớp từng câu trả lời với đáp án đúng
    for (int i = 0; i < totalQuestions && i < static_cast<int>(answers.size()); ++i)
    {
        if (answers[i] == questions[i].getCorrectChoice())
        {
            correctCount++;
        }
    }

    // Công thức: (Số câu đúng / Tổng số câu) * 10
    double rawScore = (static_cast<double>(correctCount) / totalQuestions) * 10.0;

    // Làm tròn đến 2 chữ số thập phân nếu cần (Tùy chọn)
    return rawScore;
}

// ------------------------------------------------------------
//  Tạo và Lưu kết quả
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

    // Tự động đẩy xuống tầng Persistence (ResultRepository)
    _resultRepo->append(result);

    return result;
}

bool QuizService::isPassed(double score)
{
    return score >= 5.0;
}