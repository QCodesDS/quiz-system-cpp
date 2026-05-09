#include "ExamRepository.h"
#include <algorithm>
#include <map>
#include <set>

// ============================================================
//  repositories/ExamRepository.cpp
// ============================================================

ExamRepository::ExamRepository(const std::string &examFile,
                               const std::string &questionsDir)
    : FileRepository(examFile),
      examFile(examFile),
      questionsDir(questionsDir) {}

// ------------------------------------------------------------
//  Helpers
// ------------------------------------------------------------

// "Math" → "math.txt", "CPP" → "cpp.txt"
std::string ExamRepository::subjectToFilename(const std::string &subject)
{
    std::string name;
    name.reserve(subject.size());
    for (char c : subject)
        name += static_cast<char>(std::tolower(c));
    return name + ".txt";
}

Exam ExamRepository::parseExam(const std::vector<std::string> &f) const
{
    // Format: ExamId|Subject|DurationMinutes|ExamType|TeacherId
    ExamId examId = std::stoi(f[0]);
    std::string subject = f[1];
    int duration = std::stoi(f[2]);
    ExamType type = (f[3] == "Official") ? ExamType::Official : ExamType::Practice;
    TeacherId tid = std::stoi(f[4]);
    return Exam(examId, subject, duration, type, tid);
}

Question ExamRepository::parseQuestion(const std::vector<std::string> &f) const
{
    // Format: QuestionId|ExamId|Content|ChoiceA|ChoiceB|ChoiceC|ChoiceD|CorrectChoice|Difficulty
    if (f.size() < 9)
        throw std::invalid_argument("bad question line");

    QuestionId qid = std::stoi(f[0]);
    ExamId examId = std::stoi(f[1]);
    int correct = std::stoi(f[7]);

    Difficulty diff = Difficulty::Easy;
    if (f[8] == "Medium")
        diff = Difficulty::Medium;
    else if (f[8] == "Hard")
        diff = Difficulty::Hard;

    return Question(qid, examId, f[2], f[3], f[4], f[5], f[6], correct, diff);
}

// ------------------------------------------------------------
//  Load
// ------------------------------------------------------------

std::vector<Exam> ExamRepository::loadExamHeaders() const
{
    std::vector<Exam> exams;
    for (const auto &line : readLines())
    { // readLines() đọc examFile
        auto f = splitLine(line);
        if (f.size() < 5)
            continue;
        try
        {
            exams.push_back(parseExam(f));
        }
        catch (...)
        { /* bỏ qua dòng lỗi */
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
        { /* bỏ qua dòng lỗi */
        }
    }
    return questions;
}

void ExamRepository::assembleQuestions(std::vector<Exam> &exams) const
{
    // Thu thập danh sách môn cần load
    std::set<std::string> subjects;
    for (const auto &e : exams)
        subjects.insert(e.getSubject());

    // Load questions từng file môn, group by examId
    std::map<ExamId, std::vector<Question>> qmap;
    for (const auto &subject : subjects)
    {
        std::string path = questionsDir + subjectToFilename(subject);
        for (const auto &q : loadQuestionsFromFile(path))
        {
            qmap[q.getExamId()].push_back(q);
        }
    }

    // Gắn vào đúng exam
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
//  Cache management
// ------------------------------------------------------------

void ExamRepository::ensureCache()
{
    if (cacheLoaded)
        return;
    cache = loadExamHeaders();
    assembleQuestions(cache);
    cacheLoaded = true;
}

void ExamRepository::invalidateCache()
{
    cache.clear();
    cacheLoaded = false;
}

// ------------------------------------------------------------
//  IExamRepository — public interface
// ------------------------------------------------------------

std::vector<Exam> ExamRepository::load()
{
    ensureCache();
    return cache;
}

bool ExamRepository::save(const std::vector<Exam> &exams)
{
    // 1. Ghi exams.txt (chỉ header)
    std::vector<std::string> examLines;
    examLines.reserve(exams.size());
    for (const auto &e : exams)
    {
        examLines.push_back(e.toFileString());
    }

    if (!safeWrite(examLines))
        return false; // examFile = this->filePath

    // 2. Gom questions theo môn → ghi từng file môn
    //    Dùng map<subject, vector<string>>
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
        std::string path = questionsDir + subjectToFilename(subject);
        if (!safeWriteTo(path, lines))
            return false;
    }

    invalidateCache();
    return true;
}

bool ExamRepository::backup()
{
    ensureCache();

    bool ok = backupFile(); // backup examFile

    // Backup từng file môn
    std::set<std::string> subjects;
    for (const auto &e : cache)
        subjects.insert(e.getSubject());
    for (const auto &s : subjects)
    {
        std::string path = questionsDir + subjectToFilename(s);
        ok &= backupFileTo(path);
    }

    return ok;
}

Exam *ExamRepository::findById(ExamId id)
{
    ensureCache();
    for (auto &e : cache)
    {
        if (e.getExamId() == id)
            return &e;
    }
    return nullptr;
}

std::vector<Exam> ExamRepository::findByTeacher(TeacherId teacherId)
{
    ensureCache();
    std::vector<Exam> result;
    for (const auto &e : cache)
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