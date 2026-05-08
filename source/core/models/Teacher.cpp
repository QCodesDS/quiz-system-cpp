#include "Teacher.h"
#include <iostream>

Teacher::Teacher(UserId id, const std::string &username,
                 const std::string &password, const std::string &fullName,
                 const std::string &subject, const std::string &assignedClass)
    : User(id, username, password, fullName),
      _subject(subject), _assignedClass(assignedClass) {}

std::string Teacher::getSubject() const { return _subject; }
std::string Teacher::getAssignedClass() const { return _assignedClass; }

void Teacher::setAssignedClass(const std::string &cls)
{
    _assignedClass = cls;
}

std::string Teacher::getRole() const { return "Teacher"; }

std::string Teacher::toFileString() const
{
    // Format: Teacher|id|username|password|fullName|subject|assignedClass
    return "Teacher|" + std::to_string(_id) + "|" + _username + "|" + _password + "|" + _fullName + "|" + _subject + "|" + _assignedClass;
}

void Teacher::displayInfo() const
{
    std::cout << "[Teacher] " << _fullName
              << " | Môn: " << _subject
              << " | Lớp CN: " << (_assignedClass.empty() ? "(chưa có)" : _assignedClass)
              << " (ID: " << _id << ")\n";
}