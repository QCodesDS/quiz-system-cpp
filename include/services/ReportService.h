#ifndef REPORT_SERVICE_H
#define REPORT_SERVICE_H

#include "IReportService.h"
#include "IUserRepository.h"
#include "IResultRepository.h"

// ============================================================
//  services/ReportService.h
//
//  Thống kê và bảng xếp hạng.
//  Không thay đổi data — chỉ đọc và aggregate.
// ============================================================

class ReportService : public IReportService
{
private:
    IUserRepository *userRepo;
    IResultRepository *resultRepo;

public:
    ReportService(IUserRepository *userRepo, IResultRepository *resultRepo);

    // --- IReportService ---

    // Xếp hạng toàn trường: mỗi học sinh lấy điểm trung bình cao nhất
    std::vector<LeaderboardEntry> getLeaderboard() override;

    // Xếp hạng theo môn: lấy điểm cao nhất của mỗi HS trong môn đó
    std::vector<LeaderboardEntry> getScoresBySubject(const std::string &s) override;

    SystemStats getSystemStats() override;
    double getAverageScore(StudentId) override;
};

#endif // REPORT_SERVICE_H