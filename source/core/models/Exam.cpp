#include "Exam.h"
#include <algorithm>

static std::string examTypeToString(ExamType t)
{
    return t == ExamType::Official ? "Official" : "Practice";
}

Exam::Exam(ExamId examId, const std::string &subject,
           int durationMinutes, ExamType examType, TeacherId teacherId)
    : _examId(examId), _subject(subject), _durationMinutes(durationMinutes),
      _examType(examType), _teacherId(teacherId) {}

ExamId Exam::getExamId() const { return _examId; }
std::string Exam::getSubject() const { return _subject; }
int Exam::getDurationMinutes() const { return _durationMinutes; }
ExamType Exam::getType() const { return _examType; }
TeacherId Exam::getTeacherId() const { return _teacherId; }
int Exam::getQuestionCount() const { return static_cast<int>(_questions.size()); }

const std::vector<Question> &Exam::getQuestions() const { return _questions; }

bool Exam::addQuestion(const Question &q)
{
    _questions.push_back(q);
    return true;
}

bool Exam::removeQuestion(QuestionId qId)
{
    auto it = std::find_if(_questions.begin(), _questions.end(),
                           [qId](const Question &q)
                           { return q.getId() == qId; });
    if (it == _questions.end())
        return false;
    _questions.erase(it);
    return true;
}

std::string Exam::toFileString() const
{
    // Format: ExamId|Subject|DurationMinutes|ExamType|TeacherId
    return std::to_string(_examId) + "|" + _subject + "|" + std::to_string(_durationMinutes) + "|" + examTypeToString(_examType) + "|" + std::to_string(_teacherId);
}