#include <unistd.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "robot/config.h"
#include "robot/logging.h"
#include "robot/client_app.h"

class CommandLineOption {
public:
    CommandLineOption() : log_level_(0), log_stderr_(false) {}
    ~CommandLineOption() {}

    bool parse(int argc, char *argv[]);
    void printUsage();

public:
    std::string cmd_dirname_;
    std::string cmd_basename_;
    std::string config_dir_;
    std::string log_file_;
    int log_level_;
    bool log_stderr_;
};

bool CommandLineOption::parse(int argc, char *argv[])
{
    struct stat sb;
    int opt = -1;

    {
        std::vector<char> buffer(argv[0], argv[0] + strlen(argv[0]) + 1); 
        cmd_dirname_ = ::dirname(&buffer[0]);
    }
    {
        std::vector<char> buffer(argv[0], argv[0] + strlen(argv[0]) + 1);
        cmd_basename_ = ::basename(&buffer[0]);
    }

    opterr = 0;

    while ((opt = ::getopt(argc, argv, "c:l:L:S")) != -1) {
        switch (opt) {
        case 'c':
            config_dir_ = optarg;
            break;
        case 'l':
            log_file_ = optarg;
            break;
        case 'L':
            log_level_ = ::atoi(optarg);
            break;
        case 'S':
            log_stderr_ = true;
            break;
        default:
            return false;
        }
    }

    if (log_level_ < robot::Logging::LogLevel::MIN ||
        log_level_ >= robot::Logging::LogLevel::MAX) {
        return false;
    }

    if (config_dir_.empty()) {
        std::vector<std::string> candidates;
        candidates.push_back(cmd_dirname_ +
                             "/../etc/");
        candidates.push_back(cmd_dirname_ +
                             "/etc/");

        size_t i = 0;
        for (; i < candidates.size(); ++i) {
            if (stat(candidates[i].c_str(), &sb) == 0 &&
                S_ISDIR(sb.st_mode)) {
                config_dir_ = candidates[i];
                break;
            }
        }

        if (config_dir_.empty()) {
            return false;
        }
    }

    if (log_file_.empty()) {
        log_file_ = "buzz_robot_%Y_%m_%d.log";
    }

    return true;
}

void CommandLineOption::printUsage()
{
    ::fprintf(stderr, "usage: %s [-c config_dir] [-l log_file] "
                              "[-L level_level] [-S]\n",
            cmd_basename_.c_str());
    ::fprintf(stderr, "-S: enable stderr log\n");
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    CommandLineOption option;

    if (option.parse(argc, argv) == false) {
        option.printUsage();
        return -1;
    }

    if (robot::Logging::initMainLogger(option.log_file_,
                                       option.log_level_,
                                       option.log_stderr_) == false) {
        fprintf(stderr, "init main logger(%s) failed\n",
                option.log_file_.c_str());
        return -1;
    }
    if (robot::Config::getInstance()->load(option.config_dir_) == false) {
        LOG_ERROR("load config in dir(%s) failed",
                  option.config_dir_.c_str());
        return -1;
    }

    if (robot::ClientApp::getInstance()->init() == false) {
        return -1;
    }
    LOG_INFO("buzz robot started");
    robot::ClientApp::getInstance()->run();

    return 0;
}

