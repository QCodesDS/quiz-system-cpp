#include "ResultRepository.h"

ResultRepository::ResultRepository(const std::string &resultFile)
    : FileRepository(resultFile) {}

// ------------------------------------------------------------
//  Parse
//  Format: StudentId|StudentName|ExamId|Subject|Score|DurationSeconds|ExamType|SubmittedAt
// ------------------------------------------------------------

ExamResult ResultRepository::parseLine(const std::vector<std::string> &f) const
{
    if (f.size() < 8)
        throw std::invalid_argument("bad result line");

    StudentId sid = std::stoi(f[0]);
    std::string name = f[1];
    ExamId examId = std::stoi(f[2]);
    std::string subject = f[3];
    double score = std::stod(f[4]);
    int duration = std::stoi(f[5]);
    ExamType type = (f[6] == "Official") ? ExamType::Official : ExamType::Practice;
    std::string time = f[7];

    return ExamResult(sid, name, examId, subject, score, duration, type, time);
}

// ------------------------------------------------------------
//  IResultRepository
// ------------------------------------------------------------

std::vector<ExamResult> ResultRepository::load()
{
    std::vector<ExamResult> results;
    for (const auto &line : readLines())
    {
        auto f = splitLine(line);
        try
        {
            results.push_back(parseLine(f));
        }
        catch (...)
        { /* bỏ qua dòng lỗi */
        }
    }
    return results;
}

bool ResultRepository::append(const ExamResult &r)
{
    // appendLine() từ FileRepository — không rewrite, chỉ thêm cuối file
    return appendLine(r.toFileString());
}

bool ResultRepository::backup()
{
    return backupFile();
}

std::vector<ExamResult> ResultRepository::findByStudent(StudentId sid)
{
    std::vector<ExamResult> results;
    for (const auto &r : load())
    {
        if (r.getStudentId() == sid)
            results.push_back(r);
    }
    return results;
}