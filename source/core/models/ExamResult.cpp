#include "ExamResult.h"
#include <iomanip>
#include <sstream>

ExamResult::ExamResult(StudentId studentId, const std::string &studentName,
                       ExamId examId, const std::string &subject,
                       double score, int durationSeconds,
                       ExamType examType, const std::string &submittedAt)
    : _studentId(studentId), _studentName(studentName),
      _examId(examId), _subject(subject),
      _score(score), _durationSeconds(durationSeconds),
      _examType(examType), _submittedAt(submittedAt) {}

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
    return _score >= 5.0; // khớp với Constants::PASS_SCORE
}

std::string ExamResult::toFileString() const
{
    // Format: StudentId|StudentName|ExamId|Subject|Score|DurationSeconds|ExamType|SubmittedAt
    std::ostringstream oss;
    oss << _studentId << "|" << _studentName << "|"
        << _examId << "|" << _subject << "|"
        << std::fixed << std::setprecision(2) << _score << "|"
        << _durationSeconds << "|"
        << (_examType == ExamType::Official ? "Official" : "Practice") << "|"
        << _submittedAt;
    return oss.str();
}