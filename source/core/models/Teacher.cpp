/**
 * @file Teacher.cpp
 * @brief Triển khai các phương thức cụ thể cho lớp Teacher.
 */

#include "core/models/Teacher.h"
#include <iostream>

Teacher::Teacher(UserId id, const std::string &username,
                 const std::string &password, const std::string &fullName,
                 const std::string &subject, const std::string &assignedClass)
    : User(id, username, password, fullName),
      _subject(subject),
      _assignedClass(assignedClass)
{
}

// ------------------------------------------------------------
//  Getters & Setters
// ------------------------------------------------------------

std::string Teacher::getSubject() const { return _subject; }

std::string Teacher::getAssignedClass() const { return _assignedClass; }

void Teacher::setAssignedClass(const std::string &cls)
{
    _assignedClass = cls;
}

void Teacher::setFullName(const std::string &name)
{
    _fullName = name;
}

void Teacher::setSubject(const std::string &subject)
{
    _subject = subject;
}

// ------------------------------------------------------------
//  Override từ User
// ------------------------------------------------------------

std::string Teacher::getRole() const
{
    return "Teacher";
}

std::string Teacher::toFileString() const
{
    /**
     * Định dạng lưu trữ đảm bảo tính nhất quán để Repository có thể phân tách (split).
     * assignedClass có thể là chuỗi rỗng ở cuối dòng.
     */
    return "Teacher|" +
           std::to_string(_id) + "|" +
           _username + "|" +
           _password + "|" +
           _fullName + "|" +
           _subject + "|" +
           _assignedClass;
}

void Teacher::displayInfo() const
{
    // Kiểm tra trạng thái rỗng của lớp chủ nhiệm để hiển thị thông tin thân thiện.
    std::string classInfo = _assignedClass.empty() ? "(Chưa phân công)" : _assignedClass;

    std::cout << "[Teacher] " << _fullName
              << " | Môn: " << _subject
              << " | Lớp CN: " << classInfo
              << " (ID: " << _id << ")\n";
}