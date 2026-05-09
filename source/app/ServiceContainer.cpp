#include "ServiceContainer.h"

// Include all repository implementations
#include "UserRepository.h"
#include "ExamRepository.h"
#include "ResultRepository.h"

// Include all handler implementations (USER TYPE HANDLERS)
#include "handlers/AdminHandler.h"
#include "handlers/TeacherHandler.h"
#include "handlers/StudentHandler.h"

// Include all repository interfaces
#include "IUserRepository.h"
#include "IExamRepository.h"
#include "IResultRepository.h"
#include "handlers/IUserTypeHandler.h"

// Include all service implementations
#include "IdGeneratorService.h"
#include "TeacherService.h"
#include "StudentService.h"
#include "UserService.h"
#include "AuthService.h"
#include "ExamService.h"
#include "QuizService.h"
#include "ReportService.h"

#include <stdexcept>

// ============================================================
//  app/ServiceContainer.cpp
//
//  Centralizes all dependency injection and wiring.
//  This is the ONLY place where `new` is called.
// ============================================================

// ============================================================
//  File paths (could be moved to config.h in future)
// ============================================================

static const std::string DATA_DIR = "data/";
static const std::string ADMIN_FILE = DATA_DIR + "admins.txt";
static const std::string TEACHER_FILE = DATA_DIR + "teachers.txt";
static const std::string STUDENT_FILE = DATA_DIR + "students.txt";
static const std::string EXAM_FILE = DATA_DIR + "exams.txt";
static const std::string QUESTIONS_DIR = DATA_DIR + "questions/";
static const std::string RESULT_FILE = DATA_DIR + "results.txt";

// ============================================================
//  Constructor
// ============================================================

ServiceContainer::ServiceContainer()
    : initialized(false) {}

// ============================================================
//  Destructor
// ============================================================

ServiceContainer::~ServiceContainer()
{
    // unique_ptr handles cleanup automatically
    // Destruction order: services first (they depend on repos),
    // then repositories. unique_ptr ensures proper order.
}

// ============================================================
//  Initialize: Main entry point
// ============================================================

void ServiceContainer::initialize()
{
    if (initialized)
        return; // Already initialized

    try
    {
        initializeRepositories();
        initializeServices();
        initialized = true;
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("Failed to initialize ServiceContainer: ") + e.what());
    }
}

// ============================================================
//  Initialize Repositories (lowest layer - no dependencies)
// ============================================================

void ServiceContainer::initializeRepositories()
{
    // ============================================================
    //  Create User Type Handlers (HANDLER PATTERN)
    //
    //  Handlers eliminate if/else on role strings in UserRepository.
    //  Each handler manages serialization of one user type.
    //  Adding new type = new handler only (OPEN/CLOSED PRINCIPLE)
    // ============================================================

    adminHandler = std::make_unique<AdminHandler>(ADMIN_FILE);
    teacherHandler = std::make_unique<TeacherHandler>(TEACHER_FILE);
    studentHandler = std::make_unique<StudentHandler>(STUDENT_FILE);

    // Collect handler pointers for UserRepository
    std::vector<IUserTypeHandler *> handlers;
    handlers.push_back(adminHandler.get());
    handlers.push_back(teacherHandler.get());
    handlers.push_back(studentHandler.get());

    // UserRepository: Uses handlers instead of if/else (HANDLER PATTERN)
    userRepo = std::make_unique<UserRepository>(DATA_DIR, handlers);

    // ExamRepository: Load exams and questions
    examRepo = std::make_unique<ExamRepository>(
        EXAM_FILE,
        QUESTIONS_DIR);

    // ResultRepository: Append-only results log
    resultRepo = std::make_unique<ResultRepository>(
        RESULT_FILE);
}

// ============================================================
//  Initialize Services (depend on repositories)
// ============================================================

void ServiceContainer::initializeServices()
{
    // Verify repositories are initialized
    if (!userRepo || !examRepo || !resultRepo)
        throw std::runtime_error("Repositories not initialized");

    // ============================================================
    //  Services that depend on repositories only
    // ============================================================

    // IdGeneratorService: Generates sequential IDs
    idGen = std::make_unique<IdGeneratorService>(userRepo.get());

    // TeacherService: Teacher CRUD operations
    teacherSvc = std::make_unique<TeacherService>(userRepo.get());

    // StudentService: Student CRUD operations
    studentSvc = std::make_unique<StudentService>(userRepo.get());

    // AuthService: Authentication and session management
    authSvc = std::make_unique<AuthService>(userRepo.get());

    // ExamService: Exam management
    examSvc = std::make_unique<ExamService>(examRepo.get());

    // ============================================================
    //  Services that depend on multiple repositories
    // ============================================================

    // QuizService: Quiz taking logic
    quizSvc = std::make_unique<QuizService>(
        examRepo.get(),
        resultRepo.get());

    // ReportService: Statistics and leaderboards
    reportSvc = std::make_unique<ReportService>(
        userRepo.get(),
        resultRepo.get());

    // ============================================================
    //  High-level service that depends on other services
    // ============================================================

    // UserService: Facade for user operations (now depends on dedicated services)
    userSvc = std::make_unique<UserService>(
        userRepo.get(),
        idGen.get(),
        teacherSvc.get(),
        studentSvc.get());
}

// ============================================================
//  GETTERS: Return non-owning raw pointers
// ============================================================

IUserRepository *ServiceContainer::getUserRepository() const
{
    return userRepo.get();
}

IExamRepository *ServiceContainer::getExamRepository() const
{
    return examRepo.get();
}

IResultRepository *ServiceContainer::getResultRepository() const
{
    return resultRepo.get();
}

IdGeneratorService *ServiceContainer::getIdGeneratorService() const
{
    return idGen.get();
}

TeacherService *ServiceContainer::getTeacherService() const
{
    return teacherSvc.get();
}

StudentService *ServiceContainer::getStudentService() const
{
    return studentSvc.get();
}

UserService *ServiceContainer::getUserService() const
{
    return userSvc.get();
}

AuthService *ServiceContainer::getAuthService() const
{
    return authSvc.get();
}

ExamService *ServiceContainer::getExamService() const
{
    return examSvc.get();
}

QuizService *ServiceContainer::getQuizService() const
{
    return quizSvc.get();
}

ReportService *ServiceContainer::getReportService() const
{
    return reportSvc.get();
}
