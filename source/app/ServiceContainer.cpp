/**
 * @file ServiceContainer.cpp
 * @brief Triển khai dependency injection container.
 *
 * @details Đây là nơi DUY NHẤT trong toàn bộ project gọi `new`
 * (thông qua std::make_unique). Tất cả services và repositories
 * nhận dependency qua constructor injection.
 */

#include "ServiceContainer.h"

#include "UserRepository.h"
#include "ExamRepository.h"
#include "ResultRepository.h"

#include "handlers/AdminHandler.h"
#include "handlers/TeacherHandler.h"
#include "handlers/StudentHandler.h"

#include "IUserRepository.h"
#include "IExamRepository.h"
#include "IResultRepository.h"
#include "IUserTypeHandler.h"

#include "IdGeneratorService.h"
#include "TeacherService.h"
#include "StudentService.h"
#include "UserService.h"
#include "AuthService.h"
#include "ExamService.h"
#include "QuizService.h"
#include "ReportService.h"

#include <stdexcept>

// ------------------------------------------------------------------
//  Đường dẫn file data — tập trung tại đây để dễ thay đổi.
//  TODO: Chuyển sang config.h khi project lớn hơn.
// ------------------------------------------------------------------
static const std::string DATA_DIR = "data/";
static const std::string ADMIN_FILE = DATA_DIR + "admins.txt";
static const std::string TEACHER_FILE = DATA_DIR + "teachers.txt";
static const std::string STUDENT_FILE = DATA_DIR + "students.txt";
static const std::string EXAM_FILE = DATA_DIR + "exams.txt";
static const std::string QUESTIONS_DIR = DATA_DIR + "questions/";
static const std::string RESULT_FILE = DATA_DIR + "results.txt";

// ------------------------------------------------------------------
//  Constructor / Destructor
// ------------------------------------------------------------------

ServiceContainer::ServiceContainer()
    : initialized(false)
{
}

ServiceContainer::~ServiceContainer()
{
    // std::unique_ptr tự dọn dẹp theo đúng thứ tự ngược:
    // services trước (chúng phụ thuộc repos),
    // repositories sau (chúng phụ thuộc handlers),
    // handlers cuối cùng.
}

// ------------------------------------------------------------------
//  initialize
// ------------------------------------------------------------------

void ServiceContainer::initialize()
{
    // Idempotent — tránh khởi tạo lại nếu đã chạy rồi.
    if (initialized)
        return;

    try
    {
        initializeRepositories();
        initializeServices();
        initialized = true;
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(
            std::string("ServiceContainer::initialize() thất bại: ") + e.what());
    }
}

// ------------------------------------------------------------------
//  initializeRepositories
// ------------------------------------------------------------------

void ServiceContainer::initializeRepositories()
{
    // Tạo handlers theo Handler Pattern.
    // Mỗi handler biết cách serialize/deserialize đúng một loại User.
    // Thêm loại User mới = thêm handler mới, KHÔNG sửa UserRepository.
    adminHandler = std::make_unique<AdminHandler>(ADMIN_FILE);
    teacherHandler = std::make_unique<TeacherHandler>(TEACHER_FILE);
    studentHandler = std::make_unique<StudentHandler>(STUDENT_FILE);

    // Truyền raw pointer (non-owning) vào UserRepository.
    // Container vẫn giữ ownership qua unique_ptr ở trên.
    std::vector<IUserTypeHandler *> handlers = {
        adminHandler.get(),
        teacherHandler.get(),
        studentHandler.get()};

    userRepo = std::make_unique<UserRepository>(DATA_DIR, handlers);
    examRepo = std::make_unique<ExamRepository>(EXAM_FILE, QUESTIONS_DIR);
    resultRepo = std::make_unique<ResultRepository>(RESULT_FILE);
}

// ------------------------------------------------------------------
//  initializeServices
// ------------------------------------------------------------------

void ServiceContainer::initializeServices()
{
    if (!userRepo || !examRepo || !resultRepo)
        throw std::runtime_error("Repositories chưa được khởi tạo.");

    // Tầng 1 — services chỉ phụ thuộc một repository.
    idGen = std::make_unique<IdGeneratorService>(userRepo.get());
    teacherSvc = std::make_unique<TeacherService>(userRepo.get());
    studentSvc = std::make_unique<StudentService>(userRepo.get());
    authSvc = std::make_unique<AuthService>(userRepo.get());
    examSvc = std::make_unique<ExamService>(examRepo.get());

    // Tầng 2 — services phụ thuộc nhiều repositories.
    quizSvc = std::make_unique<QuizService>(examRepo.get(), resultRepo.get());
    reportSvc = std::make_unique<ReportService>(userRepo.get(), resultRepo.get());

    // Tầng 3 — UserService facade phụ thuộc các services tầng 1.
    // Ủy thác sang TeacherService/StudentService thay vì tự xử lý.
    userSvc = std::make_unique<UserService>(
        userRepo.get(),
        idGen.get(),
        teacherSvc.get(),
        studentSvc.get());
}

// ------------------------------------------------------------------
//  Getters
// ------------------------------------------------------------------

IUserRepository *ServiceContainer::getUserRepository() const { return userRepo.get(); }
IExamRepository *ServiceContainer::getExamRepository() const { return examRepo.get(); }
IResultRepository *ServiceContainer::getResultRepository() const { return resultRepo.get(); }
IdGeneratorService *ServiceContainer::getIdGeneratorService() const { return idGen.get(); }
TeacherService *ServiceContainer::getTeacherService() const { return teacherSvc.get(); }
StudentService *ServiceContainer::getStudentService() const { return studentSvc.get(); }
UserService *ServiceContainer::getUserService() const { return userSvc.get(); }
AuthService *ServiceContainer::getAuthService() const { return authSvc.get(); }
ExamService *ServiceContainer::getExamService() const { return examSvc.get(); }
QuizService *ServiceContainer::getQuizService() const { return quizSvc.get(); }
ReportService *ServiceContainer::getReportService() const { return reportSvc.get(); }