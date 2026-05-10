#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <string>
#include <vector>

/**
 * @file typedefs.h
 * @brief Định nghĩa các bí danh kiểu dữ liệu (Type Aliases).
 *
 * Việc sử dụng Alias giúp code mang tính "Self-documenting" (tự giải thích)
 * và cho phép thay đổi kiểu dữ liệu nền tảng (ví dụ: từ int sang long long)
 * trên toàn hệ thống chỉ bằng việc chỉnh sửa tại một nơi duy nhất.
 */

// ----------------------------------------------------------------
//  Định danh (Identifiers)
// ----------------------------------------------------------------

using UserId = int;     ///< ID chung của người dùng.
using StudentId = int;  ///< ID riêng cho đối tượng Học sinh.
using TeacherId = int;  ///< ID riêng cho đối tượng Giáo viên.
using ExamId = int;     ///< ID của đề thi.
using QuestionId = int; ///< ID của câu hỏi.

// ----------------------------------------------------------------
//  Dữ liệu văn bản (String-based Types)
// ----------------------------------------------------------------

using Username = std::string;     ///< Tên đăng nhập.
using PasswordHash = std::string; ///< Mật khẩu đã được mã hóa.
using QuestionText = std::string; ///< Nội dung văn bản của câu hỏi.
using OptionText = std::string;   ///< Nội dung văn bản của một phương án trả lời.

// ----------------------------------------------------------------
//  Cấu trúc dữ liệu phức hợp (Complex Structures)
// ----------------------------------------------------------------

/**
 * @brief Danh sách các phương án trả lời cho một câu hỏi.
 */
using OptionList = std::vector<OptionText>;

/**
 * @brief Danh sách các đáp án đúng (lưu trữ chỉ số 1-4).
 */
using AnswerKey = std::vector<int>;

#endif // TYPEDEFS_H