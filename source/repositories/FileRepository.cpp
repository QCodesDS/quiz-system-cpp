#include "FileRepository.h"

#include <fstream>
#include <sstream>
#include <cstdio>
#include <stdexcept>

// ============================================================
//  repositories/FileRepository.cpp
// ============================================================

FileRepository::FileRepository(const std::string &filePath)
    : filePath(filePath) {}

// ------------------------------------------------------------
//  readLines
//  Đọc toàn bộ file, bỏ qua:
//    - dòng trống
//    - dòng bắt đầu bằng '#' (comment)
// ------------------------------------------------------------
std::vector<std::string> FileRepository::readLines() const
{
    std::vector<std::string> result;

    std::ifstream file(filePath);
    if (!file.is_open())
    {
        // File chưa tồn tại → trả vector rỗng, không crash
        return result;
    }

    std::string line;
    while (std::getline(file, line))
    {
        // Trim '\r' nếu file có CRLF line ending (Windows)
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }
        if (line.empty() || line[0] == '#')
            continue;
        result.push_back(line);
    }

    return result;
}

// ------------------------------------------------------------
//  safeWrite
//  Ghi an toàn: write .tmp → rename, tránh mất data khi crash
// ------------------------------------------------------------
bool FileRepository::safeWrite(const std::vector<std::string> &lines) const
{
    std::string tmpPath = filePath + ".tmp";

    // Bước 1: ghi ra file tạm
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

    // Bước 2: rename .tmp → file gốc
    // std::rename trả 0 nếu thành công
    if (std::rename(tmpPath.c_str(), filePath.c_str()) != 0)
    {
        std::remove(tmpPath.c_str());
        return false;
    }

    return true;
}

// ------------------------------------------------------------
//  backupFile
//  Copy filePath → filePath + ".bak"
// ------------------------------------------------------------
bool FileRepository::backupFile() const
{
    std::ifstream src(filePath, std::ios::binary);
    if (!src.is_open())
        return false; // không có file gốc → không cần backup

    std::string bakPath = filePath + ".bak";
    std::ofstream dst(bakPath, std::ios::binary | std::ios::trunc);
    if (!dst.is_open())
        return false;

    dst << src.rdbuf();
    return dst.good();
}

// ------------------------------------------------------------
//  appendLine
//  Chỉ dùng cho results.txt — không bao giờ rewrite toàn bộ
// ------------------------------------------------------------
bool FileRepository::appendLine(const std::string &line) const
{
    std::ofstream file(filePath, std::ios::app);
    if (!file.is_open())
        return false;
    file << line << "\n";
    return file.good();
}

// ------------------------------------------------------------
//  splitLine
//  Tách "A|B|C" → {"A", "B", "C"}
//  Giữ nguyên field trống (ví dụ "A||C" → {"A", "", "C"})
// ------------------------------------------------------------
std::vector<std::string> FileRepository::splitLine(const std::string &line, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream stream(line);

    while (std::getline(stream, token, delimiter))
    {
        tokens.push_back(token);
    }

    // Nếu dòng kết thúc bằng delimiter → thêm field rỗng
    if (!line.empty() && line.back() == delimiter)
    {
        tokens.push_back("");
    }

    return tokens;
}

// ------------------------------------------------------------
//  Static overloads — thao tác với filePath bất kỳ
// ------------------------------------------------------------

std::vector<std::string> FileRepository::readLinesFrom(const std::string &path)
{
    FileRepository tmp(path);
    return tmp.readLines();
}

bool FileRepository::safeWriteTo(const std::string &path,
                                 const std::vector<std::string> &lines)
{
    FileRepository tmp(path);
    return tmp.safeWrite(lines);
}

bool FileRepository::backupFileTo(const std::string &path)
{
    FileRepository tmp(path);
    return tmp.backupFile();
}