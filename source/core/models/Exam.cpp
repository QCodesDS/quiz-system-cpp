/**
 * @file Exam.cpp
 * @brief Triển khai các phương thức quản lý nội dung đề thi.
 */

#include "core/models/Exam.h"
#include <algorithm>

/**
 * @brief Helper nội bộ chuyển đổi Enum sang String phục vụ lưu trữ.
 */
static std::string examTypeToString(ExamType t)
{
    return t == ExamType::Official ? "Official" : "Practice";
}

Exam::Exam(ExamId examId, const std::string &subject,
           int durationMinutes, ExamType examType, TeacherId teacherId)
    : _examId(examId), _subject(subject), _durationMinutes(durationMinutes),
      _examType(examType), _teacherId(teacherId)
{
    // Thành viên _questions được khởi tạo mặc định là vector rỗng.
}

ExamId Exam::getExamId() const { return _examId; }
std::string Exam::getSubject() const { return _subject; }
int Exam::getDurationMinutes() const { return _durationMinutes; }
ExamType Exam::getType() const { return _examType; }
TeacherId Exam::getTeacherId() const { return _teacherId; }
int Exam::getQuestionCount() const { return static_cast<int>(_questions.size()); }

const std::vector<Question> &Exam::getQuestions() const
{
    return _questions;
}

bool Exam::addQuestion(const Question &q)
{
    _questions.push_back(q);
    return true;
}

bool Exam::removeQuestion(QuestionId qId)
{
    // Tìm kiếm câu hỏi trong danh sách dựa trên ID.
    auto it = std::find_if(_questions.begin(), _questions.end(),
                           [qId](const Question &q)
                           {
                               return q.getId() == qId;
                           });

    if (it == _questions.end())
    {
        return false;
    }

    _questions.erase(it);
    return true;
}

std::string Exam::toFileString() const
{
    /**
     * Chỉ lưu thông tin định danh của Exam.
     * Các Question liên quan thường được lưu ở một file/bảng riêng
     * để đảm bảo tính chuẩn hóa dữ liệu.
     */
    return std::to_string(_examId) + "|" +
           _subject + "|" +
           std::to_string(_durationMinutes) + "|" +
           examTypeToString(_examType) + "|" +
           std::to_string(_teacherId);
}