#ifndef ADMIN_MENU_H
#define ADMIN_MENU_H

#include <string>
#include <vector>
#include "enums.h"

// Forward declarations
class ServiceContainer;
class User;
class Teacher;
class Student;

/**
 * @class AdminMenu
 * @brief Menu chính cho Admin — quản lý giáo viên và học sinh.
 *
 * Cung cấp các chức năng:
 * - Xem, thêm, sửa, xóa giáo viên
 * - Xem, thêm, sửa, xóa học sinh
 * - Trở về màn hình đăng nhập hoặc thoát ứng dụng
 */
class AdminMenu
{
public:
    /**
     * @brief Hiển thị menu Admin chính.
     *
     * @param container ServiceContainer chứa tất cả services
     * @param currentUser User* đang đăng nhập (Admin role)
     * @return true nếu user chọn logout/exit, false nếu có lỗi
     */
    static bool show(ServiceContainer *container, User *currentUser);

private:
    // ============================================================
    //  Helper Functions
    // ============================================================

    /**
     * @brief Xóa màn hình (Windows: system("cls")).
     */
    static void clearScreen();

    /**
     * @brief Hiển thị menu quản lý giáo viên.
     * @return true nếu quay lại menu chính
     */
    static bool showTeacherMenu(ServiceContainer *container);

    /**
     * @brief Hiển thị menu quản lý học sinh.
     * @return true nếu quay lại menu chính
     */
    static bool showStudentMenu(ServiceContainer *container);

    // --- Teacher Submenu Operations ---
    static void viewAllTeachers(ServiceContainer *container);
    static void addNewTeacher(ServiceContainer *container);
    static void editTeacher(ServiceContainer *container);
    static void deleteTeacher(ServiceContainer *container);

    // --- Student Submenu Operations ---
    static void viewAllStudents(ServiceContainer *container);
    static void addNewStudent(ServiceContainer *container);
    static void editStudent(ServiceContainer *container);
    static void deleteStudent(ServiceContainer *container);

    // --- Input & Validation Helpers ---
    static std::string getHiddenPassword();
    static Gender parseGender(char choice);
    static bool confirmAction(const std::string &message);
    static void pauseForUser();
    static void showSuccess(const std::string &message);
    static void showError(const std::string &message);

    // --- Table Display Helpers ---
    static void printTeacherTableHeader();
    static void printTeacherTableRow(Teacher *teacher);
    static void printStudentTableHeader();
    static void printStudentTableRow(Student *student);
};

#endif // ADMIN_MENU_H
