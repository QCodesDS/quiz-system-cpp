#ifndef RESULT_REPOSITORY_H
#define RESULT_REPOSITORY_H

// ============================================================
//  repositories/ResultRepository.h
//
//  Khác với 2 repo kia:
//  - KHÔNG bao giờ rewrite toàn bộ file khi ghi
//  - append() chỉ thêm 1 dòng vào cuối → tránh mất dữ liệu thi
//  - load() đọc toàn bộ để thống kê / hiển thị lịch sử
//
//  results.txt là file audit log — không có "edit" hay "delete".
// ============================================================

#include "FileRepository.h"
#include "IResultRepository.h"

class ResultRepository : public FileRepository, public IResultRepository
{
private:
    ExamResult parseLine(const std::vector<std::string> &fields) const;

public:
    explicit ResultRepository(const std::string &resultFile);

    // --- IResultRepository ---
    std::vector<ExamResult> load() override;
    bool append(const ExamResult &r) override;
    bool backup() override;

    std::vector<ExamResult> findByStudent(StudentId sid) override;
};

#endif // RESULT_REPOSITORY_H