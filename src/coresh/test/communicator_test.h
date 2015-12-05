//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-09 12:27:22.
// File name: communicator_test.h
//
// Description:
// Define class CommunicatorTest.
//

#ifndef __CORESH__TEST__COMMUNICATOR__TEST__H
#define __CORESH__TEST__COMMUNICATOR__TEST__H

#include <iostream>
#include <map>
#include <string>

#include "core/base/noncopyable.h"
#include "coresh/communicator.h"

namespace coresh {

namespace test {

struct DocumentMessageType {
  enum Type {
    APPEND = 0,
    CLEAR,
  };
};

class Document : public core::Noncopyable {
 public:
  Document() : id_(0) {}
  ~Document() {}

  inline void Initialize(core::uint64 id) {
    this->id_ = id;
  }

  void Append(const std::string &text);
  void Clear();

 private:
  core::uint64 id_;
};

class TextDisplay : public core::Noncopyable {
  typedef std::map<DocumentMessageType::Type, Communicator::Connection> ConnectionMap;
  typedef std::map<core::uint64, ConnectionMap> DocumentConnectionMap;

 public:
  TextDisplay() : display_id_(0) {}
  ~TextDisplay() {}

  void Initialize(core::uint32 display_id) {
    this->display_id_ = display_id;
  }

  void Open(core::uint64 id);
  void Close(core::uint64 id);

 private:
  void OnAppend(core::uint64 document_id, core::uint8 type,
      const void *message, size_t size);
  void OnClear(core::uint64 document_id, core::uint8 type,
      const void *message, size_t size);

  DocumentConnectionMap connections_;

  core::uint32 display_id_;
};

class CommunicatorTest : public core::Noncopyable {
 public:
  CommunicatorTest() {}
  ~CommunicatorTest() {}

  void Initialize();
  void Start();

  void Append(core::uint64 id, const std::string &text) {
    if(id == 1) {
      this->document1_.Append(text);
    } else if(id == 2) {
      this->document2_.Append(text);
    } else {
      std::cout << "Error id!" << std::endl;
    }
  }
  void Clear(core::uint64 id) {
    if(id == 1) {
      this->document1_.Clear();
    } else if(id == 2) {
      this->document2_.Clear();
    } else {
      std::cout << "Error id!" << std::endl;
    }
  }

 private:
  Document document1_;
  Document document2_;

  TextDisplay editor1_;
  TextDisplay editor2_;
};

}  // namespace test

}  // namespace coresh

#endif  // __CORESH__TEST__COMMUNICATOR__TEST__H

