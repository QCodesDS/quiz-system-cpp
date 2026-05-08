# Architecture

## Tổng quan

Project áp dụng **3-Layer Architecture** kết hợp **Interface-driven design**:

```bash
UI  →  Services  →  Repositories  →  Core / Models
```

Mỗi layer chỉ phụ thuộc vào layer bên phải. Không bao giờ ngược lại.

---

## Dependency Rule

```bash
┌─────────────┐     ┌─────────────┐     ┌──────────────────┐     ┌──────────────┐
│     UI      │     │  Services   │     │  Repositories    │     │    Core      │
│             │  →  │             │  →  │                  │  →  │ Models       │
│ Chỉ in/nhận │     │ Nghiệp vụ   │     │ Đọc/ghi file     │     │ Interfaces   │
│ input       │     │ Không I/O   │     │ Không logic      │     │ Types        │
└─────────────┘     └─────────────┘     └──────────────────┘     └──────────────┘
```

| Layer           | Được phép                      | Không được phép                        |
| --------------- | ------------------------------ | -------------------------------------- |
| `ui/`           | Gọi services, dùng InputHelper | Tự tính điểm, tự ghi file, tự đọc data |
| `services/`     | Gọi repositories, gọi nhau     | `cout`, `cin`, trực tiếp mở file       |
| `repositories/` | Đọc/ghi file, dùng models      | Logic nghiệp vụ, `cout`                |
| `core/`         | Không gọi ai                   | Mọi thứ bên ngoài                      |

---

## Cấu trúc thư mục chi tiết

### `app/` — Entry point & Configuration

```bash
app/
├── main.cpp              # Khởi tạo hệ thống, vòng lặp đăng nhập, điều hướng menu
├── AppInitializer.h/.cpp # Load data từ file, khởi tạo các service/repository
├── config.h              # Đường dẫn file dữ liệu, giới hạn hệ thống
└── constants.h           # Named constants thay magic number
```

**`config.h`** — ví dụ nội dung:

```cpp
namespace Config {
    const string DATA_DIR        = "data/";
    const string STUDENTS_FILE   = DATA_DIR + "students.txt";
    const string EXAMS_FILE      = DATA_DIR + "exams.txt";
    const string QUESTIONS_FILE  = DATA_DIR + "questions.txt";
    const string RESULTS_FILE    = DATA_DIR + "results.txt";
}
```

**`constants.h`** — ví dụ nội dung:

```cpp
namespace Constants {
    const int    MAX_QUESTIONS   = 50;
    const int    MIN_QUESTIONS   = 5;
    const double PASS_SCORE      = 5.0;
    const int    MAX_LOGIN_TRIES = 3;
    const int    MIN_PASS_LENGTH = 6;
}
```

---

### `core/models/` — Layer 1: Data Models

Chỉ chứa dữ liệu thuần. Không có `cout`, không có file I/O, không có logic nghiệp vụ.

```bash
core/models/
├── User.h          # Abstract base class — id, username, password, fullName
├── Admin.h         # Kế thừa User, getRole() = "Admin"
├── Teacher.h       # Kế thừa User, thêm subject, assignedClass
├── Student.h       # Kế thừa User, thêm className, gender, age, phone
├── Question.h      # Câu hỏi trắc nghiệm 4 lựa chọn, có Difficulty
├── Exam.h          # Đề thi, chứa vector<Question>, có ExamType
└── ExamResult.h    # Bản ghi 1 lần thi, có score, durationSeconds, submittedAt
```

**Quy tắc trong models:**

- Tất cả fields là `private` hoặc `protected`
- Truy cập qua getters/setters
- Setter có thể validate (ví dụ `setPassword` kiểm tra độ dài)
- Không có `cin`, `cout`, không mở file

---

### `core/interfaces/` — Layer 1: Contracts

Pure abstract classes — toàn bộ method là `= 0`. Định nghĩa contract mà không quan tâm implementation.

```bash
core/interfaces/
├── IRepository.h        # Base: save(), load(), backup()
├── IUserRepository.h    # findById(), findByUsername(), usernameExists()
├── IExamRepository.h    # findById(), findByTeacher(), examIdExists()
├── IResultRepository.h  # findByStudent(), append()
├── IAuthService.h       # login(), logout(), changePassword()
├── IExamService.h       # addExam(), addQuestion(), hasEditPermission()
├── IQuizService.h       # canAttempt(), calculateScore(), createResult()
└── IReportService.h     # getLeaderboard(), getScoresBySubject(), getSystemStats()
```

**Tại sao cần interface?**

Services chỉ phụ thuộc vào interface, không phụ thuộc vào implementation cụ thể:

```cpp
// ExamService chỉ biết IExamRepository — không biết data lưu ở file hay DB
class ExamService : public IExamService {
    IExamRepository* repo;   // ← interface, không phải ExamRepository cụ thể
public:
    ExamService(IExamRepository* r) : repo(r) {}
};
```

---

### `core/types/` — Enums & Typedefs

```bash
core/types/
├── enums.h      # ExamType, UserRole, Gender, Difficulty
└── typedefs.h   # using StudentId = int; using ExamId = int; ...
```

**`enums.h`** — thay toàn bộ magic number:

```cpp
enum class ExamType   { Official = 1, Practice = 2 };
enum class UserRole   { Admin, Teacher, Student };
enum class Difficulty { Easy = 1, Medium = 2, Hard = 3 };
enum class Gender     { Male, Female, Other };
```

---

### `repositories/` — Layer 2a: Data Access

Implement interface, đọc/ghi file. Không có logic nghiệp vụ.

```bash
repositories/
├── FileRepository.h/.cpp    # Base class: mở file, đọc dòng, ghi safe (temp→rename)
├── UserRepository.h/.cpp    # Impl IUserRepository — parse dòng → tạo đúng loại object
├── ExamRepository.h/.cpp    # Impl IExamRepository — load 2 file rồi assemble
└── ResultRepository.h/.cpp  # Impl IResultRepository — append-only khi ghi
```

**`FileRepository`** cung cấp:

- `openFile()` / `closeFile()`
- `readLines()` — đọc toàn bộ dòng vào `vector<string>`
- `safeWrite()` — ghi ra file tạm, nếu thành công mới rename → tránh mất dữ liệu nếu crash

---

### `services/` — Layer 2b: Business Logic

Implement interface, chứa toàn bộ nghiệp vụ. Không có `cin/cout`.

```bash
services/
├── AuthService.h/.cpp   # login(), logout(), changePassword(), getCurrentUser()
├── UserService.h/.cpp   # CRUD Teacher & Student, kiểm tra ràng buộc
├── ClassService.h/.cpp  # Quản lý lớp, gán GV, lấy HS theo lớp
├── ExamService.h/.cpp   # CRUD đề thi & câu hỏi, kiểm tra quyền
├── QuizService.h/.cpp   # canAttempt(), calculateScore(), createResult()
└── ReportService.h/.cpp # getLeaderboard(), getScoresBySubject(), getSystemStats()
```

---

### `ui/` — Layer 3: Presentation

Chỉ in ra màn hình và nhận input. Gọi service để xử lý, không tự tính toán.

```bash
ui/
├── menus/
│   ├── AdminMenu.h/.cpp    # Menu điều hướng Admin
│   ├── TeacherMenu.h/.cpp  # Menu điều hướng Teacher
│   └── StudentMenu.h/.cpp  # Menu điều hướng Student
│
├── screens/
│   ├── LoginScreen.h/.cpp  # Nhận username/password → gọi AuthService::login()
│   ├── ExamScreen.h/.cpp   # Hiển thị từng câu, đo thời gian, gọi QuizService
│   └── ResultScreen.h/.cpp # In kết quả, lịch sử, bảng xếp hạng
│
└── console/
    ├── InputHelper.h/.cpp     # readInt(), readLine(), readIntInRange(), validators
    ├── TableRenderer.h/.cpp   # In bảng căn chỉnh cột tự động
    └── ColorPrinter.h/.cpp    # ANSI color: printSuccess(), printError(), printWarning()
```

---

### `data/` — File lưu trữ

```bash
data/
├── questions/
│   ├── math.txt
│   ├── cpp.txt
│   └── physics.txt
├── students.txt
├── exams.txt
└── results.txt
```

Format chi tiết xem tại [`database-format.md`](./database-format.md)

---

### `docs/` — Tài liệu

```bash
docs/
├── README.md           # Tổng quan project
├── architecture.md     # File này
├── class-diagram.md    # Class diagram (Mermaid)
└── database-format.md  # Format file .txt
```

---

## Naming Convention

| Loại              | Convention         | Ví dụ                              |
| ----------------- | ------------------ | ---------------------------------- |
| Class / Struct    | `PascalCase`       | `ExamResult`, `UserService`        |
| Interface         | `I` + `PascalCase` | `IRepository`, `IAuthService`      |
| File              | `PascalCase`       | `ExamService.h`, `InputHelper.cpp` |
| Method / Function | `camelCase`        | `calculateScore()`, `findExam()`   |
| Field             | `camelCase`        | `studentId`, `correctChoice`       |
| Enum              | `PascalCase`       | `ExamType::Official`               |
| Constant          | `UPPER_SNAKE_CASE` | `MAX_QUESTIONS`, `PASS_SCORE`      |
| Folder            | `lowercase`        | `models/`, `services/`, `ui/`      |
