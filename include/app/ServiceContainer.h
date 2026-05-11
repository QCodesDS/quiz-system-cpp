#ifndef SERVICE_CONTAINER_H
#define SERVICE_CONTAINER_H

#include <memory>
#include <string>

// Forward declarations để tránh circular includes
class IUserRepository;
class IExamRepository;
class IResultRepository;
class IUserTypeHandler;
class IAuthService;

class IdGeneratorService;
class TeacherService;
class StudentService;
class UserService;
class AuthService;
class ExamService;
class QuizService;
class ReportService;

/**
 * @brief Dependency injection container — nơi duy nhất gọi `new`.
 *
 * @details
 * Khởi tạo và sở hữu toàn bộ repositories, handlers, và services
 * theo đúng thứ tự phụ thuộc:
 *   1. Handlers (không phụ thuộc gì)
 *   2. Repositories (phụ thuộc handlers)
 *   3. Services tầng 1 (phụ thuộc repositories)
 *   4. Services tầng 2 (phụ thuộc nhiều repositories)
 *   5. UserService facade (phụ thuộc các services khác)
 *
 * Các getter trả về raw pointer không sở hữu — container giữ
 * toàn bộ ownership qua std::unique_ptr.
 *
 * @note Phải gọi initialize() trước khi dùng bất kỳ getter nào.
 *
 * @example
 *   ServiceContainer container;
 *   container.initialize();
 *   IAuthService* auth = container.getAuthService();
 */
class ServiceContainer
{
public:
    ServiceContainer();
    ~ServiceContainer();

    /**
     * @brief Khởi tạo toàn bộ dependency graph.
     *
     * @details Gọi initializeRepositories() rồi initializeServices()
     * theo đúng thứ tự. Idempotent — gọi nhiều lần không có tác dụng.
     *
     * @throws std::runtime_error nếu bất kỳ bước khởi tạo nào thất bại.
     */
    void initialize();

    /// @brief Kiểm tra container đã được khởi tạo chưa.
    bool isInitialized() const { return initialized; }

    // ----------------------------------------------------------
    //  Getters — trả về non-owning raw pointer.
    //  Container giữ ownership; caller không được delete.
    // ----------------------------------------------------------

    /// @brief Trả về repository quản lý dữ liệu người dùng.
    IUserRepository *getUserRepository() const;

    /// @brief Trả về repository quản lý dữ liệu đề thi.
    IExamRepository *getExamRepository() const;

    /// @brief Trả về repository quản lý kết quả thi.
    IResultRepository *getResultRepository() const;

    /// @brief Trả về service sinh ID tuần tự cho user và exam.
    IdGeneratorService *getIdGeneratorService() const;

    /// @brief Trả về service quản lý CRUD giáo viên.
    TeacherService *getTeacherService() const;

    /// @brief Trả về service quản lý CRUD học sinh.
    StudentService *getStudentService() const;

    /**
     * @brief Trả về UserService facade.
     *
     * @details UserService ủy thác sang TeacherService và StudentService.
     * Dùng khi cần thao tác user mà không cần biết loại cụ thể.
     */
    UserService *getUserService() const;

    /// @brief Trả về service xác thực và quản lý phiên đăng nhập.
    IAuthService *getAuthService() const;

    /// @brief Trả về service quản lý CRUD đề thi và câu hỏi.
    ExamService *getExamService() const;

    /// @brief Trả về service xử lý logic làm bài thi.
    QuizService *getQuizService() const;

    /// @brief Trả về service thống kê và bảng xếp hạng.
    ReportService *getReportService() const;

private:
    // ----------------------------------------------------------
    //  Handlers — phân loại và serialize từng loại User.
    //  Dùng Handler Pattern để loại bỏ if/else trên role string
    //  trong UserRepository (tuân thủ Open/Closed Principle).
    // ----------------------------------------------------------
    std::unique_ptr<IUserTypeHandler> adminHandler;   ///< Xử lý Admin
    std::unique_ptr<IUserTypeHandler> teacherHandler; ///< Xử lý Teacher
    std::unique_ptr<IUserTypeHandler> studentHandler; ///< Xử lý Student

    // ----------------------------------------------------------
    //  Repositories — tầng persistence, không phụ thuộc services.
    // ----------------------------------------------------------
    std::unique_ptr<IUserRepository> userRepo;     ///< Dữ liệu người dùng
    std::unique_ptr<IExamRepository> examRepo;     ///< Dữ liệu đề thi
    std::unique_ptr<IResultRepository> resultRepo; ///< Kết quả thi (append-only)

    // ----------------------------------------------------------
    //  Services — tầng nghiệp vụ, phụ thuộc repositories.
    // ----------------------------------------------------------
    std::unique_ptr<IdGeneratorService> idGen;  ///< Sinh ID tuần tự
    std::unique_ptr<TeacherService> teacherSvc; ///< CRUD giáo viên
    std::unique_ptr<StudentService> studentSvc; ///< CRUD học sinh
    std::unique_ptr<UserService> userSvc;       ///< Facade người dùng
    std::unique_ptr<AuthService> authSvc;       ///< Xác thực & phiên
    std::unique_ptr<ExamService> examSvc;       ///< Quản lý đề thi
    std::unique_ptr<QuizService> quizSvc;       ///< Logic làm bài
    std::unique_ptr<ReportService> reportSvc;   ///< Thống kê & báo cáo

    bool initialized; ///< true sau khi initialize() thành công.

    /**
     * @brief Khởi tạo handlers và repositories.
     *
     * @details Tạo các handler theo Handler Pattern, truyền vào
     * UserRepository để loại bỏ if/else trên role string.
     * Phải được gọi trước initializeServices().
     */
    void initializeRepositories();

    /**
     * @brief Khởi tạo toàn bộ services theo thứ tự phụ thuộc.
     *
     * @details Thứ tự: L1 services (phụ thuộc repo) →
     * L2 services (phụ thuộc nhiều repo) → UserService facade.
     *
     * @throws std::runtime_error nếu repositories chưa được khởi tạo.
     */
    void initializeServices();
};

#endif // SERVICE_CONTAINER_H