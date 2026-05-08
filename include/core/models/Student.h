#ifndef _STUDENT_H_
#define _STUDENT_H_

// ============================================================
//  core/models/Student.h
//  Student thêm: className, gender, age, phone
// ============================================================

#include "User.h"
#include "../types/enums.h"

class Student : public User
{
private:
    std::string _className;
    Gender _gender;
    int _age;
    std::string _phone;

public:
    Student(UserId id, const std::string &username,
            const std::string &password, const std::string &fullName,
            const std::string &className, Gender gender, int age,
            const std::string &phone);

    // --- Getters ---
    std::string getClassName() const;
    Gender getGender() const;
    int getAge() const;
    std::string getPhone() const;

    // --- Override ---
    std::string getRole() const override; // "Student"
    std::string toFileString() const override;
    void displayInfo() const override;
};

#endif // !_STUDENT_H_