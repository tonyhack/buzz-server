#include <brickred/string_util.h>

#include <cstdio>
#include <cstring>
#include <cctype>
#include <algorithm>

namespace brickred {
namespace string_util {

void split(const char *str, const char *sep,
           std::vector<std::string> *result, int max_split)
{
    size_t sep_len = strlen(sep);
    if (0 == sep_len) {
        return;
    }

    int count = 0;
    const char *last = str;
    for (;;) {
        if (max_split > 0 && ++count >= max_split) {
            result->push_back(std::string(last));
            break;
        }

        const char *next = ::strstr(last, sep);
        if (NULL == next) {
            result->push_back(std::string(last));
            break;
        }

        result->push_back(std::string(last, next - last));

        last = next + sep_len;
    }
}

void split(const char *str, size_t str_len, const char *sep,
           std::vector<std::string> *result, int max_split)
{
    size_t sep_len = strlen(sep);
    if (0 == sep_len) {
        return;
    }

    int count = 0;
    const char *end = str + str_len;
    const char *sep_end = sep + sep_len;
    const char *last = str;
    for (;;) {
        if (max_split > 0 && ++count >= max_split) {
            result->push_back(std::string(last, end));
            break;
        }

        const char *next = std::search(last, end, sep, sep_end);
        if (next == end) {
            result->push_back(std::string(last, end));
            break;
        }

        result->push_back(std::string(last, next - last));

        last = next + sep_len;
    }
}

std::string ltrim(const std::string &str, const char *ws)
{
    size_t start = str.find_first_not_of(ws);
    if (std::string::npos == start) {
        return "";
    } else {
        return str.substr(start);
    }
}

std::string rtrim(const std::string &str, const char *ws)
{
    size_t end = str.find_last_not_of(ws);
    if (std::string::npos == end) {
        return "";
    } else {
        return str.substr(0, end + 1);
    }
}

std::string trim(const std::string &str, const char *ws)
{
    return rtrim(ltrim(str));
}

std::string toUpper(const std::string &str)
{
    std::string upper(str);
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);

    return upper;
}

std::string toLower(const std::string &str)
{
    std::string lower(str);
    std::transform(lower.begin(), lower.end(), lower.begin(), ::toupper);

    return lower;
}

std::string toString(int i)
{
    char buffer[32];
    ::snprintf(buffer, sizeof(buffer), "%d", i);
    return std::string(buffer);
}

std::string toString(long l)
{
    char buffer[64];
    ::snprintf(buffer, sizeof(buffer), "%ld", l);
    return std::string(buffer);
}

std::string toString(long long ll)
{
    char buffer[64];
    ::snprintf(buffer, sizeof(buffer), "%lld", ll);
    return std::string(buffer);
}

std::string toString(unsigned ui)
{
    char buffer[32];
    ::snprintf(buffer, sizeof(buffer), "%u", ui);
    return std::string(buffer);
}

std::string toString(unsigned long ul)
{
    char buffer[64];
    ::snprintf(buffer, sizeof(buffer), "%lu", ul);
    return std::string(buffer);
}

std::string toString(unsigned long long ull)
{
    char buffer[64];
    ::snprintf(buffer, sizeof(buffer), "%llu", ull);
    return std::string(buffer);
}

const char *find(const char *str, size_t str_len, const char *keyword)
{
    size_t keyword_len = strlen(keyword);
    if (0 == keyword_len) {
        return NULL;
    }

    const char *ret = std::search(str, str + str_len,
                                  keyword, keyword + keyword_len);
    if (ret == str + str_len) {
        return NULL;
    } else {
        return ret;
    }
}

bool stricmp(const std::string &lhs, const std::string &rhs)
{
    if (lhs.size() != rhs.size()) {
        return false;
    }

    for (size_t i = 0; i < lhs.size(); ++i) {
        if (::tolower(lhs[i]) != ::tolower(rhs[i])) {
            return false;
        }
    }

    return true;
}

size_t Hash::operator()(const std::string &str) const
{
    size_t h = 5381;

    for (size_t i = 0; i < str.size(); ++i) {
        // hash = hash * 33 + str[i]
        h = (h << 5) + h + str[i];
    }

    return h;
}

bool CaseInsensitiveLess::operator()(const std::string &lhs,
                                     const std::string &rhs) const
{
    size_t comp_size = std::min(lhs.size(), rhs.size());

    for (size_t i = 0; i < comp_size; ++i) {
        int c1 = ::tolower(lhs[i]);
        int c2 = ::tolower(rhs[i]);

        if (c1 != c2) {
            return c1 < c2;
        }
    }

    if (lhs.size() < rhs.size()) {
        return true;
    }

    return false;
}

} // namespace string_util
} // namespace brickred
