#include "services/ReportService.h"
#include <algorithm>
#include <map>
#include <numeric>
#include <set>

ReportService::ReportService(IUserRepository *userRepo,
                             IResultRepository *resultRepo)
    : _userRepo(userRepo), _resultRepo(resultRepo)
{
}

// ------------------------------------------------------------
//  Bảng xếp hạng toàn trường (Tính theo điểm trung bình)
// ------------------------------------------------------------
std::vector<LeaderboardEntry> ReportService::getLeaderboard()
{
    auto results = _resultRepo->load();
    if (results.empty())
        return {};

    std::map<StudentId, std::vector<double>> scoreMap;
    std::map<StudentId, std::string> nameMap;

    for (const auto &r : results)
    {
        scoreMap[r.getStudentId()].push_back(r.getScore());
        nameMap[r.getStudentId()] = r.getStudentName();
    }

    std::vector<LeaderboardEntry> board;
    for (const auto &[sid, scores] : scoreMap)
    {
        double sum = std::accumulate(scores.begin(), scores.end(), 0.0);
        double avg = sum / static_cast<double>(scores.size());

        LeaderboardEntry entry;
        entry.studentId = sid;
        entry.studentName = nameMap[sid];
        entry.subject = "All Subjects";
        entry.bestScore = avg; // Ở đây dùng làm điểm đại diện để sort
        entry.attemptCount = static_cast<int>(scores.size());
        board.push_back(entry);
    }

    // Sắp xếp giảm dần theo điểm trung bình
    std::sort(board.begin(), board.end(),
              [](const LeaderboardEntry &a, const LeaderboardEntry &b)
              {
                  return a.bestScore > b.bestScore;
              });

    return board;
}

// ------------------------------------------------------------
//  Bảng xếp hạng theo môn (Tính theo điểm cao nhất)
// ------------------------------------------------------------
std::vector<LeaderboardEntry> ReportService::getScoresBySubject(const std::string &subject)
{
    auto results = _resultRepo->load();
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
            e.attemptCount++;
        }
    }

    std::vector<LeaderboardEntry> board;
    for (const auto &[_, entry] : entryMap)
        board.push_back(entry);

    std::sort(board.begin(), board.end(),
              [](const LeaderboardEntry &a, const LeaderboardEntry &b)
              {
                  return a.bestScore > b.bestScore;
              });

    return board;
}

// ------------------------------------------------------------
//  Thống kê hệ thống (Dành cho Admin)
// ------------------------------------------------------------
SystemStats ReportService::getSystemStats()
{
    SystemStats stats{};

    // Thống kê người dùng
    auto users = _userRepo->load();
    for (const auto &u : users)
    {
        if (!u)
            continue;
        if (u->getRole() == "Student")
            stats.totalStudents++;
        else if (u->getRole() == "Teacher")
            stats.totalTeachers++;
    }

    // Thống kê kết quả thi
    auto results = _resultRepo->load();
    stats.totalAttempts = static_cast<int>(results.size());

    std::set<ExamId> uniqueExams;
    double totalScoreSum = 0.0;

    for (const auto &r : results)
    {
        uniqueExams.insert(r.getExamId());
        totalScoreSum += r.getScore();
    }

    stats.totalExams = static_cast<int>(uniqueExams.size());
    stats.averageScore = results.empty() ? 0.0 : totalScoreSum / results.size();

    return stats;
}

double ReportService::getAverageScore(StudentId sid)
{
    auto results = _resultRepo->findByStudent(sid);
    if (results.empty())
        return 0.0;

    double sum = 0.0;
    for (const auto &r : results)
        sum += r.getScore();
    return sum / results.size();
}