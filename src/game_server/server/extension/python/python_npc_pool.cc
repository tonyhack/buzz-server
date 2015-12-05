//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-03 09:50:54.
// File name: python_npc_pool.cc
//
// Description:
// Define class PythonNpcPool.
//

#include "game_server/server/extension/python/python_npc_pool.h"

#include "game_server/server/extension/python/python_npc.h"

namespace game {

namespace server {

namespace python {

bool PythonNpcPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->npcs_.Initialize(initial_number, extend_number);
}

void PythonNpcPool::Finalize() {
  this->npcs_.Finalize();
}

PythonNpcPtr PythonNpcPool::Allocate() {
  return this->npcs_.Allocate();
}

void PythonNpcPool::Deallocate(const PythonNpcPtr &npc) {
  this->npcs_.Deallocate(npc);
}

}  // namespace python

}  // namespace server

}  // namespace game

