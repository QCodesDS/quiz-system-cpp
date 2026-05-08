#include "Question.h"

static std::string difficultyToString(Difficulty d)
{
    switch (d)
    {
    case Difficulty::Easy:
        return "Easy";
    case Difficulty::Medium:
        return "Medium";
    default:
        return "Hard";
    }
}

Question::Question(QuestionId id, ExamId examId,
                   const std::string &content,
                   const std::string &choiceA, const std::string &choiceB,
                   const std::string &choiceC, const std::string &choiceD,
                   int correctChoice, Difficulty difficulty)
    : _id(id), _examId(examId), _content(content),
      _correctChoice(correctChoice), _difficulty(difficulty)
{
    _choices[0] = choiceA;
    _choices[1] = choiceB;
    _choices[2] = choiceC;
    _choices[3] = choiceD;
}

QuestionId Question::getId() const { return _id; }
ExamId Question::getExamId() const { return _examId; }
std::string Question::getContent() const { return _content; }
int Question::getCorrectChoice() const { return _correctChoice; }
Difficulty Question::getDifficulty() const { return _difficulty; }

std::string Question::getChoice(int i) const
{
    if (i < 0 || i > 3)
        return "";
    return _choices[i];
}

std::string Question::toFileString() const
{
    // Format: QuestionId|ExamId|Content|ChoiceA|ChoiceB|ChoiceC|ChoiceD|CorrectChoice|Difficulty
    return std::to_string(_id) + "|" + std::to_string(_examId) + "|" + _content + "|" + _choices[0] + "|" + _choices[1] + "|" + _choices[2] + "|" + _choices[3] + "|" + std::to_string(_correctChoice) + "|" + difficultyToString(_difficulty);
}