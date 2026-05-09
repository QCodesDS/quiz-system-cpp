#ifndef SERVICE_CONTAINER_H
#define SERVICE_CONTAINER_H

#include <memory>
#include <string>

// Forward declarations to avoid circular includes
class IUserRepository;
class IExamRepository;
class IResultRepository;

class IdGeneratorService;
class TeacherService;
class StudentService;
class UserService;
class AuthService;
class ExamService;
class QuizService;
class ReportService;

// ============================================================
//  app/ServiceContainer.h
//
//  SINGLE RESPONSIBILITY: Dependency injection container
//  Reason to change: Only if dependency wiring changes
//
//  Wires all repositories and services in one place.
//  All `new` operations happen ONLY here.
//  All services use constructor injection.
//
//  Usage:
//    ServiceContainer container;
//    container.initialize();
//
//    UserService *userSvc = container.getUserService();
//    ExamService *examSvc = container.getExamService();
// ============================================================

class ServiceContainer
{
private:
    // ============================================================
    //  REPOSITORIES (lowest layer - no dependencies)
    // ============================================================
    std::unique_ptr<IUserRepository> userRepo;
    std::unique_ptr<IExamRepository> examRepo;
    std::unique_ptr<IResultRepository> resultRepo;

    // ============================================================
    //  SERVICES (depend on repositories)
    // ============================================================
    std::unique_ptr<IdGeneratorService> idGen;
    std::unique_ptr<TeacherService> teacherSvc;
    std::unique_ptr<StudentService> studentSvc;
    std::unique_ptr<UserService> userSvc;
    std::unique_ptr<AuthService> authSvc;
    std::unique_ptr<ExamService> examSvc;
    std::unique_ptr<QuizService> quizSvc;
    std::unique_ptr<ReportService> reportSvc;

    // Track initialization state
    bool initialized = false;

    // Private helpers for initialization
    void initializeRepositories();
    void initializeServices();

public:
    ServiceContainer();
    ~ServiceContainer();

    // Initialize all services - must be called before using getters
    void initialize();

    // ============================================================
    //  GETTERS: Return non-owning raw pointers
    //  Container retains ownership of all objects
    // ============================================================

    IUserRepository *getUserRepository() const;
    IExamRepository *getExamRepository() const;
    IResultRepository *getResultRepository() const;

    IdGeneratorService *getIdGeneratorService() const;
    TeacherService *getTeacherService() const;
    StudentService *getStudentService() const;
    UserService *getUserService() const;
    AuthService *getAuthService() const;
    ExamService *getExamService() const;
    QuizService *getQuizService() const;
    ReportService *getReportService() const;

    // Check if container is initialized
    bool isInitialized() const { return initialized; }
};

#endif // SERVICE_CONTAINER_H
