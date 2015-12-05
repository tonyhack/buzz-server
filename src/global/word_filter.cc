#include "global/word_filter.h"

#include <algorithm>
#include <fstream>
#include <vector>

#include "global/logging.h"

namespace global {

WordFilter::WordFilter() {}
WordFilter::~WordFilter() {}

bool WordFilter::Initialize(const std::string &file) {
  std::ifstream fin(file.c_str());
  if (!fin) {
    return false;
  }

  std::string keyword;
  while (std::getline(fin, keyword)) {
    if (this->automata_.AddPattern(keyword) == false) {
      LOG_DEBUG("Keyword %s duplicate.", keyword.c_str());
    }
  }

  if (this->automata_.Compile() == false) {
    return false;
  }

  return true;
}

bool WordFilter::Search(const std::string &text) {
  this->automata_.SetText(text);
  return this->automata_.FindNext(NULL);
}

void WordFilter::Block(std::string *text) {
  if (NULL == text) {
    return;
  }

  std::vector<char> mask;
  std::vector<char> buffer;
  mask.resize(text->size(), 0);
  buffer.reserve(text->size());

  this->automata_.SetText(*text);
  global::AcAutomata::MatchResult result;
  while (this->automata_.FindNext(&result)) {
    for (size_t i = result.start_position_;
         i <= result.end_position_; ++i) {
      mask[i] = '*';
    }
  }

  int star_count = 0;
  for (size_t i = 0; i < text->size(); ++i) {
    if (mask[i] != '*') {
      if (star_count != 0) {
        for (int j = 0; j < std::min(3, star_count); ++j) {
          buffer.push_back('*');
        }
        star_count = 0;
      }
      buffer.push_back((*text)[i]);
    } else {
      ++star_count;
    }
  }

  if (star_count != 0) {
    for (int j = 0; j < std::min(3, star_count); ++j) {
      buffer.push_back('*');
    }
  }

  text->assign(buffer.begin(), buffer.end());
}

}  // namespace global

