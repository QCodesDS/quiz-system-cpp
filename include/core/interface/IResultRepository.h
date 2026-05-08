#ifndef _IRESULTREPOSITORY_H_
#define _IRESULTREPOSITORY_H_

// ============================================================
//  core/interfaces/IResultRepository.h
//  append-only — không bao giờ rewrite toàn bộ file khi ghi
// ============================================================

#include <vector>
#include "ExamResult.h"

class IResultRepository
{
public:
    virtual ~IResultRepository() = default;

    virtual std::vector<ExamResult> load() = 0;
    virtual bool append(const ExamResult &r) = 0;
    virtual bool backup() = 0;

    virtual std::vector<ExamResult> findByStudent(StudentId sid) = 0;
};

#endif // !_IRESULTREPOSITORY_H_