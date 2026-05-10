#include "repositories/ResultRepository.h"
#include <stdexcept>

ResultRepository::ResultRepository(const std::string &resultFile)
    : FileRepository(resultFile)
{
}

// ------------------------------------------------------------
//  Parse Logic
//  Format: StudentId|StudentName|ExamId|Subject|Score|DurationSeconds|ExamType|SubmittedAt
// ------------------------------------------------------------

ExamResult ResultRepository::parseLine(const std::vector<std::string> &f) const
{
    if (f.size() < 8)
        throw std::invalid_argument("Dữ liệu kết quả không đủ trường thông tin");

    try
    {
        StudentId sid = std::stoi(f[0]);
        std::string name = f[1];
        ExamId eid = std::stoi(f[2]);
        std::string sub = f[3];
        double score = std::stod(f[4]);
        int duration = std::stoi(f[5]);
        ExamType type = (f[6] == "Official") ? ExamType::Official : ExamType::Practice;
        std::string time = f[7];

        return ExamResult(sid, name, eid, sub, score, duration, type, time);
    }
    catch (const std::exception &e)
    {
        throw std::invalid_argument("Lỗi định dạng dữ liệu: " + std::string(e.what()));
    }
}

// ------------------------------------------------------------
//  Triển khai IResultRepository
// ------------------------------------------------------------

std::vector<ExamResult> ResultRepository::load()
{
    std::vector<ExamResult> results;
    // readLines() lọc bỏ dòng trống và comment tự động từ lớp cha
    for (const auto &line : readLines())
    {
        auto f = splitLine(line);
        try
        {
            results.push_back(parseLine(f));
        }
        catch (...)
        {
            // Log hoặc bỏ qua các dòng lỗi để không làm gián đoạn việc đọc file
        }
    }
    return results;
}

bool ResultRepository::append(const ExamResult &r)
{
    // Sử dụng cơ chế append duy nhất từ FileRepository
    return appendLine(r.toFileString());
}

bool ResultRepository::backup()
{
    return backupFile();
}

std::vector<ExamResult> ResultRepository::findByStudent(StudentId sid)
{
    std::vector<ExamResult> results;
    // Đọc tuần tự (Sequential scan) vì file kết quả thường ghi theo trình tự thời gian
    for (const auto &res : load())
    {
        if (res.getStudentId() == sid)
        {
            results.push_back(res);
        }
    }
    return results;
}