#ifndef EXAM_REPOSITORY_H
#define EXAM_REPOSITORY_H

// ============================================================
//  repositories/ExamRepository.h
//
//  Load từ 2 nguồn:
//    - exams.txt        : header của đề thi (ExamId, subject, ...)
//    - questions/<môn>.txt : câu hỏi, phân theo môn
//
//  Sau khi load xong, assemble: gắn Question vào đúng Exam.
//  Save cũng tách tương tự: ghi exams.txt + từng file môn.
// ============================================================

#include "FileRepository.h"
#include "IExamRepository.h"

class ExamRepository : public FileRepository, public IExamRepository
{
private:
    std::string examFile;     // data/exams.txt
    std::string questionsDir; // data/questions/

    // Parse một dòng exams.txt → Exam (chưa có questions)
    Exam parseExam(const std::vector<std::string> &fields) const;

    // Parse một dòng questions/<môn>.txt → Question
    Question parseQuestion(const std::vector<std::string> &fields) const;

    // Chuyển tên môn → tên file, ví dụ "Math" → "math.txt"
    static std::string subjectToFilename(const std::string &subject);

    // Load tất cả câu hỏi từ một file môn
    std::vector<Question> loadQuestionsFromFile(const std::string &path) const;

    // Load toàn bộ exams (chưa gắn questions)
    std::vector<Exam> loadExamHeaders() const;

    // Gắn questions vào đúng exam (by examId)
    void assembleQuestions(std::vector<Exam> &exams) const;

    // Cache in-memory sau khi load — tránh đọc file mỗi lần find
    std::vector<Exam> cache;
    bool cacheLoaded = false;

    void ensureCache();
    void invalidateCache();

public:
    ExamRepository(const std::string &examFile, const std::string &questionsDir);

    // --- IExamRepository ---
    bool save(const std::vector<Exam> &exams) override;
    std::vector<Exam> load() override;
    bool backup() override;

    Exam *findById(ExamId id) override;
    std::vector<Exam> findByTeacher(TeacherId teacherId) override;
    bool examIdExists(ExamId id) override;
};

#endif // EXAM_REPOSITORY_H