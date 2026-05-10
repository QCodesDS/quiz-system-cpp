/**
 * @file Student.cpp
 * @brief Triển khai các phương thức cụ thể cho lớp Student.
 */

#include "core/models/Student.h"
#include <iostream>

/**
 * @brief Helper nội bộ chuyển đổi Enum Gender sang chuỗi để lưu trữ hoặc hiển thị.
 */
static std::string genderToString(Gender g)
{
    switch (g)
    {
    case Gender::Male:
        return "Male";
    case Gender::Female:
        return "Female";
    default:
        return "Other";
    }
}

Student::Student(UserId id, const std::string &username,
                 const std::string &password, const std::string &fullName,
                 const std::string &className, Gender gender, int age,
                 const std::string &phone)
    : User(id, username, password, fullName),
      _className(className),
      _gender(gender),
      _age(age),
      _phone(phone)
{
}

// ------------------------------------------------------------
//  Getters
// ------------------------------------------------------------

std::string Student::getClassName() const { return _className; }
Gender Student::getGender() const { return _gender; }
int Student::getAge() const { return _age; }
std::string Student::getPhone() const { return _phone; }

// ------------------------------------------------------------
//  Setters
// ------------------------------------------------------------

void Student::setFullName(const std::string &name)
{
    _fullName = name;
}

void Student::setClassName(const std::string &className)
{
    _className = className;
}

void Student::setGender(Gender gender)
{
    _gender = gender;
}

void Student::setAge(int age)
{
    _age = age;
}

void Student::setPhone(const std::string &phone)
{
    _phone = phone;
}

// ------------------------------------------------------------
//  Overrides
// ------------------------------------------------------------

std::string Student::getRole() const { return "Student"; }

std::string Student::toFileString() const
{
    /**
     * Đồng bộ hóa định dạng lưu trữ file cho Student.
     * Lưu ý: Thứ tự các trường phải khớp với trình tự đọc của Repository.
     */
    return "Student|" +
           std::to_string(_id) + "|" +
           _username + "|" +
           _password + "|" +
           _fullName + "|" +
           _className + "|" +
           genderToString(_gender) + "|" +
           std::to_string(_age) + "|" +
           _phone;
}

void Student::displayInfo() const
{
    // Định dạng hiển thị chi tiết để hỗ trợ việc kiểm tra danh sách trên Console.
    std::cout << "[Student] " << _fullName
              << " | Lớp: " << _className
              << " | " << genderToString(_gender)
              << " | " << _age << " tuổi"
              << " | SĐT: " << _phone
              << " (ID: " << _id << ")\n";
}