#ifndef BRICKRED_COMMAND_LINE_OPTION_H
#define BRICKRED_COMMAND_LINE_OPTION_H

#include <map>
#include <string>
#include <vector>

namespace brickred {

class CommandLineOption {
public:
    struct ParameterType {
        enum type {
            NONE = 0,
            REQUIRED
        };
    };

    typedef std::vector<std::string> ArgumentVector;
    typedef std::vector<std::string> ParameterVector;
    typedef std::map<std::string, ParameterType::type> OptionParameterTypeMap;
    typedef std::map<std::string, ParameterVector> OptionParametersMap;

    CommandLineOption();
    ~CommandLineOption();

    void addOption(const std::string &opt,
                   ParameterType::type param_type = ParameterType::NONE);
    bool parse(int argc, char *argv[]);

    bool hasOption(const std::string &opt) const;
    const std::string &getParameter(const std::string &opt) const;
    const ParameterVector &getParameters(const std::string &opt) const;
    const ArgumentVector &getLeftArguments() const { return left_args_; }

private:
    OptionParameterTypeMap option_types_;
    OptionParametersMap option_params_;
    ArgumentVector left_args_;
};

} // namespace brickred

#endif
