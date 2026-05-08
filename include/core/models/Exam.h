#ifndef _EXAM_H_
#define _EXAM_H_

#include <string>
#include <vector>
#include "Question.h"
#include "types/enums.h"
#include "types/typedefs.h"

// ============================================================
//  core/models/Exam.h
//  Đề thi — chứa vector<Question>
//
//  Exam không tự load câu hỏi từ file.
//  ExamRepository chịu trách nhiệm assemble Exam + Questions.
// ============================================================

class Exam
{
private:
    ExamId _examId;
    std::string _subject;
    int _durationMinutes;
    ExamType _examType;
    TeacherId _teacherId;
    std::vector<Question> _questions;

public:
    Exam(ExamId examId, const std::string &subject,
         int durationMinutes, ExamType examType, TeacherId teacherId);

    // --- Getters ---
    ExamId getExamId() const;
    std::string getSubject() const;
    int getDurationMinutes() const;
    ExamType getType() const;
    TeacherId getTeacherId() const;
    int getQuestionCount() const;

    const std::vector<Question> &getQuestions() const;

    // --- Mutation ---
    bool addQuestion(const Question &q);
    bool removeQuestion(QuestionId qId); // trả false nếu không tìm thấy

    // Ghi phần header của exam (không bao gồm questions)
    std::string toFileString() const;
};

#endif // !_EXAM_H_