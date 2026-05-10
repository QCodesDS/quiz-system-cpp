#ifndef FILE_REPOSITORY_H
#define FILE_REPOSITORY_H

#include <string>
#include <vector>

/**
 * @class FileRepository
 * @brief Lớp cơ sở cung cấp các tiện ích xử lý tệp tin văn bản phẳng (.txt).
 *
 * Cung cấp cơ chế đọc/ghi an toàn, hỗ trợ backup và phân tách dữ liệu theo delimiter.
 * Các lớp kế thừa (như UserRepository, ExamRepository) bắt buộc sử dụng các helper
 * từ lớp này thay vì tự thao tác trực tiếp với luồng tệp (fstream).
 */
class FileRepository
{
protected:
    std::string _filePath; ///< Đường dẫn tệp tin mục tiêu.

    /**
     * @brief Khởi tạo repository với đường dẫn cụ thể.
     * Protected constructor đảm bảo lớp này chỉ được sử dụng thông qua kế thừa.
     */
    explicit FileRepository(const std::string &filePath);

    // ------------------------------------------------------------
    //  Các phương thức hỗ trợ luồng dữ liệu (Data Stream Helpers)
    // ------------------------------------------------------------

    /**
     * @brief Đọc nội dung tệp, tự động loại bỏ dòng trống và chú thích (comment).
     */
    std::vector<std::string> readLines() const;

    /**
     * @brief Ghi dữ liệu an toàn (Safe Write Pattern).
     * Cơ chế: Ghi vào .tmp -> Rename .tmp thành file gốc. Giúp tránh mất dữ liệu nếu crash giữa chừng.
     */
    bool safeWrite(const std::vector<std::string> &lines) const;

    /**
     * @brief Tạo bản sao lưu dự phòng (filePath.bak).
     */
    bool backupFile() const;

    /**
     * @brief Ghi thêm một dòng vào cuối tệp (Append mode).
     * Thích hợp cho các tệp nhật ký (Logs) hoặc kết quả thi (Results).
     */
    bool appendLine(const std::string &line) const;

    /**
     * @brief Phân tách chuỗi dựa trên ký tự phân cách (mặc định là '|').
     */
    static std::vector<std::string> splitLine(const std::string &line, char delimiter = '|');

    // ------------------------------------------------------------
    //  Các Static Overloads (Dùng khi thao tác đường dẫn linh hoạt)
    // ------------------------------------------------------------

    static std::vector<std::string> readLinesFrom(const std::string &path);
    static bool safeWriteTo(const std::string &path, const std::vector<std::string> &lines);
    static bool backupFileTo(const std::string &path);
};

#endif // FILE_REPOSITORY_H