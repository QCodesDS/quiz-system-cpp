/**
 * @file ExamResult.cpp
 * @brief Triển khai lưu trữ kết quả bài thi.
 */

#include "core/models/ExamResult.h"
#include <iomanip>
#include <sstream>

ExamResult::ExamResult(StudentId studentId, const std::string &studentName,
                       ExamId examId, const std::string &subject,
                       double score, int durationSeconds,
                       ExamType examType, const std::string &submittedAt)
    : _studentId(studentId),
      _studentName(studentName),
      _examId(examId),
      _subject(subject),
      _score(score),
      _durationSeconds(durationSeconds),
      _examType(examType),
      _submittedAt(submittedAt)
{
}

// ------------------------------------------------------------
//  Getters
// ------------------------------------------------------------

StudentId ExamResult::getStudentId() const { return _studentId; }
std::string ExamResult::getStudentName() const { return _studentName; }
ExamId ExamResult::getExamId() const { return _examId; }
std::string ExamResult::getSubject() const { return _subject; }
double ExamResult::getScore() const { return _score; }
int ExamResult::getDurationSeconds() const { return _durationSeconds; }
ExamType ExamResult::getExamType() const { return _examType; }
std::string ExamResult::getSubmittedAt() const { return _submittedAt; }

bool ExamResult::isPassed() const
{
    // Ngưỡng điểm đạt tối thiểu theo quy định hệ thống.
    return _score >= 5.0;
}

std::string ExamResult::toFileString() const
{
    /**
     * Sử dụng ostringstream để kiểm soát định dạng số thập phân.
     * Điểm số được cố định ở 2 chữ số sau dấu phẩy (ví dụ: 8.50).
     */
    std::ostringstream oss;
    oss << _studentId << "|"
        << _studentName << "|"
        << _examId << "|"
        << _subject << "|"
        << std::fixed << std::setprecision(2) << _score << "|"
        << _durationSeconds << "|"
        << (_examType == ExamType::Official ? "Official" : "Practice") << "|"
        << _submittedAt;

    return oss.str();
}