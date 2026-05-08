#ifndef _ENUMS_H_
#define _ENUMS_H_

// ============================================================
//  core/types/enums.h
//  Toàn bộ enum của hệ thống — thay thế magic number
// ============================================================

enum class ExamType
{
    Official = 1, // Thi chính thức — mỗi học sinh chỉ được làm 1 lần
    Practice = 2  // Luyện tập — không giới hạn số lần
};

enum class UserRole
{
    Admin,
    Teacher,
    Student
};

enum class Difficulty
{
    Easy = 1,
    Medium = 2,
    Hard = 3
};

enum class Gender
{
    Male,
    Female,
    Other
};

#endif // !_ENUMS_H_