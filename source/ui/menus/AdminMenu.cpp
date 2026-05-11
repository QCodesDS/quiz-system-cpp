#include "ui/menus/AdminMenu.h"
#include "ServiceContainer.h"
#include "User.h"
#include "core/models/Teacher.h"
#include "core/models/Student.h"
#include "core/interface/IAuthService.h"
#include "services/UserService.h"
#include "services/AuthService.h"
#include "enums.h"
#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <iomanip>

// ============================================================
//  Main Menu
// ============================================================

bool AdminMenu::show(ServiceContainer *container, User *currentUser)
{
    if (!container || !currentUser)
    {
        std::cerr << "ERROR: Container or User is null\n";
        return false;
    }

    bool backToLogin = false;

    while (!backToLogin)
    {
        clearScreen();

        std::cout << "\n╔═══════════════════════════════════════════════════════╗\n";
        std::cout << "║           MENU QUẢN TRỊ VIÊN HỆ THỐNG                 ║\n";
        std::cout << "║                                                       ║\n";
        std::cout << "║  Xin chào, " << std::setw(35) << std::left << currentUser->getFullName() << " ║\n";
        std::cout << "║                                                       ║\n";
        std::cout << "║  [1] Quản lý Giáo viên                                ║\n";
        std::cout << "║  [2] Quản lý Học sinh                                 ║\n";
        std::cout << "║  [L] Đăng xuất                                        ║\n";
        std::cout << "║  [E] Thoát chương trình                               ║\n";
        std::cout << "║                                                       ║\n";
        std::cout << "╚═══════════════════════════════════════════════════════╝\n\n";

        std::cout << "Lựa chọn của bạn: ";
        char choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice)
        {
        case '1':
            showTeacherMenu(container);
            break;
        case '2':
            showStudentMenu(container);
            break;
        case 'L':
        case 'l':
            backToLogin = true;
            break;
        case 'E':
        case 'e':
            backToLogin = true;
            break;
        default:
            std::cout << "Lựa chọn không hợp lệ. Vui lòng thử lại.\n";
            pauseForUser();
            break;
        }
    }

    return true;
}

// ============================================================
//  Teacher Submenu
// ============================================================

bool AdminMenu::showTeacherMenu(ServiceContainer *container)
{
    bool backToMain = false;

    while (!backToMain)
    {
        clearScreen();

        std::cout << "\n╔═══════════════════════════════════════════════════════╗\n";
        std::cout << "║          QUẢN LÝ GIÁO VIÊN                           ║\n";
        std::cout << "║                                                       ║\n";
        std::cout << "║  [1] Xem danh sách giáo viên                         ║\n";
        std::cout << "║  [2] Thêm giáo viên                                  ║\n";
        std::cout << "║  [3] Sửa thông tin giáo viên                         ║\n";
        std::cout << "║  [4] Xóa giáo viên                                   ║\n";
        std::cout << "║  [0] Quay lại                                        ║\n";
        std::cout << "║                                                       ║\n";
        std::cout << "╚═══════════════════════════════════════════════════════╝\n\n";

        std::cout << "Lựa chọn của bạn: ";
        char choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice)
        {
        case '1':
            viewAllTeachers(container);
            break;
        case '2':
            addNewTeacher(container);
            break;
        case '3':
            editTeacher(container);
            break;
        case '4':
            deleteTeacher(container);
            break;
        case '0':
            backToMain = true;
            break;
        default:
            std::cout << "Lựa chọn không hợp lệ. Vui lòng thử lại.\n";
            pauseForUser();
            break;
        }
    }

    return true;
}

// ============================================================
//  Student Submenu
// ============================================================

bool AdminMenu::showStudentMenu(ServiceContainer *container)
{
    bool backToMain = false;

    while (!backToMain)
    {
        clearScreen();

        std::cout << "\n╔═══════════════════════════════════════════════════════╗\n";
        std::cout << "║           QUẢN LÝ HỌC SINH                           ║\n";
        std::cout << "║                                                       ║\n";
        std::cout << "║  [1] Xem danh sách học sinh                          ║\n";
        std::cout << "║  [2] Thêm học sinh                                   ║\n";
        std::cout << "║  [3] Sửa thông tin học sinh                          ║\n";
        std::cout << "║  [4] Xóa học sinh                                    ║\n";
        std::cout << "║  [0] Quay lại                                        ║\n";
        std::cout << "║                                                       ║\n";
        std::cout << "╚═══════════════════════════════════════════════════════╝\n\n";

        std::cout << "Lựa chọn của bạn: ";
        char choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice)
        {
        case '1':
            viewAllStudents(container);
            break;
        case '2':
            addNewStudent(container);
            break;
        case '3':
            editStudent(container);
            break;
        case '4':
            deleteStudent(container);
            break;
        case '0':
            backToMain = true;
            break;
        default:
            std::cout << "Lựa chọn không hợp lệ. Vui lòng thử lại.\n";
            pauseForUser();
            break;
        }
    }

    return true;
}

// ============================================================
//  Teacher Operations
// ============================================================

void AdminMenu::viewAllTeachers(ServiceContainer *container)
{
    clearScreen();

    auto *userSvc = container->getUserService();
    auto teachers = userSvc->getAllTeachers();

    if (teachers.empty())
    {
        std::cout << "\n✗ Không có giáo viên nào trong hệ thống.\n";
        pauseForUser();
        return;
    }

    std::cout << "\n";
    printTeacherTableHeader();

    for (auto teacher : teachers)
    {
        if (teacher)
            printTeacherTableRow(teacher);
    }

    std::cout << "╚═════╩════════════════╩════════════════╩═══════════╩════════════════╝\n";
    pauseForUser();
}

void AdminMenu::addNewTeacher(ServiceContainer *container)
{
    clearScreen();

    std::cout << "\n╔═══════════════════════════════════════════════════════╗\n";
    std::cout << "║           THÊM GIÁO VIÊN MỚI                          ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════╝\n\n";

    std::string username, fullName, subject, assignedClass;

    std::cout << "Tên đăng nhập: ";
    std::getline(std::cin, username);

    if (username.empty())
    {
        showError("Tên đăng nhập không được để trống!");
        return;
    }

    std::cout << "Mật khẩu: ";
    std::string password = getHiddenPassword();

    if (password.empty())
    {
        showError("Mật khẩu không được để trống!");
        return;
    }

    std::cout << "Họ tên: ";
    std::getline(std::cin, fullName);

    if (fullName.empty())
    {
        showError("Họ tên không được để trống!");
        return;
    }

    std::cout << "Môn học: ";
    std::getline(std::cin, subject);

    if (subject.empty())
    {
        showError("Môn học không được để trống!");
        return;
    }

    std::cout << "Lớp phụ trách (để trống nếu không có): ";
    std::getline(std::cin, assignedClass);

    auto *userSvc = container->getUserService();
    bool ok = userSvc->addTeacher(username, password, fullName, subject, assignedClass);

    if (ok)
    {
        showSuccess("Thêm giáo viên thành công!");
    }
    else
    {
        showError("Thêm giáo viên thất bại! (Kiểm tra tên đăng nhập đã tồn tại)");
    }
}

void AdminMenu::editTeacher(ServiceContainer *container)
{
    clearScreen();

    std::cout << "\n╔═══════════════════════════════════════════════════════╗\n";
    std::cout << "║           SỬA THÔNG TIN GIÁO VIÊN                     ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════╝\n\n";

    int id;
    std::cout << "Nhập ID giáo viên: ";
    std::cin >> id;
    std::cin.ignore();

    auto *userSvc = container->getUserService();
    auto teachers = userSvc->getAllTeachers();

    Teacher *teacher = nullptr;
    for (auto t : teachers)
    {
        if (t && t->getId() == id)
        {
            teacher = t;
            break;
        }
    }

    if (!teacher)
    {
        showError("Không tìm thấy giáo viên với ID này!");
        return;
    }

    std::cout << "\nThông tin hiện tại:\n";
    std::cout << "  Tên: " << teacher->getFullName() << "\n";
    std::cout << "  Môn học: " << teacher->getSubject() << "\n";
    std::cout << "  Lớp phụ trách: " << (teacher->getAssignedClass().empty() ? "(không có)" : teacher->getAssignedClass()) << "\n\n";

    std::string fullName, subject, assignedClass;

    std::cout << "Họ tên (Enter để giữ nguyên): ";
    std::getline(std::cin, fullName);
    if (fullName.empty())
        fullName = teacher->getFullName();

    std::cout << "Môn học (Enter để giữ nguyên): ";
    std::getline(std::cin, subject);
    if (subject.empty())
        subject = teacher->getSubject();

    std::cout << "Lớp phụ trách (Enter để giữ nguyên): ";
    std::getline(std::cin, assignedClass);
    if (assignedClass.empty())
        assignedClass = teacher->getAssignedClass();

    bool ok = userSvc->updateTeacher(id, fullName, subject, assignedClass);

    if (ok)
    {
        showSuccess("Cập nhật giáo viên thành công!");
    }
    else
    {
        showError("Cập nhật giáo viên thất bại!");
    }
}

void AdminMenu::deleteTeacher(ServiceContainer *container)
{
    clearScreen();

    std::cout << "\n╔═══════════════════════════════════════════════════════╗\n";
    std::cout << "║           XÓA GIÁO VIÊN                               ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════╝\n\n";

    int id;
    std::cout << "Nhập ID giáo viên cần xóa: ";
    std::cin >> id;
    std::cin.ignore();

    auto *userSvc = container->getUserService();
    auto teachers = userSvc->getAllTeachers();

    Teacher *teacher = nullptr;
    for (auto t : teachers)
    {
        if (t && t->getId() == id)
        {
            teacher = t;
            break;
        }
    }

    if (!teacher)
    {
        showError("Không tìm thấy giáo viên với ID này!");
        return;
    }

    std::cout << "\nThông tin giáo viên:\n";
    std::cout << "  ID: " << teacher->getId() << "\n";
    std::cout << "  Tên: " << teacher->getFullName() << "\n";
    std::cout << "  Môn: " << teacher->getSubject() << "\n\n";

    if (confirmAction("Bạn có chắc chắn muốn xóa giáo viên này?"))
    {
        bool ok = userSvc->removeUser(id);
        if (ok)
        {
            showSuccess("Xóa giáo viên thành công!");
        }
        else
        {
            showError("Xóa giáo viên thất bại!");
        }
    }
    else
    {
        std::cout << "✓ Hủy thao tác xóa.\n";
        pauseForUser();
    }
}

// ============================================================
//  Student Operations
// ============================================================

void AdminMenu::viewAllStudents(ServiceContainer *container)
{
    clearScreen();

    auto *userSvc = container->getUserService();
    auto students = userSvc->getAllStudents();

    if (students.empty())
    {
        std::cout << "\n✗ Không có học sinh nào trong hệ thống.\n";
        pauseForUser();
        return;
    }

    std::cout << "\n";
    printStudentTableHeader();

    for (auto student : students)
    {
        if (student)
            printStudentTableRow(student);
    }

    std::cout << "╚═════╩════════════════╩════════════════╩═════╩═══╩═════╩══════════════╩═══════════╝\n";
    pauseForUser();
}

void AdminMenu::addNewStudent(ServiceContainer *container)
{
    clearScreen();

    std::cout << "\n╔═══════════════════════════════════════════════════════╗\n";
    std::cout << "║           THÊM HỌC SINH MỚI                           ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════╝\n\n";

    std::string username, fullName, className, phone;
    char genderChoice;
    int age;

    std::cout << "Tên đăng nhập: ";
    std::getline(std::cin, username);
    if (username.empty())
    {
        showError("Tên đăng nhập không được để trống!");
        return;
    }

    std::cout << "Mật khẩu: ";
    std::string password = getHiddenPassword();
    if (password.empty())
    {
        showError("Mật khẩu không được để trống!");
        return;
    }

    std::cout << "Họ tên: ";
    std::getline(std::cin, fullName);
    if (fullName.empty())
    {
        showError("Họ tên không được để trống!");
        return;
    }

    std::cout << "Lớp: ";
    std::getline(std::cin, className);
    if (className.empty())
    {
        showError("Lớp không được để trống!");
        return;
    }

    std::cout << "Giới tính (M/F): ";
    std::cin >> genderChoice;
    std::cin.ignore();
    Gender gender = parseGender(genderChoice);

    std::cout << "Tuổi (10-25): ";
    std::cin >> age;
    std::cin.ignore();
    if (age < 10 || age > 25)
    {
        showError("Tuổi phải trong khoảng 10-25!");
        return;
    }

    std::cout << "Số điện thoại (10 số): ";
    std::getline(std::cin, phone);
    if (phone.length() != 10 || phone.find_first_not_of("0123456789") != std::string::npos)
    {
        showError("Số điện thoại phải là 10 chữ số!");
        return;
    }

    auto *userSvc = container->getUserService();
    bool ok = userSvc->addStudent(username, password, fullName, className, gender, age, phone);

    if (ok)
    {
        showSuccess("Thêm học sinh thành công!");
    }
    else
    {
        showError("Thêm học sinh thất bại! (Kiểm tra tên đăng nhập đã tồn tại)");
    }
}

void AdminMenu::editStudent(ServiceContainer *container)
{
    clearScreen();

    std::cout << "\n╔═══════════════════════════════════════════════════════╗\n";
    std::cout << "║           SỬA THÔNG TIN HỌC SINH                      ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════╝\n\n";

    int id;
    std::cout << "Nhập ID học sinh: ";
    std::cin >> id;
    std::cin.ignore();

    auto *userSvc = container->getUserService();
    auto students = userSvc->getAllStudents();

    Student *student = nullptr;
    for (auto s : students)
    {
        if (s && s->getId() == id)
        {
            student = s;
            break;
        }
    }

    if (!student)
    {
        showError("Không tìm thấy học sinh với ID này!");
        return;
    }

    std::cout << "\nThông tin hiện tại:\n";
    std::cout << "  Tên: " << student->getFullName() << "\n";
    std::cout << "  Lớp: " << student->getClassName() << "\n";
    std::cout << "  Giới tính: " << (student->getGender() == Gender::Male ? "Nam" : "Nữ") << "\n";
    std::cout << "  Tuổi: " << student->getAge() << "\n";
    std::cout << "  Số ĐT: " << student->getPhone() << "\n\n";

    std::string fullName, className, phone;
    int age;

    std::cout << "Họ tên (Enter để giữ nguyên): ";
    std::getline(std::cin, fullName);
    if (fullName.empty())
        fullName = student->getFullName();

    std::cout << "Lớp (Enter để giữ nguyên): ";
    std::getline(std::cin, className);
    if (className.empty())
        className = student->getClassName();

    std::cout << "Giới tính M/F (Enter để giữ nguyên): ";
    std::string genderInput;
    std::getline(std::cin, genderInput);
    Gender gender = student->getGender();
    if (!genderInput.empty())
        gender = parseGender(genderInput[0]);

    std::cout << "Tuổi (Enter để giữ nguyên): ";
    std::string ageInput;
    std::getline(std::cin, ageInput);
    age = student->getAge();
    if (!ageInput.empty())
    {
        age = std::stoi(ageInput);
        if (age < 10 || age > 25)
        {
            showError("Tuổi phải trong khoảng 10-25!");
            return;
        }
    }

    std::cout << "Số điện thoại (Enter để giữ nguyên): ";
    std::getline(std::cin, phone);
    if (phone.empty())
        phone = student->getPhone();
    else if (phone.length() != 10 || phone.find_first_not_of("0123456789") != std::string::npos)
    {
        showError("Số điện thoại phải là 10 chữ số!");
        return;
    }

    bool ok = userSvc->updateStudent(id, fullName, className, gender, age, phone);

    if (ok)
    {
        showSuccess("Cập nhật học sinh thành công!");
    }
    else
    {
        showError("Cập nhật học sinh thất bại!");
    }
}

void AdminMenu::deleteStudent(ServiceContainer *container)
{
    clearScreen();

    std::cout << "\n╔═══════════════════════════════════════════════════════╗\n";
    std::cout << "║           XÓA HỌC SINH                                ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════╝\n\n";

    int id;
    std::cout << "Nhập ID học sinh cần xóa: ";
    std::cin >> id;
    std::cin.ignore();

    auto *userSvc = container->getUserService();
    auto students = userSvc->getAllStudents();

    Student *student = nullptr;
    for (auto s : students)
    {
        if (s && s->getId() == id)
        {
            student = s;
            break;
        }
    }

    if (!student)
    {
        showError("Không tìm thấy học sinh với ID này!");
        return;
    }

    std::cout << "\nThông tin học sinh:\n";
    std::cout << "  ID: " << student->getId() << "\n";
    std::cout << "  Tên: " << student->getFullName() << "\n";
    std::cout << "  Lớp: " << student->getClassName() << "\n\n";

    if (confirmAction("Bạn có chắc chắn muốn xóa học sinh này?"))
    {
        bool ok = userSvc->removeUser(id);
        if (ok)
        {
            showSuccess("Xóa học sinh thành công!");
        }
        else
        {
            showError("Xóa học sinh thất bại!");
        }
    }
    else
    {
        std::cout << "✓ Hủy thao tác xóa.\n";
        pauseForUser();
    }
}

// ============================================================
//  Helper Functions
// ============================================================

void AdminMenu::clearScreen()
{
    system("cls");
}

std::string AdminMenu::getHiddenPassword()
{
    std::string password;
    char ch;
    while ((ch = _getch()) != '\r')
    {
        if (ch == '\b')
        {
            if (!password.empty())
            {
                password.pop_back();
                std::cout << "\b \b";
            }
        }
        else
        {
            password += ch;
            std::cout << '*';
        }
    }
    std::cout << "\n";
    return password;
}

Gender AdminMenu::parseGender(char choice)
{
    if (choice == 'M' || choice == 'm')
        return Gender::Male;
    return Gender::Female;
}

bool AdminMenu::confirmAction(const std::string &message)
{
    std::cout << message << " (Y/N): ";
    char choice;
    std::cin >> choice;
    std::cin.ignore();
    return (choice == 'Y' || choice == 'y');
}

void AdminMenu::pauseForUser()
{
    std::cout << "\nNhấn Enter để tiếp tục...";
    std::cin.get();
}

void AdminMenu::showSuccess(const std::string &message)
{
    clearScreen();
    std::cout << "\n✓ " << message << "\n";
    pauseForUser();
}

void AdminMenu::showError(const std::string &message)
{
    clearScreen();
    std::cout << "\n✗ " << message << "\n";
    pauseForUser();
}

void AdminMenu::printTeacherTableHeader()
{
    std::cout << "╔═════╦════════════════╦════════════════╦═══════════╦════════════════╗\n";
    std::cout << "║ ID  ║ Tên đăng nhập  ║ Họ tên         ║ Môn học   ║ Lớp phụ trách  ║\n";
    std::cout << "╠═════╬════════════════╬════════════════╬═══════════╬════════════════╣\n";
}

void AdminMenu::printTeacherTableRow(Teacher *teacher)
{
    std::cout << "║ " << std::setw(3) << std::left << teacher->getId() << " ║ ";
    std::cout << std::setw(14) << std::left << teacher->getUsername() << " ║ ";
    std::cout << std::setw(14) << std::left << (teacher->getFullName().length() > 14 ? teacher->getFullName().substr(0, 14) : teacher->getFullName()) << " ║ ";
    std::cout << std::setw(9) << std::left << (teacher->getSubject().length() > 9 ? teacher->getSubject().substr(0, 9) : teacher->getSubject()) << " ║ ";
    std::string assignedClass = teacher->getAssignedClass().empty() ? "(không có)" : teacher->getAssignedClass();
    std::cout << std::setw(14) << std::left << (assignedClass.length() > 14 ? assignedClass.substr(0, 14) : assignedClass) << " ║\n";
}

void AdminMenu::printStudentTableHeader()
{
    std::cout << "╔═════╦════════════════╦════════════════╦═════╦═══╦═════╦══════════════╦═══════════╗\n";
    std::cout << "║ ID  ║ Tên đăng nhập  ║ Họ tên         ║ Lớp ║GT ║Tuổi║ Số điện thoại║ Trạng thái║\n";
    std::cout << "╠═════╬════════════════╬════════════════╬═════╬═══╬═════╬══════════════╬═══════════╣\n";
}

void AdminMenu::printStudentTableRow(Student *student)
{
    std::cout << "║ " << std::setw(3) << std::left << student->getId() << " ║ ";
    std::cout << std::setw(14) << std::left << student->getUsername() << " ║ ";
    std::cout << std::setw(14) << std::left << (student->getFullName().length() > 14 ? student->getFullName().substr(0, 14) : student->getFullName()) << " ║ ";
    std::cout << std::setw(3) << std::left << student->getClassName() << " ║ ";
    std::cout << std::setw(1) << (student->getGender() == Gender::Male ? "M" : "F") << "  ║ ";
    std::cout << std::setw(3) << std::left << student->getAge() << " ║ ";
    std::cout << std::setw(12) << std::left << student->getPhone() << " ║ Active    ║\n";
}
