//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-03-18 18:48:08.
// File name: function_proxy.cc
//
// Description:
// Define class FunctionProxy.
//

#include "game_server/server/extension/python/function_proxy.h"

#include "entity/inform_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "gateway_server/protocol/gateway_inform_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "global/configure/configure.h"

namespace game {

namespace server {

namespace python {

bool FunctionProxy::SendInformMessage(core::uint64 actor, int inform_type,
    PyObject *params) {
  if (params == NULL || inform_type <= entity::InformType::MIN ||
      inform_type >= entity::InformType::MAX) {
    LOG_ERROR("参数错误");
    return false;
  }

  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(actor);
  if (game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", actor);
    return false;
  }

  gateway::protocol::MessageInformNotify message;
  message.__set_id_(inform_type);

  Py_ssize_t size = PyList_Size(params);
  for (Py_ssize_t pos = 0; pos < size; ++pos) {
    message.params_.push_back(PyString_AsString(PyList_GetItem(params, pos)));
  }

  game_actor->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);

  return true;
}

bool FunctionProxy::BroadcastInformMessage(int inform_type, PyObject *params) {
  if (params == NULL || inform_type <= entity::InformType::MIN ||
      inform_type >= entity::InformType::MAX) {
    LOG_ERROR("参数错误");
    return false;
  }

  gateway::protocol::MessageInformNotify message;
  message.__set_id_(inform_type);

  Py_ssize_t size = PyList_Size(params);
  for (Py_ssize_t pos = 0; pos < size; ++pos) {
    message.params_.push_back(PyString_AsString(PyList_GetItem(params, pos)));
    LOG_INFO("MessageInformNotify params[%zu] params(%s)",
        pos, PyString_AsString(PyList_GetItem(params, pos)));
  }

  GameServerSingleton::GetInstance().BroadcastMessage(message,
      gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);

  return true;
}

const std::string FunctionProxy::GetPlayingConfigureFile() const {
  return global::configure::ConfigureSingleton::GetInstance().GetPlayingConfigureFile();
}

}  // namespace python

}  // namespace server

}  // namespace game

