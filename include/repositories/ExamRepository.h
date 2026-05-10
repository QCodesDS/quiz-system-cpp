#ifndef EXAM_REPOSITORY_H
#define EXAM_REPOSITORY_H

#include "repositories/FileRepository.h"
#include "core/interface/IExamRepository.h"

/**
 * @class ExamRepository
 * @brief Quản lý dữ liệu đề thi và câu hỏi.
 *
 * Cơ chế lưu trữ phân tán:
 * - Thông tin chung (Header): Lưu tại 'exams.txt'.
 * - Chi tiết câu hỏi: Lưu tại thư mục 'questions/' chia theo từng môn (ví dụ: math.txt).
 *
 * Lớp này sử dụng cơ chế Cache để tối ưu hiệu năng đọc file.
 */
class ExamRepository : public FileRepository, public IExamRepository
{
private:
    std::string _examFile;     ///< Đường dẫn tệp header đề thi.
    std::string _questionsDir; ///< Thư mục chứa các tệp câu hỏi.

    // ------------------------------------------------------------
    //  Parsing Helpers
    // ------------------------------------------------------------

    Exam parseExam(const std::vector<std::string> &fields) const;
    Question parseQuestion(const std::vector<std::string> &fields) const;

    // ------------------------------------------------------------
    //  File & Data Helpers
    // ------------------------------------------------------------

    static std::string subjectToFilename(const std::string &subject);
    std::vector<Question> loadQuestionsFromFile(const std::string &path) const;
    std::vector<Exam> loadExamHeaders() const;

    /**
     * @brief Hợp nhất câu hỏi vào các đối tượng Exam tương ứng.
     */
    void assembleQuestions(std::vector<Exam> &exams) const;

    // ------------------------------------------------------------
    //  Cache Management
    // ------------------------------------------------------------

    std::vector<Exam> _cache;
    bool _cacheLoaded = false;

    void ensureCache();
    void invalidateCache();

public:
    ExamRepository(const std::string &examFile, const std::string &questionsDir);

    // ------------------------------------------------------------
    //  Triển khai IExamRepository
    // ------------------------------------------------------------

    std::vector<Exam> load() override;
    bool save(const std::vector<Exam> &exams) override;
    bool backup() override;

    Exam *findById(ExamId id) override;
    std::vector<Exam> findByTeacher(TeacherId teacherId) override;
    bool examIdExists(ExamId id) override;
};

#endif // EXAM_REPOSITORY_H