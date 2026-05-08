#ifndef _EXAMRESULTH_
#define _EXAMRESULTH_

// ============================================================
//  core/models/ExamResult.h
//  Bản ghi một lần thi — append vào results.txt, không bao giờ sửa
//
//  studentName và subject là snapshot tại thời điểm thi,
//  đề phòng sau này đổi tên user/môn học.
// ============================================================

#include <string>
#include "types/enums.h"
#include "types/typedefs.h"

class ExamResult
{
private:
    StudentId _studentId;
    std::string _studentName; // snapshot
    ExamId _examId;
    std::string _subject; // snapshot
    double _score;        // thang điểm 10
    int _durationSeconds;
    ExamType _examType;
    std::string _submittedAt; // "YYYY-MM-DD HH:MM:SS"

public:
    ExamResult(StudentId studentId, const std::string &studentName,
               ExamId examId, const std::string &subject,
               double score, int durationSeconds,
               ExamType examType, const std::string &submittedAt);

    // --- Getters ---
    StudentId getStudentId() const;
    std::string getStudentName() const;
    ExamId getExamId() const;
    std::string getSubject() const;
    double getScore() const;
    int getDurationSeconds() const;
    ExamType getExamType() const;
    std::string getSubmittedAt() const;

    bool isPassed() const; // score >= PASS_SCORE (5.0)

    std::string toFileString() const;
};

#endif // !_EXAMRESULTH_