#include "Student.h"
#include <iostream>

// Helper nhỏ — chỉ dùng nội bộ file này
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
      _className(className), _gender(gender), _age(age), _phone(phone) {}

std::string Student::getClassName() const { return _className; }
Gender Student::getGender() const { return _gender; }
int Student::getAge() const { return _age; }
std::string Student::getPhone() const { return _phone; }

std::string Student::getRole() const { return "Student"; }

std::string Student::toFileString() const
{
    // Format: Student|id|username|password|fullName|className|gender|age|phone
    return "Student|" + std::to_string(_id) + "|" + _username + "|" + _password + "|" + _fullName + "|" + _className + "|" + genderToString(_gender) + "|" + std::to_string(_age) + "|" + _phone;
}

void Student::displayInfo() const
{
    std::cout << "[Student] " << _fullName
              << " | Lớp: " << _className
              << " | " << genderToString(_gender)
              << " | " << _age << " tuổi"
              << " | SĐT: " << _phone
              << " (ID: " << _id << ")\n";
}