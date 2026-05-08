#pragma once

// ============================================================
//  core/interfaces/IExamService.h
// ============================================================

#include <string>
#include "Exam.h"
#include "Question.h"
#include "enums.h"

class IExamService
{
public:
    virtual ~IExamService() = default;

    virtual bool addExam(const std::string &subject, int durationMinutes,
                         ExamType type, TeacherId teacherId) = 0;
    virtual bool removeExam(ExamId examId) = 0;

    virtual bool addQuestion(ExamId examId, const Question &q) = 0;
    virtual bool updateQuestion(ExamId examId, QuestionId qId,
                                const Question &newQ) = 0;
    virtual bool removeQuestion(ExamId examId, QuestionId qId) = 0;

    virtual Exam *findExam(ExamId examId) = 0;

    // GV chỉ được sửa đề của chính mình
    virtual bool hasEditPermission(const Exam &exam, TeacherId teacherId) = 0;
};