#include "services/ExamService.h"
#include <algorithm>

ExamService::ExamService(IExamRepository *examRepo)
    : _examRepo(examRepo)
{
}

// ------------------------------------------------------------
// ID Generation
// Logic tìm ID lớn nhất hiện tại để tránh trùng lặp
// ------------------------------------------------------------

ExamId ExamService::nextExamId() const
{
    auto all = _examRepo->load();
    ExamId maxId = 0;
    for (const auto &e : all)
    {
        if (e.getExamId() > maxId)
            maxId = e.getExamId();
    }
    return maxId + 1;
}

QuestionId ExamService::nextQuestionId() const
{
    auto all = _examRepo->load();
    QuestionId maxId = 100; // Khởi tạo từ 100 để ID câu hỏi nhìn khác biệt với ID đề
    for (const auto &e : all)
    {
        for (const auto &q : e.getQuestions())
        {
            if (q.getId() > maxId)
                maxId = q.getId();
        }
    }
    return maxId + 1;
}

// ------------------------------------------------------------
// Exam CRUD Logic
// ------------------------------------------------------------

bool ExamService::addExam(const std::string &subject, int durationMinutes,
                          ExamType type, TeacherId teacherId)
{
    // Validation cơ bản
    if (subject.empty() || durationMinutes <= 0)
        return false;

    auto all = _examRepo->load();

    // Tạo đề thi mới và gán ID hợp lệ
    all.emplace_back(nextExamId(), subject, durationMinutes, type, teacherId);

    return _examRepo->save(all);
}

bool ExamService::removeExam(ExamId examId)
{
    auto all = _examRepo->load();

    auto it = std::find_if(all.begin(), all.end(),
                           [examId](const Exam &e)
                           { return e.getExamId() == examId; });

    if (it == all.end())
        return false;

    all.erase(it);
    return _examRepo->save(all);
}

// ------------------------------------------------------------
// Question Management Logic
// ------------------------------------------------------------

bool ExamService::addQuestion(ExamId examId, const Question &q)
{
    auto all = _examRepo->load();

    for (auto &e : all)
    {
        if (e.getExamId() == examId)
        {
            // Gán ID mới cho câu hỏi trước khi thêm vào đề
            Question assigned(nextQuestionId(), examId,
                              q.getContent(),
                              q.getChoice(0), q.getChoice(1),
                              q.getChoice(2), q.getChoice(3),
                              q.getCorrectChoice(), q.getDifficulty());

            e.addQuestion(assigned);
            return _examRepo->save(all);
        }
    }
    return false;
}

bool ExamService::updateQuestion(ExamId examId, QuestionId qId,
                                 const Question &newQ)
{
    auto all = _examRepo->load();

    for (auto &e : all)
    {
        if (e.getExamId() == examId)
        {
            // Xóa câu hỏi cũ dựa trên qId
            if (!e.removeQuestion(qId))
                return false;

            // Tạo đối tượng cập nhật với qId giữ nguyên
            Question updated(qId, examId,
                             newQ.getContent(),
                             newQ.getChoice(0), newQ.getChoice(1),
                             newQ.getChoice(2), newQ.getChoice(3),
                             newQ.getCorrectChoice(), newQ.getDifficulty());

            e.addQuestion(updated);
            return _examRepo->save(all);
        }
    }
    return false;
}

bool ExamService::removeQuestion(ExamId examId, QuestionId qId)
{
    auto all = _examRepo->load();

    for (auto &e : all)
    {
        if (e.getExamId() == examId)
        {
            if (!e.removeQuestion(qId))
                return false;
            return _examRepo->save(all);
        }
    }
    return false;
}

Exam *ExamService::findExam(ExamId examId)
{
    return _examRepo->findById(examId);
}

bool ExamService::hasEditPermission(const Exam &exam, TeacherId tid)
{
    // Đề thi chỉ được chỉnh sửa bởi chính người tạo ra nó
    return exam.getTeacherId() == tid;
}