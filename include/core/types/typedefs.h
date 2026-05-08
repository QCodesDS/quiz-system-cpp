#ifndef _TYPEDEFS_H_
#define _TYPEDEFS_H_

// ============================================================
//  core/types/typedefs.h
//  Type aliases — giúp code tự documenting, dễ refactor sau
// ============================================================

#include <string>
#include <vector>

using UserId = int;
using StudentId = int;
using TeacherId = int;
using ExamId = int;
using QuestionId = int;

using Username = std::string;
using PasswordHash = std::string;
using QuestionText = std::string;
using OptionText = std::string;
using OptionList = std::vector<OptionText>;
using AnswerKey = std::vector<int>; // Lưu index của các đáp án đúng

#endif // !_TYPEDEFS_H_
