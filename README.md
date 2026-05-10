# Quiz System — C++ Console Application

> Chương trình thi trắc nghiệm trên console, xây dựng bằng C++ theo mô hình 3-layer architecture và các nguyên tắc OOP.

---

## Tính năng

### Admin

- Quản lý tài khoản Teacher và Student (CRUD)
- Quản lý lớp học, phân công giáo viên chủ nhiệm
- Quản lý toàn bộ đề thi và câu hỏi
- Xem thống kê tổng quan hệ thống
- Xem bảng xếp hạng toàn trường

### Teacher

- Tạo, sửa, xoá đề thi và câu hỏi (chỉ đề của mình)
- Xem danh sách học sinh trong lớp chủ nhiệm
- Xem kết quả thi theo đề
- Thống kê điểm theo môn

### Student

- Xem danh sách đề thi có thể làm
- Làm bài thi (Official: 1 lần / Practice: không giới hạn)
- Xem lịch sử thi và điểm số
- Xem bảng xếp hạng

---

## Cấu trúc thư mục

```bash
quiz-system-cpp/
├── app/                  # Entry point & config
├── core/
│   ├── models/           # Data models (User, Exam, Question...)
│   ├── interfaces/       # Pure abstract contracts (IRepository, IService...)
│   └── types/            # Enums, typedefs
├── repositories/         # File I/O layer
├── services/             # Business logic layer
├── ui/
│   ├── menus/            # Menu điều hướng
│   ├── screens/          # Màn hình nghiệp vụ
│   └── console/          # Helpers: input, color, table
└── data/                 # File lưu trữ dữ liệu (.txt)
```

> Chi tiết từng file xem tại [`architecture.md`](./architecture.md)

---

## Yêu cầu hệ thống

| Thành phần   | Yêu cầu                 |
| ------------ | ----------------------- |
| Compiler     | g++ 11+ hoặc MSVC 2019+ |
| C++ Standard | C++17                   |
| Build system | CMake 3.15+             |
| OS           | Windows / Linux / macOS |

---

## Build & Chạy

```bash
# Clone project
git clone https://github.com/QCodesDS/quiz-system-cpp.git
cd quiz-system-cpp

# Build
cmake -B build
cmake --build build

# Chạy
./build/QuizSystem
```

---

## Tài khoản mặc định

| Role  | Username | Password    |
| ----- | -------- | ----------- |
| Admin | `admin`  | `Admin@123` |

> Admin tạo tài khoản Teacher và Student sau khi đăng nhập lần đầu.

---

## OOP Principles

| Tính chất         | Thể hiện ở đâu                                                                          |
| ----------------- | --------------------------------------------------------------------------------------- |
| **Encapsulation** | Fields `private/protected` trong tất cả model, truy cập qua getter/setter               |
| **Inheritance**   | `User → Admin, Teacher, Student` · `FileRepository → UserRepository, ExamRepository...` |
| **Polymorphism**  | `User*` gọi `getRole()`, `toFileString()` khác nhau tuỳ object thực tế                  |
| **Abstraction**   | `IRepository`, `IAuthService`, `IExamService`... định nghĩa contract, ẩn implementation |

---

## Tài liệu

| File                                         | Nội dung                                           |
| -------------------------------------------- | -------------------------------------------------- |
| [`architecture.md`](./architecture.md)       | Cấu trúc thư mục, mô tả từng file, dependency rule |
| [`class-diagram.md`](./class-diagram.md)     | Class diagram toàn bộ hệ thống (Mermaid)           |
| [`database-format.md`](./database-format.md) | Format lưu trữ dữ liệu trong file .txt             |
