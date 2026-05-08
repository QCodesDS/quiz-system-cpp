#ifndef _IEXAMREPOSITORY_H_
#define _IEXAMREPOSITORY_H_

// ============================================================
//  core/interfaces/IExamRepository.h
// ============================================================

#include <vector>
#include "Exam.h"

class IExamRepository
{
public:
    virtual ~IExamRepository() = default;

    virtual bool save(const std::vector<Exam> &exams) = 0;
    virtual std::vector<Exam> load() = 0;
    virtual bool backup() = 0;

    virtual Exam *findById(ExamId id) = 0;
    virtual std::vector<Exam> findByTeacher(TeacherId teacherId) = 0;
    virtual bool examIdExists(ExamId id) = 0;
};

#endif // !_IEXAMREPOSITORY_H_