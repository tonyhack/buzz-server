//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-09 14:29:58.
// File name: communicator_test.cc
//
// Description:
// Define class CommunicatorTest.
//

#include "coresh/test/communicator_test.h"

#include <boost/bind.hpp>

namespace coresh {

namespace test {

void Document::Append(const std::string &text) {
  CommunicatorSingleton::GetInstance().Broadcast(DocumentMessageType::APPEND,
      this->id_, 0, text.data(), text.size() + 1);
}

void Document::Clear() {
  CommunicatorSingleton::GetInstance().Broadcast(DocumentMessageType::CLEAR,
      this->id_, 0, NULL, 0);
}

void TextDisplay::Open(core::uint64 id) {
  Communicator::Connection connection1 =
    CommunicatorSingleton::GetInstance().Follow(DocumentMessageType::APPEND,
        id, 0, boost::bind(&TextDisplay::OnAppend, this, _1, _2, _3, _4));
  Communicator::Connection connection2 =
    CommunicatorSingleton::GetInstance().Follow(DocumentMessageType::CLEAR,
        id, 0, boost::bind(&TextDisplay::OnClear, this, _1, _2, _3, _4));

  DocumentConnectionMap::iterator iterator = this->connections_.find(id);
  if(iterator == this->connections_.end()) {
    ConnectionMap temp_connection;
    temp_connection.insert(std::make_pair(DocumentMessageType::APPEND, connection1));
    temp_connection.insert(std::make_pair(DocumentMessageType::CLEAR, connection2));
    this->connections_.insert(std::make_pair(id, temp_connection));
  } else {
    iterator->second.insert(std::make_pair(DocumentMessageType::APPEND, connection1));
    iterator->second.insert(std::make_pair(DocumentMessageType::CLEAR, connection2));
  }
}

void TextDisplay::Close(core::uint64 id) {
  DocumentConnectionMap::iterator iterator = this->connections_.find(id);
  if(iterator != this->connections_.end()) {
    ConnectionMap::iterator iter = iterator->second.begin();
    for(; iter != iterator->second.end(); ++iter) {
      CommunicatorSingleton::GetInstance().Unfollow(iter->second);
    }
  }
}

void TextDisplay::OnAppend(core::uint64 document_id, core::uint8 type,
    const void *message, size_t size) {
  std::cout << "  TextDisplay [" << display_id_ << "] receive document["
    << document_id << "] append:\"" << message << "\"" << std::endl;
}

void TextDisplay::OnClear(core::uint64 document_id, core::uint8 type,
    const void *message, size_t size) {
  std::cout << "  TextDisplay [" << display_id_ << "] receive document["
  << document_id << "] clear" << std::endl;
}

void CommunicatorTest::Initialize() {
  this->document1_.Initialize(1);
  this->document2_.Initialize(2);

  this->editor1_.Initialize(100);
  this->editor2_.Initialize(200);
}

void CommunicatorTest::Start() {

  std::cout << "editor1[doc1, doc2] editor2[doc2]:" << std::endl;

  this->editor1_.Open(1);
  this->editor1_.Open(2);

  this->editor2_.Open(2);

  this->document1_.Append("append append 1");
  this->document2_.Append("append append 2");

  this->document1_.Clear();
  this->document2_.Clear();

  std::cout << std::endl;




  std::cout << "editor1[doc2] editor2[doc2]:" << std::endl;

  this->editor1_.Close(1);

  this->document1_.Append("append append 1111");
  this->document2_.Append("append append 2222");

  this->document1_.Clear();
  this->document2_.Clear();

  std::cout << std::endl;




  std::cout << "editor2[doc2]:" << std::endl;

  this->editor1_.Close(2);

  this->document1_.Append("append append append 111111111");
  this->document2_.Append("append append append 222222222");

  this->document1_.Clear();
  this->document2_.Clear();

  std::cout << std::endl;




  std::cout << "null:" << std::endl;

  this->editor2_.Close(2);

  this->document1_.Append("append append append 11111111111111");
  this->document2_.Append("append append append 22222222222222");

  this->document1_.Clear();
  this->document2_.Clear();

  std::cout << std::endl;




  std::cout << "editor1[all]:" << std::endl;

  this->editor1_.Open(CommunicatorSingleton::GetInstance().GetChannelAll());

  this->document1_.Append("append append append 11111111111111-----");
  this->document2_.Append("append append append 22222222222222-----");

  this->document1_.Clear();
  this->document2_.Clear();

  std::cout << std::endl;




  std::cout << "null:" << std::endl;

  this->editor1_.Close(CommunicatorSingleton::GetInstance().GetChannelAll());

  this->document1_.Append("append append append 11111111111111-----11111");
  this->document2_.Append("append append append 22222222222222-----22222");

  this->document1_.Clear();
  this->document2_.Clear();
  std::cout << std::endl;

}

}  // namespace test

}  // namespace coresh

int main() {
  coresh::test::CommunicatorTest test;
  test.Initialize();
  test.Start();

  return 0;
}

