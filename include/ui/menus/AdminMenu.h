#ifndef ADMIN_MENU_H
#define ADMIN_MENU_H

// Forward declarations
class ServiceContainer;
class User;

/**
 * @class AdminMenu
 * @brief Stub menu cho Admin — sẽ được implement trong phase tiếp theo.
 */
class AdminMenu
{
public:
    /**
     * @brief Hiển thị menu Admin.
     *
     * @param container ServiceContainer chứa tất cả services
     * @param currentUser User* đang đăng nhập (Admin role)
     * @return true nếu user chọn logout, false nếu có lỗi
     */
    static bool show(ServiceContainer *container, User *currentUser);
};

#endif // ADMIN_MENU_H
