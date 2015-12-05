#include <brickred/command_line_option.h>

#include <brickred/string_util.h>

namespace brickred {

static const std::string s_cstr_empty_string;
static const CommandLineOption::ParameterVector s_empty_params;

CommandLineOption::CommandLineOption()
{
}

CommandLineOption::~CommandLineOption()
{
}

void CommandLineOption::addOption(const std::string &opt,
    ParameterType::type param_type)
{
    option_types_.insert(std::make_pair(opt, param_type));

    ParameterVector params;
    option_params_.insert(std::make_pair(opt, params));
}

static bool isOption(const std::string &arg)
{
    return arg.size() > 1 && arg[0] == '-';
}

static bool isLongOption(const std::string &arg)
{
    return arg.size() > 2 && arg[0] == '-' && arg[1] == '-';
}

static int processLongOption(
    const CommandLineOption::OptionParameterTypeMap &option_types,
    CommandLineOption::OptionParametersMap &option_params,
    const std::string &arg, const std::string &next_param)
{
    std::vector<std::string> long_opt_parts;
    string_util::split(arg.c_str() + 2, "=", &long_opt_parts, 1);
    const std::string &opt = long_opt_parts[0];

    // check exists
    CommandLineOption::OptionParameterTypeMap::const_iterator iter =
        option_types.find(opt);
    if (iter == option_types.end()) {
        return -1;
    }
    CommandLineOption::ParameterType::type opt_type = iter->second;

    if (CommandLineOption::ParameterType::NONE == opt_type) {
        // none parameter
        if (long_opt_parts.size() > 1) {
            return -1;
        }
        option_params[opt].push_back(s_cstr_empty_string);
        return 1;

    } else if (CommandLineOption::ParameterType::REQUIRED == opt_type) {
        // required parameter
        if (long_opt_parts.size() > 1) {
            option_params[opt].push_back(long_opt_parts[1]);
            return 1;

        } else {
            if (next_param.empty()) {
                return -1;
            }
            option_params[opt].push_back(next_param);
            return 2;
        }

    } else {
        // error type
        return -1;
    }
}

static int processShortOption(
    const CommandLineOption::OptionParameterTypeMap &option_types,
    CommandLineOption::OptionParametersMap &option_params,
    const std::string &arg, const std::string &next_param)
{
    for (size_t i = 1; i < arg.size(); ++i) {
        const std::string opt(1, arg[i]);

        // check exists
        CommandLineOption::OptionParameterTypeMap::const_iterator iter =
            option_types.find(opt);
        if (iter == option_types.end()) {
            return -1;
        }
        CommandLineOption::ParameterType::type opt_type = iter->second;

        if (CommandLineOption::ParameterType::NONE == opt_type) {
            option_params[opt].push_back(s_cstr_empty_string);

        } else if (CommandLineOption::ParameterType::REQUIRED == opt_type) {
            // left part in option is parameter
            if (i + 1 < arg.size()) {
                option_params[opt].push_back(arg.substr(i + 1));
                return 1;
            }

            // use next paramter
            if (next_param.empty()) {
                return -1;
            }
            option_params[opt].push_back(next_param);
            return 2;
        }
    }

    return 1;
}

bool CommandLineOption::parse(int argc, char *argv[])
{
    for (int i = 1; i < argc;) {
        std::string arg(argv[i]);
        std::string next_param;

        if (i + 1 < argc && isOption(argv[i + 1]) == false) {
            next_param = argv[i + 1];
        }

        // left string in argv[] are all left arguments
        if ("--" == arg) {
            for (i += 1; i < argc; ++i) {
                left_args_.push_back(argv[i]);
            }
            return true;
        }

        // left argument
        if (isOption(arg) == false) {
            left_args_.push_back(arg);
            i += 1;
            continue;
        }

        if (isLongOption(arg)) {
            // long option
            int ret = processLongOption(option_types_, option_params_,
                                        arg, next_param);
            if (-1 == ret) {
                return false;
            }

            i += ret;
            continue;

        } else {
            // short option
            int ret = processShortOption(option_types_, option_params_,
                                         arg, next_param);
            if (-1 == ret) {
                return false;
            }

            i += ret;
            continue;
        }
    }

    return true;
}

bool CommandLineOption::hasOption(const std::string &opt) const
{
    OptionParametersMap::const_iterator iter = option_params_.find(opt);
    if (iter == option_params_.end()) {
        return false;
    }

    const ParameterVector &params = iter->second;
    if (params.empty()) {
        return false;
    }

    return true;
}

const std::string &CommandLineOption::getParameter(
    const std::string &opt) const
{
    OptionParametersMap::const_iterator iter = option_params_.find(opt);
    if (iter == option_params_.end()) {
        return s_cstr_empty_string;
    }

    const ParameterVector &params = iter->second;
    if (params.empty()) {
        return s_cstr_empty_string;
    }

    return params.back();
}

const CommandLineOption::ParameterVector &CommandLineOption::getParameters(
    const std::string &opt) const
{
    OptionParametersMap::const_iterator iter = option_params_.find(opt);
    if (iter == option_params_.end()) {
        return s_empty_params;
    }

    return iter->second;
}

} // namespace brickred
