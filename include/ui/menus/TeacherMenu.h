#ifndef TEACHER_MENU_H
#define TEACHER_MENU_H

// Forward declarations
class ServiceContainer;
class User;

/**
 * @class TeacherMenu
 * @brief Stub menu cho Teacher — sẽ được implement trong phase tiếp theo.
 */
class TeacherMenu
{
public:
    /**
     * @brief Hiển thị menu Teacher.
     *
     * @param container ServiceContainer chứa tất cả services
     * @param currentUser User* đang đăng nhập (Teacher role)
     * @return true nếu user chọn logout, false nếu có lỗi
     */
    static bool show(ServiceContainer *container, User *currentUser);
};

#endif // TEACHER_MENU_H
