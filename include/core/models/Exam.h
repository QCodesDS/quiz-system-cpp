#ifndef EXAM_H
#define EXAM_H

#include "Question.h"

/**
 * @class Exam
 * @brief Đại diện cho một đề thi trong hệ thống.
 *
 * Lớp này quản lý thông tin chung của đề thi và danh sách các câu hỏi đi kèm.
 * @note Exam không tự đảm nhận việc truy xuất dữ liệu. Việc tạo lập và
 *       liên kết dữ liệu giữa Exam và Questions do ExamRepository đảm nhận.
 */
class Exam
{
private:
    ExamId _examId;                   ///< ID duy nhất của đề thi.
    std::string _subject;             ///< Tên môn học/chủ đề.
    int _durationMinutes;             ///< Thời gian làm bài (phút).
    ExamType _examType;               ///< Loại đề thi (Official/Practice).
    TeacherId _teacherId;             ///< ID giáo viên tạo đề.
    std::vector<Question> _questions; ///< Danh sách các câu hỏi thuộc đề thi.

public:
    /**
     * @brief Khởi tạo một đối tượng Exam.
     * @param examId ID của đề thi.
     * @param subject Tên môn học.
     * @param durationMinutes Thời gian làm bài.
     * @param examType Loại đề thi.
     * @param teacherId ID giáo viên sở hữu.
     */
    Exam(ExamId examId, const std::string &subject,
         int durationMinutes, ExamType examType, TeacherId teacherId);

    // ------------------------------------------------------------
    //  Getters
    // ------------------------------------------------------------

    ExamId getExamId() const;
    std::string getSubject() const;
    int getDurationMinutes() const;
    ExamType getType() const;
    TeacherId getTeacherId() const;
    int getQuestionCount() const;

    /**
     * @brief Truy cập danh sách câu hỏi.
     * @return const std::vector<Question>& — Tham chiếu hằng tới danh sách câu hỏi.
     */
    const std::vector<Question> &getQuestions() const;

    // ------------------------------------------------------------
    //  Modifiers
    // ------------------------------------------------------------

    /**
     * @brief Thêm một câu hỏi vào đề thi.
     * @param q Đối tượng Question cần thêm.
     * @return true nếu thêm thành công.
     */
    bool addQuestion(const Question &q);

    /**
     * @brief Xóa câu hỏi khỏi đề thi dựa trên ID.
     * @param qId ID của câu hỏi cần xóa.
     * @return true nếu xóa thành công, false nếu không tìm thấy ID.
     */
    bool removeQuestion(QuestionId qId);

    /**
     * @brief Chuyển đổi thông tin header đề thi thành chuỗi lưu file.
     * @note Không bao gồm nội dung chi tiết của các câu hỏi.
     * @return std::string — Định dạng: "ExamId|Subject|Duration|Type|TeacherId".
     */
    std::string toFileString() const;
};

#endif // EXAM_H