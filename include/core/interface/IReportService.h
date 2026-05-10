#ifndef I_REPORT_SERVICE_H
#define I_REPORT_SERVICE_H

#include "typedefs.h"

/**
 * @brief Dữ liệu một dòng trong bảng xếp hạng.
 *
 * @details Khai báo ở đây để tránh circular dependency giữa
 * IReportService và các model cụ thể.
 */
struct LeaderboardEntry
{
    StudentId studentId;     ///< ID học sinh.
    std::string studentName; ///< Tên hiển thị trong bảng xếp hạng.
    std::string subject;     ///< Môn học tương ứng.
    double bestScore;        ///< Điểm cao nhất đạt được (thang 10.0).
    int attemptCount;        ///< Tổng số lần làm bài (Practice + Official).
};

/**
 * @brief Thống kê tổng quan toàn hệ thống.
 *
 * @details Dùng cho dashboard Admin để hiển thị số liệu tổng hợp.
 */
struct SystemStats
{
    int totalStudents;   ///< Tổng số học sinh trong hệ thống.
    int totalTeachers;   ///< Tổng số giáo viên trong hệ thống.
    int totalExams;      ///< Tổng số đề thi đã tạo.
    int totalAttempts;   ///< Tổng số lượt làm bài (tất cả loại).
    double averageScore; ///< Điểm trung bình toàn hệ thống (thang 10.0).
};

/**
 * @brief Interface thống kê và báo cáo kết quả thi.
 *
 * @details Định nghĩa contract cho các loại báo cáo:
 * bảng xếp hạng toàn hệ thống, xếp hạng theo môn,
 * thống kê tổng quan, và điểm trung bình của một học sinh.
 *
 * @note Tất cả method đều chỉ đọc — không có thao tác ghi.
 */
class IReportService
{
public:
    virtual ~IReportService() = default;

    /**
     * @brief Lấy bảng xếp hạng tất cả học sinh theo điểm cao nhất.
     *
     * @details Mỗi học sinh xuất hiện một lần, lấy điểm cao nhất
     * trong tất cả các môn và loại bài thi.
     *
     * @return Danh sách LeaderboardEntry sắp xếp giảm dần theo bestScore.
     */
    virtual std::vector<LeaderboardEntry> getLeaderboard() = 0;

    /**
     * @brief Lấy bảng xếp hạng học sinh theo một môn cụ thể.
     *
     * @param subject Tên môn học cần lọc (vd: "Math", "Physics").
     * @return Danh sách LeaderboardEntry của môn đó, sắp xếp
     *         giảm dần theo bestScore.
     */
    virtual std::vector<LeaderboardEntry> getScoresBySubject(
        const std::string &subject) = 0;

    /**
     * @brief Lấy thống kê tổng quan toàn hệ thống.
     *
     * @return SystemStats chứa các số liệu tổng hợp.
     */
    virtual SystemStats getSystemStats() = 0;

    /**
     * @brief Tính điểm trung bình của một học sinh trên tất cả bài thi.
     *
     * @param studentId ID học sinh cần tính.
     * @return Điểm trung bình thang 10.0, hoặc 0.0 nếu chưa làm bài nào.
     */
    virtual double getAverageScore(StudentId studentId) = 0;
};

#endif // I_REPORT_SERVICE_H