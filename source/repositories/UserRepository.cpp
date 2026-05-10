#include "repositories/UserRepository.h"
#include "repositories/handlers/IUserTypeHandler.h"
#include <fstream>
#include <algorithm>
#include <iterator>

UserRepository::UserRepository(const std::string &dataDir,
                               std::vector<IUserTypeHandler *> handlers)
    : FileRepository(dataDir), // Sử dụng dataDir làm đường dẫn gốc
      _handlers(handlers)
{
}

std::vector<std::unique_ptr<User>> UserRepository::load()
{
    std::vector<std::unique_ptr<User>> allUsers;

    /**
     * Lặp qua từng Handler: Mỗi handler tự biết file nào của nó và cách parse dữ liệu đó.
     * Điều này giúp loại bỏ hoàn toàn các chuỗi if (role == "Admin")...
     */
    for (auto *handler : _handlers)
    {
        if (!handler)
            continue;

        // Đọc nội dung thô từ file tương ứng của Handler
        std::vector<std::string> lines = readLinesFrom(handler->getFilePath());

        // Yêu cầu Handler chuyển đổi text thành danh sách đối tượng
        auto users = handler->deserialize(lines);

        // Gộp kết quả vào danh sách tổng (Sử dụng move iterator để tối ưu hiệu năng)
        allUsers.insert(allUsers.end(),
                        std::make_move_iterator(users.begin()),
                        std::make_move_iterator(users.end()));
    }

    return allUsers;
}

// ------------------------------------------------------------
//  Triển khai IUserRepository
// ------------------------------------------------------------

bool UserRepository::save(const std::vector<std::unique_ptr<User>> &users)
{
    bool isAllSuccess = true;

    for (auto *handler : _handlers)
    {
        if (!handler)
            continue;

        /**
         * Bước 1: Handler lọc ra những User thuộc loại nó quản lý và serialize thành text.
         */
        auto lines = handler->serialize(users);

        /**
         * Bước 2: Ghi dữ liệu đã serialize vào đúng file của loại User đó.
         */
        bool result = safeWriteTo(handler->getFilePath(), lines);
        isAllSuccess &= result;
    }

    return isAllSuccess;
}

bool UserRepository::backup()
{
    bool isAllSuccess = true;
    for (auto *handler : _handlers)
    {
        if (!handler)
            continue;
        isAllSuccess &= backupFileTo(handler->getFilePath());
    }
    return isAllSuccess;
}

std::unique_ptr<User> UserRepository::findById(UserId id)
{
    auto all = load(); // Load toàn bộ vào bộ nhớ để tìm kiếm
    std::unique_ptr<User> found = nullptr;

    for (auto &u : all)
    {
        if (u && u->getId() == id)
        {
            // Chuyển quyền sở hữu đối tượng tìm thấy ra khỏi vector
            found = std::move(u);
            break;
        }
    }
    // Khi vector 'all' bị hủy, các unique_ptr còn lại sẽ tự giải phóng bộ nhớ.
    return found;
}

std::unique_ptr<User> UserRepository::findByUsername(const std::string &username)
{
    auto all = load();
    std::unique_ptr<User> found = nullptr;

    for (auto &u : all)
    {
        if (u && u->getUsername() == username)
        {
            found = std::move(u);
            break;
        }
    }
    return found;
}

bool UserRepository::usernameExists(const std::string &username)
{
    auto all = load();
    return std::any_of(all.begin(), all.end(), [&](const std::unique_ptr<User> &u)
                       { return u && u->getUsername() == username; });
}