# Class Diagram

## 1. Model Hierarchy — Kế thừa từ User

```mermaid
classDiagram
    class User {
        <<abstract>>
        #int id
        #string username
        #string password
        #string fullName
        +User(id, username, password, fullName)
        +getId() int
        +getUsername() string
        +getFullName() string
        +setPassword(newPass) bool
        +getRole()* string
        +toFileString()* string
        +displayInfo()* void
    }

    class Admin {
        +getRole() string
        +toFileString() string
        +displayInfo() void
    }

    class Teacher {
        -string subject
        -string assignedClass
        +getSubject() string
        +getAssignedClass() string
        +setAssignedClass(cls) void
        +getRole() string
        +toFileString() string
        +displayInfo() void
    }

    class Student {
        -string className
        -Gender gender
        -int age
        -string phone
        +getClassName() string
        +getGender() Gender
        +getAge() int
        +getPhone() string
        +getRole() string
        +toFileString() string
        +displayInfo() void
    }

    User <|-- Admin
    User <|-- Teacher
    User <|-- Student
```

---

## 2. Exam & Question

```mermaid
classDiagram
    class Question {
        -int id
        -string content
        -string choices[4]
        -int correctChoice
        -Difficulty difficulty
        +getContent() string
        +getChoice(i) string
        +getCorrectChoice() int
        +getDifficulty() Difficulty
        +toFileString() string
    }

    class Exam {
        -int examId
        -string subject
        -int durationMinutes
        -ExamType examType
        -int teacherId
        -vector~Question~ questions
        +getExamId() int
        +getSubject() string
        +getType() ExamType
        +getTeacherId() int
        +addQuestion(q) bool
        +removeQuestion(id) bool
        +getQuestions() vector~Question~
        +getQuestionCount() int
    }

    class ExamResult {
        -int studentId
        -string studentName
        -int examId
        -string subject
        -double score
        -int durationSeconds
        -ExamType examType
        -string submittedAt
        +getScore() double
        +getDurationSeconds() int
        +getSubmittedAt() string
        +isPassed() bool
        +toFileString() string
    }

    Exam "1" *-- "many" Question
    ExamResult --> Exam
    ExamResult --> Student
```

---

## 3. Interface Layer

```mermaid
classDiagram
    class IRepository {
        <<interface>>
        +save(data)* bool
        +load()* vector
        +backup()* bool
    }

    class IUserRepository {
        <<interface>>
        +save(users)* bool
        +load()* vector~User~
        +findById(id)* User
        +findByUsername(u)* User
        +usernameExists(u)* bool
        +backup()* bool
    }

    class IExamRepository {
        <<interface>>
        +save(exams)* bool
        +load()* vector~Exam~
        +findById(id)* Exam
        +findByTeacher(tid)* vector~Exam~
        +examIdExists(id)* bool
        +backup()* bool
    }

    class IResultRepository {
        <<interface>>
        +load()* vector~ExamResult~
        +append(result)* bool
        +findByStudent(sid)* vector~ExamResult~
        +backup()* bool
    }

    class IAuthService {
        <<interface>>
        +login(u, p)* User
        +logout()* void
        +changePassword(user, pass)* bool
        +getCurrentUser()* User
        +validateCredentials(u, p)* bool
    }

    class IExamService {
        <<interface>>
        +addExam(...)* bool
        +removeExam(id)* bool
        +addQuestion(examId, q)* bool
        +updateQuestion(examId, id, q)* bool
        +removeQuestion(examId, id)* bool
        +findExam(id)* Exam
        +hasEditPermission(exam, tid)* bool
    }

    class IQuizService {
        <<interface>>
        +canAttempt(sid, examId)* bool
        +calculateScore(exam, answers)* double
        +createResult(...)* ExamResult
        +isPassed(score)* bool
    }

    class IReportService {
        <<interface>>
        +getLeaderboard()* vector~LeaderboardEntry~
        +getScoresBySubject(sub)* vector~LeaderboardEntry~
        +getSystemStats()* SystemStats
        +getAverageScore(sid)* double
    }

    IRepository <|-- IUserRepository
    IRepository <|-- IExamRepository
    IRepository <|-- IResultRepository
```

---

## 4. Repository Layer — Implement Interfaces

```mermaid
classDiagram
    class FileRepository {
        #string filePath
        #openFile(path) bool
        #readLines() vector~string~
        #writeLines(lines) bool
        #safeWrite(lines) bool
    }

    class UserRepository {
        +save(users) bool
        +load() vector~User~
        +findById(id) User
        +findByUsername(u) User
        +usernameExists(u) bool
        +backup() bool
        -parseLine(line) User
    }

    class ExamRepository {
        +save(exams) bool
        +load() vector~Exam~
        +findById(id) Exam
        +findByTeacher(tid) vector~Exam~
        +examIdExists(id) bool
        +backup() bool
        -loadExams() vector~Exam~
        -loadQuestions(exams) void
    }

    class ResultRepository {
        +load() vector~ExamResult~
        +append(result) bool
        +findByStudent(sid) vector~ExamResult~
        +backup() bool
    }

    FileRepository <|-- UserRepository
    FileRepository <|-- ExamRepository
    FileRepository <|-- ResultRepository

    IUserRepository <|.. UserRepository
    IExamRepository <|.. ExamRepository
    IResultRepository <|.. ResultRepository
```

---

## 5. Service Layer — Implement Interfaces

```mermaid
classDiagram
    class AuthService {
        -IUserRepository* userRepo
        -User* currentUser
        +login(u, p) User
        +logout() void
        +changePassword(user, pass) bool
        +getCurrentUser() User
        +validateCredentials(u, p) bool
    }

    class UserService {
        -IUserRepository* userRepo
        +addTeacher(...) bool
        +addStudent(...) bool
        +updateUser(id, ...) bool
        +removeUser(id) bool
        +getAllTeachers() vector~Teacher~
        +getAllStudents() vector~Student~
        -isIdExists(id) bool
        -isUsernameExists(u) bool
    }

    class ExamService {
        -IExamRepository* examRepo
        +addExam(...) bool
        +removeExam(id) bool
        +addQuestion(examId, q) bool
        +updateQuestion(examId, id, q) bool
        +removeQuestion(examId, id) bool
        +findExam(id) Exam
        +hasEditPermission(exam, tid) bool
    }

    class QuizService {
        -IExamRepository* examRepo
        -IResultRepository* resultRepo
        +canAttempt(sid, examId) bool
        +calculateScore(exam, answers) double
        +createResult(...) ExamResult
        +isPassed(score) bool
    }

    class ReportService {
        -IUserRepository* userRepo
        -IResultRepository* resultRepo
        +getLeaderboard() vector~LeaderboardEntry~
        +getScoresBySubject(sub) vector~LeaderboardEntry~
        +getSystemStats() SystemStats
        +getAverageScore(sid) double
    }

    IAuthService <|.. AuthService
    IExamService <|.. ExamService
    IQuizService <|.. QuizService
    IReportService <|.. ReportService

    AuthService --> IUserRepository
    UserService --> IUserRepository
    ExamService --> IExamRepository
    QuizService --> IExamRepository
    QuizService --> IResultRepository
    ReportService --> IUserRepository
    ReportService --> IResultRepository
```

---

## 6. UI Layer

```mermaid
classDiagram
    class LoginScreen {
        -IAuthService* authService
        +show() User
    }

    class ExamScreen {
        -IQuizService* quizService
        -IExamRepository* examRepo
        -IResultRepository* resultRepo
        +show(student, exam) void
        -displayQuestion(q, index) void
        -collectAnswers(exam) vector~int~
        -measureTime() int
    }

    class ResultScreen {
        -IReportService* reportService
        +showResult(result) void
        +showHistory(results) void
        +showLeaderboard(entries) void
    }

    class AdminMenu {
        -IUserService* userService
        -IExamService* examService
        -IReportService* reportService
        +show(admin) void
    }

    class TeacherMenu {
        -IExamService* examService
        -IReportService* reportService
        +show(teacher) void
    }

    class StudentMenu {
        -IQuizService* quizService
        -IExamRepository* examRepo
        +show(student) void
    }

    class InputHelper {
        +readInt(prompt)$ int
        +readIntInRange(prompt, min, max)$ int
        +readLine(prompt)$ string
        +isValidUsername(s)$ bool
        +isValidPassword(s)$ bool
        +isValidPhone(s)$ bool
    }

    class TableRenderer {
        +printTable(headers, rows)$ void
        +printSeparator(width)$ void
    }

    class ColorPrinter {
        +printSuccess(msg)$ void
        +printError(msg)$ void
        +printWarning(msg)$ void
        +printHighlight(msg)$ void
    }

    AdminMenu --> IExamService
    TeacherMenu --> IExamService
    StudentMenu --> IQuizService
    ExamScreen --> IQuizService
    ResultScreen --> IReportService
    LoginScreen --> IAuthService
```
