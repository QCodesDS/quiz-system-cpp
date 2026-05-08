#ifndef _QUESTION_H_
#define _QUESTION_H_

// ============================================================
//  core/models/Question.h
//  Câu hỏi trắc nghiệm 4 lựa chọn
//
//  correctChoice: 1=A, 2=B, 3=C, 4=D (khớp với database-format)
// ============================================================

#include <string>
#include "../types/enums.h"
#include "../types/typedefs.h"

class Question
{
private:
    QuestionId _id;
    ExamId _examId;
    std::string _content;
    std::string _choices[4]; // index 0=A, 1=B, 2=C, 3=D
    int _correctChoice;      // 1–4
    Difficulty _difficulty;

public:
    Question(QuestionId id, ExamId examId,
             const std::string &content,
             const std::string &choiceA, const std::string &choiceB,
             const std::string &choiceC, const std::string &choiceD,
             int correctChoice, Difficulty difficulty);

    QuestionId getId() const;
    ExamId getExamId() const;
    std::string getContent() const;
    std::string getChoice(int i) const; // i: 0–3, A=0, B=1, C=2, D=3
    int getCorrectChoice() const;
    Difficulty getDifficulty() const;

    std::string toFileString() const;
};

#endif // !_QUESTION_H_