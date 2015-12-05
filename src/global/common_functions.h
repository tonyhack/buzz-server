//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-10 15:53:29.
// File name: common_functions.h
//
// Description:
// Define common functions.
//

#ifndef __GLOBAL__COMMON__FUNCTIONS__H
#define __GLOBAL__COMMON__FUNCTIONS__H

#include <cstddef>
#include <string>
#include <vector>

#include "core/base/types.h"

namespace global {

// 字符串分割
int StringSplit(const char *str, const char *split, std::vector<core::int32> &result);
int StringSplit(const char *str, const char *split, std::vector<core::int64> &result);
int StringSplit(const char *str, const char *split, std::vector<std::string> &result);

// 格式为: 2013/03/26 19:14:20
time_t FormatTime(const char *str);

// 空字符串检测
bool CheckEmptyStr(const char *str);

// 取UTF8字符串可视长度(中文算2个字符, 英文算1个字符)
int GetStrVisualLengthUTF8(const char *str);

std::string ToString(int i);
std::string ToString(long l);
std::string ToString(long long ll);
std::string ToString(unsigned ui);
std::string ToString(unsigned long ul);
std::string ToString(unsigned long long ull);

void Hexdump(const char *buffer, size_t size);

// 计算俩段时间经过的自然天数
core::int32 GetNatureDays(time_t last_time, time_t now_time);
  
}  // namespace global

#endif  // __GLOBAL__COMMON__FUNCTIONS__H
