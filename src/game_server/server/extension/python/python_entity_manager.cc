//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-28 13:52:46.
// File name: python_entity_manager.cc
//
// Description:
// Define class PythonEntityManager.
//

#include "game_server/server/extension/python/python_entity_manager.h"

#include "game_server/server/extension/python/python_actor.h"
#include "game_server/server/extension/python/python_actor_manager.h"
#include "game_server/server/extension/python/python_npc.h"
#include "game_server/server/extension/python/python_npc_manager.h"
#include "game_server/server/extension/python/python_variable_constants.h"

namespace game {

namespace server {

namespace python {

PythonEntityManager::PythonEntityManager() {}

PythonEntityManager::~PythonEntityManager() {}

bool PythonEntityManager::GetActor(core::uint64 id) {
  PythonActorPtr actor = PythonActorManager::GetInstance()->Get(id);
  if(actor.get() == NULL) {
    global::LogError("%s:%d (%s) PythonActorManager 中获取 PythonActor(%lu) 失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return false;
  }

  try {

    // 导入 __main__ 模块
    boost::python::object main = boost::python::import("__main__");
    // 玩家对象传入模块变量表
    main.attr(variable::g_python_variable_constants.kVariableStr[
        variable::Variable::CURRENT_ACTOR].c_str()) = actor;

  } catch(boost::python::error_already_set) {

    // TODO: 替换为异常信息写入日志
    PyErr_Print();
    global::LogError("%s:%d (%s) PythonActorPtr 写入 Python __main__ 模块失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;

  }

  return true;
}

bool PythonEntityManager::GetNpc(core::uint64 id) {
  PythonNpcPtr npc = PythonNpcManager::GetInstance()->Get(id);
  if(npc.get() == NULL) {
    global::LogError("%s:%d (%s) PythonNpcManager 中获取 PythonNpc(%lu) 失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return false;
  }

  try {

    // 导入 __main__ 模块
    boost::python::object main = boost::python::import("__main__");
    // 玩家对象传入模块变量表
    main.attr(variable::g_python_variable_constants.kVariableStr[
        variable::Variable::CURRENT_NPC].c_str()) = npc;

  } catch(boost::python::error_already_set) {

    // TODO: 替换为异常信息写入日志
    PyErr_Print();
    global::LogError("%s:%d (%s) PythonNpcPtr 写入 Python __main__ 模块失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;

  }

  return true;
}

}  // namespace python

}  // namespace server

}  // namespace game

