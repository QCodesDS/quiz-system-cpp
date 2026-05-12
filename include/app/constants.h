#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

// --- Colors for console output (Windows-specific) ---
namespace Color
{
    const std::string RESET = "\033[0m";
    const std::string BOLD = "\033[1m";
    const std::string CYAN = "\033[38;5;81m";    // viền menu admin
    const std::string YELLOW = "\033[38;5;220m"; // highlight tên user
    const std::string GREEN = "\033[38;5;83m";   // success
    const std::string RED = "\033[38;5;203m";    // error
    const std::string DIM = "\033[2m";           // text phụ
    const std::string PURPLE = "\033[38;5;135m";
}

#endif // CONSTANTS_H