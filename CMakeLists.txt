cmake_minimum_required(VERSION 3.16)
project(quiz-system-cpp)

# ============================================================
#  C++ Standard
# ============================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================
#  Compiler Flags
# ============================================================
if(MSVC)
    # Visual Studio
    add_compile_options(/W4)
else()
    # GCC, Clang
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# ============================================================
#  Source Files
# ============================================================

# Recursively glob all .cpp files under source/
file(GLOB_RECURSE SOURCES
    "source/**/*.cpp"
)

# ============================================================
#  Include Directories
# ============================================================

# Add main include directory
include_directories(include)

# Add all subdirectories under include/ for completeness
include_directories(
    include/app
    include/core/interface
    include/core/models
    include/core/types
    include/repositories
    include/repositories/handlers
    include/services
    include/ui/console
    include/ui/menus
    include/ui/screens
)

# ============================================================
#  Executable
# ============================================================

add_executable(app ${SOURCES})

# ============================================================
#  Target Include Directories (modern CMake approach)
# ============================================================

target_include_directories(app PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ============================================================
#  Output Directory
# ============================================================

set_target_properties(app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# ============================================================
#  Build Information
# ============================================================

message(STATUS "Quiz System CMake Configuration")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Source Files: ${SOURCES}")
message(STATUS "  Executable: app")
