#ifndef _IREPORTSERVICE_H_
#define _IREPORTSERVICE_H_

// ============================================================
//  core/interfaces/IReportService.h
//  Struct phụ trợ cho report — khai báo ở đây để tránh circular deps
// ============================================================

#include <string>
#include <vector>
#include "typedefs.h"

struct LeaderboardEntry
{
    StudentId studentId;
    std::string studentName;
    std::string subject;
    double bestScore;
    int attemptCount;
};

struct SystemStats
{
    int totalStudents;
    int totalTeachers;
    int totalExams;
    int totalAttempts;
    double averageScore; // trung bình toàn hệ thống
};

class IReportService
{
public:
    virtual ~IReportService() = default;

    // Xếp hạng tất cả HS theo điểm cao nhất
    virtual std::vector<LeaderboardEntry> getLeaderboard() = 0;

    // Xếp hạng theo môn cụ thể
    virtual std::vector<LeaderboardEntry> getScoresBySubject(const std::string &subject) = 0;

    virtual SystemStats getSystemStats() = 0;
    virtual double getAverageScore(StudentId) = 0;
};

#endif // !_IREPORTSERVICE_H_