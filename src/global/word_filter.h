#ifndef __GLOBAL__WORD__FILTER__H
#define __GLOBAL__WORD__FILTER__H

#include <string>

#include "global/ac_automata.h"

namespace global {

class WordFilter {
 public:
  WordFilter();
  ~WordFilter();

  bool Initialize(const std::string &file);
  bool Search(const std::string &text);
  void Block(std::string *text);

 private:
  AcAutomata automata_;
};

}  // namespace global

#endif  // __GLOBAL__WORD__FILTER__H

