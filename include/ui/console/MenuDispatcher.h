#ifndef MENU_DISPATCHER_H
#define MENU_DISPATCHER_H

// Forward declarations
class ServiceContainer;
class User;

/**
 * @namespace MenuDispatcher
 * @brief Handles role-based menu dispatch and execution.
 *
 * Routes users to appropriate menu based on their role (Admin/Teacher/Student)
 * and executes the menu logic.
 */
namespace MenuDispatcher
{
    /**
     * @brief Dispatch user to appropriate menu based on role.
     *
     * @param container ServiceContainer with all services
     * @param user Current logged-in user
     * @return true if user chose logout/exit, false otherwise
     *
     * @details
     * - Checks user->getRole()
     * - Calls AdminMenu::show(), TeacherMenu::show(), or StudentMenu::show()
     * - Returns result of menu (true = logout, false = stay in menu)
     * - Handles unknown roles gracefully
     */
    bool dispatchToMenu(ServiceContainer *container, User *user);
}

#endif // MENU_DISPATCHER_H
