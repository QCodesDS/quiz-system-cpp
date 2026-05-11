#include "ui/console/MenuDispatcher.h"
#include "ServiceContainer.h"
#include "User.h"
#include "ui/menus/AdminMenu.h"
#include "ui/menus/TeacherMenu.h"
#include "ui/menus/StudentMenu.h"
#include <iostream>

namespace MenuDispatcher
{
    bool dispatchToMenu(ServiceContainer *container, User *user)
    {
        if (!container || !user)
        {
            std::cerr << "ERROR: Container or User is null\n";
            return false;
        }

        std::string role = user->getRole();

        if (role == "Admin")
        {
            return AdminMenu::show(container, user);
        }
        else if (role == "Teacher")
        {
            return TeacherMenu::show(container, user);
        }
        else if (role == "Student")
        {
            return StudentMenu::show(container, user);
        }
        else
        {
            std::cerr << "ERROR: Unknown role: " << role << "\n";
            return false;
        }
    }
}
