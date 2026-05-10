#ifndef I_QUIZ_SERVICE_H
#define I_QUIZ_SERVICE_H

#include "Exam.h"
#include "ExamResult.h"

/**
 * @brief Interface xử lý logic làm bài thi.
 *
 * @details Định nghĩa contract cho toàn bộ flow một lượt thi:
 * kiểm tra điều kiện → tính điểm → tạo kết quả → xác nhận đạt/rớt.
 *
 * @note Flow chuẩn khi học sinh nộp bài:
 *   1. canAttempt()      — kiểm tra điều kiện
 *   2. calculateScore()  — tính điểm từ đáp án
 *   3. createResult()    — tạo và lưu kết quả
 *   4. isPassed()        — thông báo kết quả
 */
class IQuizService
{
public:
    virtual ~IQuizService() = default;

    /**
     * @brief Kiểm tra học sinh có được phép làm bài thi này không.
     *
     * @details Quy tắc:
     * - Official: chỉ được làm một lần duy nhất.
     * - Practice: luôn được phép làm, không giới hạn số lần.
     *
     * @param studentId ID học sinh.
     * @param examId    ID đề thi.
     * @return true nếu học sinh được phép làm bài.
     */
    virtual bool canAttempt(StudentId studentId, ExamId examId) = 0;

    /**
     * @brief Tính điểm bài thi dựa trên đáp án học sinh.
     *
     * @param exam    Đề thi chứa đáp án đúng và tổng số câu.
     * @param answers Danh sách đáp án học sinh chọn (index 0..n-1,
     *                giá trị 1–4 tương ứng lựa chọn A/B/C/D).
     * @return Điểm số thang 10.0.
     */
    virtual double calculateScore(const Exam &exam,
                                  const std::vector<int> &answers) = 0;

    /**
     * @brief Tạo và lưu kết quả thi vào storage.
     *
     * @param studentId       ID học sinh.
     * @param studentName     Tên học sinh (lưu vào kết quả để tránh join).
     * @param exam            Đề thi vừa làm.
     * @param score           Điểm đã tính từ calculateScore().
     * @param durationSeconds Thời gian làm bài thực tế tính bằng giây.
     * @return ExamResult đã được lưu vào storage.
     */
    virtual ExamResult createResult(StudentId studentId,
                                    const std::string &studentName,
                                    const Exam &exam,
                                    double score,
                                    int durationSeconds) = 0;

    /**
     * @brief Kiểm tra điểm số có đạt ngưỡng qua môn không.
     *
     * @param score Điểm cần kiểm tra (thang 10.0).
     * @return true nếu đạt (mặc định >= 5.0).
     */
    virtual bool isPassed(double score) = 0;
};

#endif // I_QUIZ_SERVICE_H