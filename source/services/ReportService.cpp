#include "ReportService.h"
#include <algorithm>
#include <map>
#include <set>
#include <numeric>
#include <memory>

// ============================================================
//  services/ReportService.cpp
// ============================================================

ReportService::ReportService(IUserRepository *userRepo,
                             IResultRepository *resultRepo)
    : userRepo(userRepo), resultRepo(resultRepo) {}

// ------------------------------------------------------------
//  getLeaderboard
//  Với mỗi học sinh: tính điểm trung bình của tất cả lần thi.
//  Sort giảm dần theo điểm trung bình.
// ------------------------------------------------------------
std::vector<LeaderboardEntry> ReportService::getLeaderboard()
{
    auto results = resultRepo->load();

    // Group by studentId
    std::map<StudentId, std::vector<double>> scoreMap;
    std::map<StudentId, std::string> nameMap;

    for (const auto &r : results)
    {
        scoreMap[r.getStudentId()].push_back(r.getScore());
        nameMap[r.getStudentId()] = r.getStudentName();
    }

    std::vector<LeaderboardEntry> board;
    board.reserve(scoreMap.size());

    for (const auto &[sid, scores] : scoreMap)
    {
        double avg = std::accumulate(scores.begin(), scores.end(), 0.0) / static_cast<double>(scores.size());

        LeaderboardEntry entry;
        entry.studentId = sid;
        entry.studentName = nameMap[sid];
        entry.subject = ""; // toàn trường — không filter môn
        entry.bestScore = avg;
        entry.attemptCount = static_cast<int>(scores.size());
        board.push_back(entry);
    }

    std::sort(board.begin(), board.end(),
              [](const LeaderboardEntry &a, const LeaderboardEntry &b)
              {
                  return a.bestScore > b.bestScore;
              });

    return board;
}

// ------------------------------------------------------------
//  getScoresBySubject
//  Với mỗi học sinh trong môn đó: lấy điểm cao nhất.
//  Sort giảm dần theo bestScore.
// ------------------------------------------------------------
std::vector<LeaderboardEntry> ReportService::getScoresBySubject(
    const std::string &subject)
{

    auto results = resultRepo->load();

    // Filter theo môn
    std::map<StudentId, LeaderboardEntry> entryMap;

    for (const auto &r : results)
    {
        if (r.getSubject() != subject)
            continue;

        StudentId sid = r.getStudentId();
        if (entryMap.find(sid) == entryMap.end())
        {
            LeaderboardEntry e;
            e.studentId = sid;
            e.studentName = r.getStudentName();
            e.subject = subject;
            e.bestScore = r.getScore();
            e.attemptCount = 1;
            entryMap[sid] = e;
        }
        else
        {
            auto &e = entryMap[sid];
            if (r.getScore() > e.bestScore)
                e.bestScore = r.getScore();
            ++e.attemptCount;
        }
    }

    std::vector<LeaderboardEntry> board;
    board.reserve(entryMap.size());
    for (const auto &[_, e] : entryMap)
        board.push_back(e);

    std::sort(board.begin(), board.end(),
              [](const LeaderboardEntry &a, const LeaderboardEntry &b)
              {
                  return a.bestScore > b.bestScore;
              });

    return board;
}

// ------------------------------------------------------------
//  getSystemStats
// ------------------------------------------------------------
SystemStats ReportService::getSystemStats()
{
    SystemStats stats{};

    // Đếm user
    auto users = userRepo->load();
    for (const auto &u : users)
    {
        if (!u)
            continue;
        if (u->getRole() == "Student")
            ++stats.totalStudents;
        if (u->getRole() == "Teacher")
            ++stats.totalTeachers;
    }
    // users vector auto-cleans up

    // Thống kê kết quả
    auto results = resultRepo->load();
    stats.totalAttempts = static_cast<int>(results.size());

    // Đếm số đề thi unique
    std::set<ExamId> examIds;
    double totalScore = 0.0;
    for (const auto &r : results)
    {
        examIds.insert(r.getExamId());
        totalScore += r.getScore();
    }
    stats.totalExams = static_cast<int>(examIds.size());
    stats.averageScore = results.empty()
                             ? 0.0
                             : totalScore / static_cast<double>(results.size());

    return stats;
}

// ------------------------------------------------------------
//  getAverageScore
// ------------------------------------------------------------
double ReportService::getAverageScore(StudentId sid)
{
    auto results = resultRepo->findByStudent(sid);
    if (results.empty())
        return 0.0;

    double total = 0.0;
    for (const auto &r : results)
        total += r.getScore();
    return total / static_cast<double>(results.size());
}