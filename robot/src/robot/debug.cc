#include "robot/debug.h"

#include <cctype>
#include <cstdio>
#include <algorithm>
#include "robot/logging.h"

namespace robot {
namespace debug {

void hexdump(const char *buffer, size_t size)
{
#define LINE_CHAR_COUNT 16
    const char *buffer_end = buffer + size;
    const char *line_start = buffer;

    for (;;) {
        const char *line_end = std::min(line_start + LINE_CHAR_COUNT,
                                        buffer_end);
        if (line_start == line_end) {
            break;
        }

        char output[1024];
        size_t count = 0;
        size_t blank_count = LINE_CHAR_COUNT;

        // hex part
        for (const char *p = line_start; p < line_end; ++p, --blank_count) {
            count += ::snprintf(output + count, sizeof(output), "%02hhx ", *p);
        }
        for (size_t i = 0; i < blank_count; ++i) {
            count += ::snprintf(output + count, sizeof(output), "   ");
        }

        // blank
        count += ::snprintf(output + count, sizeof(output), "    ");

        // acsii part
        for (const char *p = line_start; p < line_end; ++p) {
            count += ::snprintf(output + count, sizeof(output), "%c",
                                ::isprint(*p) ? *p : '.');
        }

        LOG_DEBUG("%s", output);

        line_start = line_end;
    }

#undef LINE_CHAR_COUNT
}

} // namespace debug
} // namespace robot

