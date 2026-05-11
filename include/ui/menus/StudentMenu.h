#ifndef STUDENT_MENU_H
#define STUDENT_MENU_H

// Forward declarations
class ServiceContainer;
class User;

/**
 * @class StudentMenu
 * @brief Stub menu cho Student — sẽ được implement trong phase tiếp theo.
 */
class StudentMenu
{
public:
    /**
     * @brief Hiển thị menu Student.
     *
     * @param container ServiceContainer chứa tất cả services
     * @param currentUser User* đang đăng nhập (Student role)
     * @return true nếu user chọn logout, false nếu có lỗi
     */
    static bool show(ServiceContainer *container, User *currentUser);
};

#endif // STUDENT_MENU_H
