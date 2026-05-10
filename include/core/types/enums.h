#ifndef ENUMS_H
#define ENUMS_H

/**
 * @file enums.h
 * @brief Định nghĩa các kiểu liệt kê (Enumerations) toàn cục.
 *
 * Sử dụng 'enum class' (Strongly Typed Enums) để tránh xung đột tên
 * và bắt buộc kiểm tra kiểu dữ liệu nghiêm ngặt trong quá trình biên dịch.
 */

/**
 * @enum ExamType
 * @brief Phân loại hình thức thi.
 */
enum class ExamType
{
    Official = 1, ///< Thi chính thức: Lưu kết quả, giới hạn 01 lần thực hiện.
    Practice = 2  ///< Luyện tập: Cho phép thực hiện không giới hạn số lần.
};

/**
 * @enum UserRole
 * @brief Các vai trò người dùng trong hệ thống.
 */
enum class UserRole
{
    Admin,   ///< Quản trị viên hệ thống.
    Teacher, ///< Giáo viên (quản lý đề thi/câu hỏi).
    Student  ///< Học sinh (thực hiện bài thi/xem kết quả).
};

/**
 * @enum Difficulty
 * @brief Các mức độ khó của câu hỏi trắc nghiệm.
 */
enum class Difficulty
{
    Easy = 1,   ///< Dễ.
    Medium = 2, ///< Trung bình.
    Hard = 3    ///< Khó.
};

/**
 * @enum Gender
 * @brief Giới tính của người dùng.
 */
enum class Gender
{
    Male,   ///< Nam.
    Female, ///< Nữ.
    Other   ///< Khác.
};

#endif // ENUMS_H