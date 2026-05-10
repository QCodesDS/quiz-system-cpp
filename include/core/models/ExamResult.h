#ifndef EXAM_RESULT_H
#define EXAM_RESULT_H

#include "enums.h"
#include "typedefs.h"

/**
 * @class ExamResult
 * @brief Đại diện cho kết quả của một lượt thi duy nhất.
 *
 * Lớp này hoạt động như một bản ghi lịch sử (Audit Log). Dữ liệu sau khi ghi
 * sẽ được append vào file và không được phép chỉnh sửa để đảm bảo tính minh bạch.
 *
 * @note Các trường thông tin như tên sinh viên và môn học được lưu dưới dạng "snapshot"
 *       (bản sao tại thời điểm thi) để tránh dữ liệu lịch sử bị thay đổi khi
 *       thông tin gốc trong hệ thống cập nhật.
 */
class ExamResult
{
private:
    StudentId _studentId;     ///< ID sinh viên thực hiện bài thi.
    std::string _studentName; ///< Tên sinh viên (snapshot).
    ExamId _examId;           ///< ID đề thi liên kết.
    std::string _subject;     ///< Tên môn học (snapshot).
    double _score;            ///< Điểm số đạt được (thang điểm 10).
    int _durationSeconds;     ///< Thời gian hoàn thành bài thi (giây).
    ExamType _examType;       ///< Loại đề thi (Official/Practice).
    std::string _submittedAt; ///< Thời điểm nộp bài (YYYY-MM-DD HH:MM:SS).

public:
    /**
     * @brief Khởi tạo bản ghi kết quả bài thi.
     */
    ExamResult(StudentId studentId, const std::string &studentName,
               ExamId examId, const std::string &subject,
               double score, int durationSeconds,
               ExamType examType, const std::string &submittedAt);

    // ------------------------------------------------------------
    //  Getters
    // ------------------------------------------------------------

    StudentId getStudentId() const;
    std::string getStudentName() const;
    ExamId getExamId() const;
    std::string getSubject() const;
    double getScore() const;
    int getDurationSeconds() const;
    ExamType getExamType() const;
    std::string getSubmittedAt() const;

    /**
     * @brief Kiểm tra bài thi có đạt mức điểm sàn hay không.
     * @return true nếu score >= 5.0.
     */
    bool isPassed() const;

    /**
     * @brief Chuyển đổi kết quả thành chuỗi định dạng lưu file.
     * @return std::string — Định dạng: "SId|SName|EId|Subj|Score|Dur|Type|Time".
     */
    std::string toFileString() const;
};

#endif // EXAM_RESULT_H