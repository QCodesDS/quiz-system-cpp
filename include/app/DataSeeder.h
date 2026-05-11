#ifndef DATA_SEEDER_H
#define DATA_SEEDER_H

// Forward declaration
class ServiceContainer;

/**
 * @class DataSeeder
 * @brief Utility class for seeding test data into the system.
 *
 * Provides static methods to populate the system with test users
 * if they don't already exist. Safe to call multiple times.
 *
 * Test Credentials:
 * - Admin: admin / admin123 (if not already in data file)
 * - Teacher: teacher01 / teacher123
 * - Student: student01 / student123
 */
class DataSeeder
{
public:
    /**
     * @brief Seed test users if they don't already exist.
     *
     * Attempts to login with test credentials. If login fails (user doesn't exist),
     * creates the user using UserService. If login succeeds, logs out and skips seeding.
     *
     * @param container ServiceContainer with all services
     *
     * @details
     * - Tests admin account; doesn't create (requires admin-only method)
     * - Creates teacher01 if not found
     * - Creates student01 if not found
     * - All passwords are hashed via AuthService::hashPassword()
     */
    static void seed(ServiceContainer &container);
};

#endif // DATA_SEEDER_H
