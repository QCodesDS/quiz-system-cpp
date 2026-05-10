#ifndef QUESTION_H
#define QUESTION_H

#include <string>
#include "enums.h"
#include "typedefs.h"

/**
 * @class Question
 * @brief Đại diện cho một câu hỏi trắc nghiệm trong đề thi.
 *
 * Lớp này quản lý nội dung câu hỏi, 4 phương án lựa chọn (A, B, C, D)
 * và đáp án đúng theo quy ước số nguyên (1-4).
 */
class Question
{
private:
    QuestionId _id;          ///< ID duy nhất của câu hỏi.
    ExamId _examId;          ///< ID của đề thi mà câu hỏi này thuộc về.
    std::string _content;    ///< Nội dung câu hỏi.
    std::string _choices[4]; ///< Mảng chứa 4 lựa chọn (0:A, 1:B, 2:C, 3:D).
    int _correctChoice;      ///< Đáp án đúng (quy ước: 1=A, 2=B, 3=C, 4=D).
    Difficulty _difficulty;  ///< Độ khó của câu hỏi.

public:
    /**
     * @brief Khởi tạo một đối tượng Question.
     *
     * @param id ID câu hỏi.
     * @param examId ID đề thi liên kết.
     * @param content Nội dung văn bản câu hỏi.
     * @param choiceA Lựa chọn A.
     * @param choiceB Lựa chọn B.
     * @param choiceC Lựa chọn C.
     * @param choiceD Lựa chọn D.
     * @param correctChoice Chỉ số đáp án đúng (1-4).
     * @param difficulty Độ khó (Easy/Medium/Hard).
     */
    Question(QuestionId id, ExamId examId,
             const std::string &content,
             const std::string &choiceA, const std::string &choiceB,
             const std::string &choiceC, const std::string &choiceD,
             int correctChoice, Difficulty difficulty);

    // ------------------------------------------------------------
    //  Getters
    // ------------------------------------------------------------

    QuestionId getId() const;
    ExamId getExamId() const;
    std::string getContent() const;
    int getCorrectChoice() const;
    Difficulty getDifficulty() const;

    /**
     * @brief Lấy nội dung của một lựa chọn dựa trên chỉ số.
     * @param i Chỉ số lựa chọn (0-3 ứng với A-D).
     * @return std::string — Nội dung lựa chọn hoặc chuỗi rỗng nếu chỉ số không hợp lệ.
     */
    std::string getChoice(int i) const;

    /**
     * @brief Chuyển đổi dữ liệu câu hỏi thành chuỗi định dạng lưu file.
     * @return std::string — Định dạng: "QId|EId|Content|A|B|C|D|Correct|Diff".
     */
    std::string toFileString() const;
};

#endif // QUESTION_H