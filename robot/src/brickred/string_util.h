#ifndef BRICKRED_STRING_UTIL_H
#define BRICKRED_STRING_UTIL_H

#include <cstddef>
#include <string>
#include <vector>

namespace brickred {
namespace string_util {

void split(const char *str, const char *sep,
           std::vector<std::string> *result, int max_split = -1);
void split(const char *str, size_t str_len, const char *sep,
           std::vector<std::string> *result, int max_split = -1);

std::string ltrim(const std::string &str, const char *ws = " \t");
std::string rtrim(const std::string &str, const char *ws = " \t");
std::string trim(const std::string &str, const char *ws = " \t");
std::string toUpper(const std::string &str);
std::string toLower(const std::string &str);

std::string toString(int i);
std::string toString(long l);
std::string toString(long long ll);
std::string toString(unsigned ui);
std::string toString(unsigned long ul);
std::string toString(unsigned long long ull);

const char *find(const char *str, size_t str_len, const char *keyword);
bool stricmp(const std::string &lhs, const std::string &rhs);

struct Hash {
    size_t operator()(const std::string &str) const;
};

struct CaseInsensitiveLess {
    bool operator()(const std::string &lhs, const std::string &rhs) const;
};


} // namespace string_util
} // namespace brickred

#endif
