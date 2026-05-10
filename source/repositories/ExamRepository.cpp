#include "ExamRepository.h"
#include <algorithm>
#include <cctype>
#include <map>
#include <set>
#include <stdexcept>

ExamRepository::ExamRepository(const std::string &examFile,
                               const std::string &questionsDir)
    : FileRepository(examFile),
      _examFile(examFile),
      _questionsDir(questionsDir)
{
}

// ------------------------------------------------------------
//  Parsing Helpers
// ------------------------------------------------------------

Exam ExamRepository::parseExam(const std::vector<std::string> &f) const
{
    // Định dạng: ExamId|Subject|DurationMinutes|ExamType|TeacherId
    ExamId id = std::stoi(f[0]);
    std::string s = f[1];
    int dur = std::stoi(f[2]);
    ExamType type = (f[3] == "Official") ? ExamType::Official : ExamType::Practice;
    TeacherId tid = std::stoi(f[4]);

    return Exam(id, s, dur, type, tid);
}

Question ExamRepository::parseQuestion(const std::vector<std::string> &f) const
{
    // Định dạng: QId|ExamId|Content|A|B|C|D|CorrectIdx|Difficulty
    if (f.size() < 9)
        throw std::invalid_argument("Dòng dữ liệu câu hỏi không hợp lệ");

    QuestionId qid = std::stoi(f[0]);
    ExamId eid = std::stoi(f[1]);
    int correct = std::stoi(f[7]);

    Difficulty diff = Difficulty::Easy;
    if (f[8] == "Medium")
        diff = Difficulty::Medium;
    else if (f[8] == "Hard")
        diff = Difficulty::Hard;

    return Question(qid, eid, f[2], f[3], f[4], f[5], f[6], correct, diff);
}

// ------------------------------------------------------------
//  File & Data Helpers
// ------------------------------------------------------------

std::string ExamRepository::subjectToFilename(const std::string &subject)
{
    std::string name;
    name.reserve(subject.size());
    for (char c : subject)
        name += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return name + ".txt";
}

std::vector<Exam> ExamRepository::loadExamHeaders() const
{
    std::vector<Exam> exams;
    // readLines() mặc định đọc từ _examFile (filePath của lớp cha)
    for (const auto &line : readLines())
    {
        auto f = splitLine(line);
        if (f.size() < 5)
            continue;
        try
        {
            exams.push_back(parseExam(f));
        }
        catch (...)
        { /* Skip malformed header */
        }
    }
    return exams;
}

std::vector<Question> ExamRepository::loadQuestionsFromFile(const std::string &path) const
{
    std::vector<Question> questions;
    for (const auto &line : readLinesFrom(path))
    {
        auto f = splitLine(line);
        try
        {
            questions.push_back(parseQuestion(f));
        }
        catch (...)
        { /* Skip malformed question */
        }
    }
    return questions;
}

void ExamRepository::assembleQuestions(std::vector<Exam> &exams) const
{
    if (exams.empty())
        return;

    // Bước 1: Xác định danh sách các môn học cần tải file
    std::set<std::string> subjects;
    for (const auto &e : exams)
        subjects.insert(e.getSubject());

    // Bước 2: Tải câu hỏi từ từng file môn học và nhóm theo ExamId
    std::map<ExamId, std::vector<Question>> qmap;
    for (const auto &s : subjects)
    {
        std::string path = _questionsDir + subjectToFilename(s);
        for (const auto &q : loadQuestionsFromFile(path))
        {
            qmap[q.getExamId()].push_back(q);
        }
    }

    // Bước 3: Gắn danh sách câu hỏi vào đúng đối tượng Exam
    for (auto &exam : exams)
    {
        auto it = qmap.find(exam.getExamId());
        if (it != qmap.end())
        {
            for (const auto &q : it->second)
            {
                exam.addQuestion(q);
            }
        }
    }
}

// ------------------------------------------------------------
//  Cache Management
// ------------------------------------------------------------

void ExamRepository::ensureCache()
{
    if (_cacheLoaded)
        return;

    _cache = loadExamHeaders();
    assembleQuestions(_cache);
    _cacheLoaded = true;
}

void ExamRepository::invalidateCache()
{
    _cache.clear();
    _cacheLoaded = false;
}

// ------------------------------------------------------------
//  Triển khai IExamRepository
// ------------------------------------------------------------

std::vector<Exam> ExamRepository::load()
{
    ensureCache();
    return _cache;
}

bool ExamRepository::save(const std::vector<Exam> &exams)
{
    // 1. Ghi tệp Header (exams.txt)
    std::vector<std::string> examLines;
    for (const auto &e : exams)
        examLines.push_back(e.toFileString());

    if (!safeWrite(examLines))
        return false;

    // 2. Phân loại câu hỏi theo môn học để ghi vào các file riêng lẻ
    std::map<std::string, std::vector<std::string>> qLines;
    for (const auto &e : exams)
    {
        for (const auto &q : e.getQuestions())
        {
            qLines[e.getSubject()].push_back(q.toFileString());
        }
    }

    for (const auto &[subject, lines] : qLines)
    {
        std::string path = _questionsDir + subjectToFilename(subject);
        if (!safeWriteTo(path, lines))
            return false;
    }

    invalidateCache();
    return true;
}

bool ExamRepository::backup()
{
    ensureCache();
    bool ok = backupFile(); // Backup exam header file

    std::set<std::string> subjects;
    for (const auto &e : _cache)
        subjects.insert(e.getSubject());

    for (const auto &s : subjects)
    {
        std::string path = _questionsDir + subjectToFilename(s);
        ok &= backupFileTo(path);
    }
    return ok;
}

Exam *ExamRepository::findById(ExamId id)
{
    ensureCache();
    auto it = std::find_if(_cache.begin(), _cache.end(),
                           [id](const Exam &e)
                           { return e.getExamId() == id; });

    return (it != _cache.end()) ? &(*it) : nullptr;
}

std::vector<Exam> ExamRepository::findByTeacher(TeacherId teacherId)
{
    ensureCache();
    std::vector<Exam> result;
    for (const auto &e : _cache)
    {
        if (e.getTeacherId() == teacherId)
            result.push_back(e);
    }
    return result;
}

bool ExamRepository::examIdExists(ExamId id)
{
    return findById(id) != nullptr;
}