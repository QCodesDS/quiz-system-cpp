#include "FileRepository.h"
#include <fstream>
#include <sstream>
#include <cstdio>

FileRepository::FileRepository(const std::string &filePath)
    : _filePath(filePath)
{
}

// ------------------------------------------------------------
//  Các phương thức hỗ trợ luồng dữ liệu (Data Stream Helpers)
// ------------------------------------------------------------

std::vector<std::string> FileRepository::readLines() const
{
    std::vector<std::string> result;
    std::ifstream file(_filePath);

    if (!file.is_open())
        return result;

    std::string line;
    while (std::getline(file, line))
    {
        // Xử lý ký tự kết thúc dòng của Windows (\r\n)
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        // Bỏ qua dòng trống hoặc dòng chú thích bắt đầu bằng '#'
        if (line.empty() || line[0] == '#')
            continue;

        result.push_back(line);
    }

    return result;
}

bool FileRepository::safeWrite(const std::vector<std::string> &lines) const
{
    std::string tmpPath = _filePath + ".tmp";

    // Giai đoạn 1: Ghi vào tệp tạm thời
    std::ofstream tmp(tmpPath, std::ios::out | std::ios::trunc);
    if (!tmp.is_open())
        return false;

    for (const auto &line : lines)
    {
        tmp << line << "\n";
        if (tmp.fail())
        {
            tmp.close();
            std::remove(tmpPath.c_str());
            return false;
        }
    }
    tmp.close();

    // Giai đoạn 2: Thay thế tệp gốc bằng tệp tạm (Atomic-like rename)
    // Lưu ý: Trên Windows, rename có thể fail nếu file gốc đang mở.
    std::remove(_filePath.c_str());
    if (std::rename(tmpPath.c_str(), _filePath.c_str()) != 0)
    {
        std::remove(tmpPath.c_str());
        return false;
    }

    return true;
}

bool FileRepository::backupFile() const
{
    std::ifstream src(_filePath, std::ios::binary);
    if (!src.is_open())
        return false;

    std::string bakPath = _filePath + ".bak";
    std::ofstream dst(bakPath, std::ios::binary | std::ios::trunc);
    if (!dst.is_open())
        return false;

    dst << src.rdbuf();
    return dst.good();
}

bool FileRepository::appendLine(const std::string &line) const
{
    std::ofstream file(_filePath, std::ios::app);
    if (!file.is_open())
        return false;

    file << line << "\n";
    return file.good();
}

std::vector<std::string> FileRepository::splitLine(const std::string &line, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream stream(line);

    while (std::getline(stream, token, delimiter))
    {
        tokens.push_back(token);
    }

    // Xử lý trường hợp dòng kết thúc bằng ký tự phân cách (field cuối rỗng)
    if (!line.empty() && line.back() == delimiter)
    {
        tokens.push_back("");
    }

    return tokens;
}

// ------------------------------------------------------------
//  Các Static Overloads (Dùng khi thao tác đường dẫn linh hoạt)
// ------------------------------------------------------------

std::vector<std::string> FileRepository::readLinesFrom(const std::string &path)
{
    return FileRepository(path).readLines();
}

bool FileRepository::safeWriteTo(const std::string &path, const std::vector<std::string> &lines)
{
    return FileRepository(path).safeWrite(lines);
}

bool FileRepository::backupFileTo(const std::string &path)
{
    return FileRepository(path).backupFile();
}