#ifndef FILE_REPOSITORY_H
#define FILE_REPOSITORY_H

// ============================================================
//  repositories/FileRepository.h
//  Base class cho mọi repository đọc/ghi file .txt
//
//  Cung cấp:
//  - readLines()  : đọc toàn bộ file vào vector<string>,
//                   bỏ qua dòng trống và dòng bắt đầu bằng '#'
//  - safeWrite()  : ghi ra .tmp → rename → tránh mất data khi crash
//  - appendLine() : dùng riêng cho ResultRepository (append-only)
//
//  Subclass KHÔNG được tự mở file — luôn gọi qua helper này.
// ============================================================

#include <string>
#include <vector>
#include <fstream>

class FileRepository
{
protected:
    std::string filePath;

    explicit FileRepository(const std::string &filePath);

    // Đọc file, lọc comment và dòng trống
    std::vector<std::string> readLines() const;

    // Ghi toàn bộ lines ra file theo safe write pattern:
    //   1. Ghi ra filePath + ".tmp"
    //   2. Nếu OK → rename .tmp → filePath
    //   3. Nếu lỗi → xoá .tmp, giữ nguyên file gốc
    bool safeWrite(const std::vector<std::string> &lines) const;

    // Tạo bản sao filePath → filePath + ".bak"
    bool backupFile() const;

    // Append một dòng vào cuối file (không rewrite — dùng cho results)
    bool appendLine(const std::string &line) const;

    // Tách một dòng theo delimiter '|'
    static std::vector<std::string> splitLine(const std::string &line, char delimiter = '|');

    // Static overload — dùng khi cần thao tác file không phải this->filePath
    // (ví dụ UserRepository cần ghi cả 3 file riêng biệt)
    static std::vector<std::string> readLinesFrom(const std::string &path);
    static bool safeWriteTo(const std::string &path, const std::vector<std::string> &lines);
    static bool backupFileTo(const std::string &path);
};

#endif // FILE_REPOSITORY_H