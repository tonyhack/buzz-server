//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-10 16:04:38.
// File name: common_functions.cc
//
// Description:
// Define common functions.
//

#include "global/common_functions.h"

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>

#include "global/logging.h"

namespace global {

int StringSplit(const char *str, const char *split, std::vector<core::int32> &result) {
  if(str == NULL || split == NULL) {
    return 0;
  }

  int num = 0;
  char *token = 0;
  result.clear();

  int len = strlen(str);
  if(len == 0) {
    return 0;
  }

  char *temp_str = new char[len + 1];
  strncpy(temp_str, str, len + 1);

  token = strtok(temp_str, split);
  while(token != NULL) {
    result.push_back(atoi(token));
    ++num;
    token = strtok(NULL, split);
  }

  if(temp_str != NULL) {
    delete [] temp_str;
  }

  return num;
}

int StringSplit(const char *str, const char *split, std::vector<core::int64> &result) {
  if(str == NULL || split == NULL) {
    return 0;
  }

  int num = 0;
  char *token = 0;
  result.clear();

  int len = strlen(str);
  if(len == 0) {
    return 0;
  }

  char *temp_str = new char[len + 1];
  strncpy(temp_str, str, len + 1);

  token = strtok(temp_str, split);
  while(token != NULL) {
    result.push_back(atol(token));
    ++num;
    token = strtok(NULL, split);
  }

  if(temp_str != NULL) {
    delete [] temp_str;
  }

  return num;
}

int StringSplit(const char *str, const char *split, std::vector<std::string> &result) {
  if(str == NULL || split == NULL) {
    return 0;
  }

  int num = 0;
  char *token = 0;
  result.clear();

  int len = strlen(str);
  if(len == 0) {
    return 0;
  }

  char *temp_str = new char[len + 1];
  strncpy(temp_str, str, len + 1);

  token = strtok(temp_str, split);
  while(token != NULL) {
    result.push_back(token);
    ++num;
    token = strtok(NULL, split);
  }

  if(temp_str != NULL) {
    delete [] temp_str;
  }

  return num;
}

// 格式为: 2013/03/26 19:14:20
time_t FormatTime(const char *str) {
  struct tm t;
  if(sscanf(str, "%d/%d/%d %d:%d:%d", &t.tm_year, &t.tm_mon, &t.tm_mday,
        &t.tm_hour, &t.tm_min, &t.tm_sec) == -1) {
    return -1;
  }
  t.tm_year -= 1900;
  t.tm_mon -= 1;
  return mktime(&t);
}

bool CheckEmptyStr(const char *str) {
  return strncmp(str, "", 2) == 0;
}

static bool CheckUTF8FollowingBytes(const char *first, size_t count) {
  for (size_t i = 1; i <= count; ++i) {
    unsigned char c = *(first + i);
    if (c >> 6 != 2) {
      return false;
    }
  }
  return true; 
}

int GetStrVisualLengthUTF8(const char *str) {
  int visual_length = 0;

  for (const char *p = str; *p != 0;) {
    unsigned char c = *p;

    if (c >> 7 == 0x0) {
      // single byte
      ++visual_length;
      ++p;
    } else if (c >> 5 == 0x06) {
      // double bytes
      if (CheckUTF8FollowingBytes(p, 1) == false) {
        return -1;
      }
      visual_length += 2;
      p += 2;
    } else if (c >> 4 == 0x0e) {
      // three bytes
      if (CheckUTF8FollowingBytes(p, 2) == false) {
        return -1;
      }
      visual_length += 2;
      p += 3;
    } else if (c >> 3 == 0x1e) {
      // four bytes
      if (CheckUTF8FollowingBytes(p, 3) == false) {
        return -1;
      }
      visual_length += 2;
      p += 4;
    } else {
      return -1;
    }
  }

  return visual_length;
}

std::string ToString(int i)
{
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%d", i);
  return std::string(buffer);
}

std::string ToString(long l)
{
  char buffer[64];
  snprintf(buffer, sizeof(buffer), "%ld", l);
  return std::string(buffer);
}

std::string ToString(long long ll)
{
  char buffer[64];
  snprintf(buffer, sizeof(buffer), "%lld", ll);
  return std::string(buffer);
}

std::string ToString(unsigned ui)
{
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%u", ui);
  return std::string(buffer);
}

std::string ToString(unsigned long ul)
{
  char buffer[64];
  snprintf(buffer, sizeof(buffer), "%lu", ul);
  return std::string(buffer);
}

std::string ToString(unsigned long long ull)
{
  char buffer[64];
  snprintf(buffer, sizeof(buffer), "%llu", ull);
  return std::string(buffer);
}

void Hexdump(const char *buffer, size_t size) {
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
        count += snprintf(output + count, sizeof(output), "%02hhx ", *p);
      }
      for (size_t i = 0; i < blank_count; ++i) {
        count += snprintf(output + count, sizeof(output), "   ");
      }

      // blank
      count += snprintf(output + count, sizeof(output), "    ");

      // acsii part
      for (const char *p = line_start; p < line_end; ++p) {
        count += snprintf(output + count, sizeof(output), "%c",
          isprint(*p) ? *p : '.');
      }

      LOG_DEBUG("%s", output);

      line_start = line_end;
    }

#undef LINE_CHAR_COUNT
}

core::int32 GetNatureDays(time_t last_time, time_t now_time) {
  if(last_time > now_time) {
    return 0;
  }

  struct tm tm = {0};
  localtime_r(&last_time, &tm);
  struct tm now_tm = {0};
  localtime_r(&now_time, &now_tm);
  core::int32 dailys = 0;
  if(now_tm.tm_year < tm.tm_year) {
    dailys = 0; 
  } else if(now_tm.tm_year == tm.tm_year) {
    dailys = now_tm.tm_yday - tm.tm_yday;
  } else {
    core::int32 i = now_tm.tm_year - tm.tm_year;
    while(i != 0) {
      if((now_tm.tm_year+1900-1)%4 == 0 && (now_tm.tm_year+1900-1)%100 != 0) {
        dailys += 366;
      } else {
        dailys += 365;
      }
      --i; 
    }
    dailys += now_tm.tm_yday;
    dailys -= tm.tm_yday;
  }

  return dailys;
}

}  // namespace global

