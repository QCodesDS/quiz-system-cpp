#ifndef _TEACHER_H_
#define _TEACHER_H_

// ============================================================
//  core/models/Teacher.h
//  Teacher thêm 2 field: subject (môn dạy) và assignedClass
//  assignedClass để trống nếu chưa được phân công lớp chủ nhiệm
// ============================================================

#include "User.h"

class Teacher : public User
{
private:
    std::string _subject;
    std::string _assignedClass; // "" nếu chưa có

public:
    Teacher(UserId id, const std::string &username,
            const std::string &password, const std::string &fullName,
            const std::string &subject, const std::string &assignedClass = "");

    // --- Getters ---
    std::string getSubject() const;
    std::string getAssignedClass() const;

    // --- Setter ---
    void setAssignedClass(const std::string &cls);

    // --- Override ---
    std::string getRole() const override;      // "Teacher"
    std::string toFileString() const override; // "Teacher|id|user|pass|name|subject|class"
    void displayInfo() const override;
};

#endif // !_TEACHER_H_