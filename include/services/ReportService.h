#ifndef REPORT_SERVICE_H
#define REPORT_SERVICE_H

#include "core/interface/IReportService.h"
#include "core/interface/IUserRepository.h"
#include "core/interface/IResultRepository.h"

/**
 * @class ReportService
 * @brief Chuyên trách tính toán thống kê và trích xuất báo cáo.
 *
 * Lưu ý: Lớp này tuân thủ nguyên tắc Read-Only đối với dữ liệu,
 * chỉ thực hiện tổng hợp (aggregation) để phục vụ hiển thị.
 */
class ReportService : public IReportService
{
private:
    IUserRepository *_userRepo;
    IResultRepository *_resultRepo;

public:
    ReportService(IUserRepository *userRepo, IResultRepository *resultRepo);

    // --- Triển khai IReportService ---

    // Xếp hạng toàn trường dựa trên điểm trung bình của mỗi học sinh
    std::vector<LeaderboardEntry> getLeaderboard() override;

    // Xếp hạng theo môn học dựa trên điểm cao nhất của mỗi học sinh
    std::vector<LeaderboardEntry> getScoresBySubject(const std::string &subject) override;

    // Tổng hợp số liệu toàn hệ thống
    SystemStats getSystemStats() override;

    // Tính điểm trung bình tích lũy của một sinh viên cụ thể
    double getAverageScore(StudentId sid) override;
};

#endif // REPORT_SERVICE_H