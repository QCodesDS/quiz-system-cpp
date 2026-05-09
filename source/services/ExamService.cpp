#include "ExamService.h"
#include <algorithm>

// ============================================================
//  services/ExamService.cpp
// ============================================================

ExamService::ExamService(IExamRepository *examRepo)
    : examRepo(examRepo) {}

// ------------------------------------------------------------
//  ID generation
// ------------------------------------------------------------

ExamId ExamService::nextExamId() const
{
    auto all = examRepo->load();
    ExamId maxId = 0;
    for (const auto &e : all)
        if (e.getExamId() > maxId)
            maxId = e.getExamId();
    return maxId + 1;
}

QuestionId ExamService::nextQuestionId() const
{
    auto all = examRepo->load();
    QuestionId maxId = 100;
    for (const auto &e : all)
        for (const auto &q : e.getQuestions())
            if (q.getId() > maxId)
                maxId = q.getId();
    return maxId + 1;
}

// ------------------------------------------------------------
//  IExamService
// ------------------------------------------------------------

bool ExamService::addExam(const std::string &subject, int durationMinutes,
                          ExamType type, TeacherId teacherId)
{
    if (subject.empty())
        return false;
    if (durationMinutes <= 0)
        return false;

    auto all = examRepo->load();
    all.emplace_back(nextExamId(), subject, durationMinutes, type, teacherId);
    return examRepo->save(all);
}

bool ExamService::removeExam(ExamId examId)
{
    auto all = examRepo->load();

    auto it = std::find_if(all.begin(), all.end(),
                           [examId](const Exam &e)
                           { return e.getExamId() == examId; });

    if (it == all.end())
        return false;
    all.erase(it);
    return examRepo->save(all);
}

bool ExamService::addQuestion(ExamId examId, const Question &q)
{
    auto all = examRepo->load();

    for (auto &e : all)
    {
        if (e.getExamId() != examId)
            continue;
        // Tạo question mới với ID được gán tự động
        // (q truyền vào có thể chưa có ID đúng — ExamService assign)
        Question assigned(nextQuestionId(), examId,
                          q.getContent(),
                          q.getChoice(0), q.getChoice(1),
                          q.getChoice(2), q.getChoice(3),
                          q.getCorrectChoice(), q.getDifficulty());
        e.addQuestion(assigned);
        return examRepo->save(all);
    }

    return false; // examId không tồn tại
}

bool ExamService::updateQuestion(ExamId examId, QuestionId qId,
                                 const Question &newQ)
{
    auto all = examRepo->load();

    for (auto &e : all)
    {
        if (e.getExamId() != examId)
            continue;

        // Remove cũ, thêm mới với cùng qId
        if (!e.removeQuestion(qId))
            return false;

        Question updated(qId, examId,
                         newQ.getContent(),
                         newQ.getChoice(0), newQ.getChoice(1),
                         newQ.getChoice(2), newQ.getChoice(3),
                         newQ.getCorrectChoice(), newQ.getDifficulty());
        e.addQuestion(updated);
        return examRepo->save(all);
    }

    return false;
}

bool ExamService::removeQuestion(ExamId examId, QuestionId qId)
{
    auto all = examRepo->load();

    for (auto &e : all)
    {
        if (e.getExamId() != examId)
            continue;
        if (!e.removeQuestion(qId))
            return false;
        return examRepo->save(all);
    }

    return false;
}

Exam *ExamService::findExam(ExamId examId)
{
    return examRepo->findById(examId);
}

bool ExamService::hasEditPermission(const Exam &exam, TeacherId tid)
{
    return exam.getTeacherId() == tid;
}