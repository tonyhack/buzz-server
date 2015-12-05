#include <brickred/log_file_sink.h>

#include <cerrno>
#include <ctime>
#include <cstdio>
#include <cstring>
#include <vector>

#include <brickred/base_logger.h>
#include <brickred/timestamp.h>

namespace brickred {

class LogFileSink::Impl {
public:
    explicit Impl(const std::string &file_path);
    ~Impl();

    void log(const char *buffer, size_t size);
    bool openFile();

private:
    std::string file_path_;
    std::string actual_file_path_;
    FILE *fp_;
};

///////////////////////////////////////////////////////////////////////////////
LogFileSink::Impl::Impl(const std::string &file_path) :
    file_path_(file_path), fp_(NULL)
{
}

LogFileSink::Impl::~Impl()
{
    if (fp_ != NULL) {
        ::fclose(fp_);
    }
}

bool LogFileSink::Impl::openFile()
{
    char actual_file_path[2048];

    Timestamp now;
    now.setNow();
    Timestamp::format(actual_file_path, sizeof(actual_file_path),
                      file_path_.c_str(), now.getSecond());

    if (actual_file_path_ == actual_file_path) {
        return true;
    }

    actual_file_path_ = actual_file_path;

    FILE *fp = ::fopen(actual_file_path_.c_str(), "a");
    if (NULL == fp) {
        return false;
    }
    if (fp_ != NULL) {
        ::fclose(fp_);
    }
    ::setbuf(fp, NULL);
    fp_ = fp;

    return true;
}

void LogFileSink::Impl::log(const char *buffer, size_t size)
{
    if (openFile() == false) {
        BASE_ERROR("open file %s failed: %s",
                   actual_file_path_.c_str(), ::strerror(errno));
        return;
    }

    ::fwrite(buffer, size, 1, fp_);
}

///////////////////////////////////////////////////////////////////////////////
LogFileSink::LogFileSink(const std::string &file_path) :
    pimpl_(new Impl(file_path))
{
}

LogFileSink::~LogFileSink()
{
}

void LogFileSink::log(const char *buffer, size_t size)
{
    pimpl_->log(buffer, size);
}

bool LogFileSink::openFile()
{
    return pimpl_->openFile();
}

} // namespace brickred
