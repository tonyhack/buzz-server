//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-27 18:23:38.
// File name: python_engine.cc
//
// Description:
// Define class PythonEngine.
//

#include "game_server/server/extension/python/python_engine.h"

#include <libgen.h>

#include "game_server/server/extension/python/communicator_proxy.h"
#include "game_server/server/extension/python/function_proxy.h"
#include "game_server/server/extension/python/python_actor.h"
#include "game_server/server/extension/python/python_npc.h"
#include "game_server/server/extension/python/python_entity_manager.h"
#include "game_server/server/extension/python/python_variable_constants.h"
#include "game_server/server/extension/python/request_proxy.h"
#include "game_server/server/extension/python/log_proxy.h"
#include "game_server/server/extension/python/timer_proxy.h"

namespace game {

namespace server {

namespace python {

// 类型注册
BOOST_PYTHON_MODULE(cctype) {
  // 注册 CommunicatorProxy 类
  boost::python::class_<CommunicatorProxy>("CommunicatorProxy")
    .def("follow", &CommunicatorProxy::Follow)
    .def("unfollow", &CommunicatorProxy::Unfollow)
    ;
  boost::python::register_ptr_to_python<CommunicatorProxyPtr>();

  // 注册 PythonEntityManager 类
  boost::python::class_<PythonEntityManager>("PythonEntityManager")
    .def("get_actor", &PythonEntityManager::GetActor)
    .def("get_npc", &PythonEntityManager::GetNpc)
    ;
  boost::python::register_ptr_to_python<PythonEntityManagerPtr>();

  // 注册 PythonActor 类
  boost::python::class_<PythonActor>("PythonActor")
    .def("get_attr_entity_client", &PythonActor::GetAttrEntityClient)
    .def("get_attr_entity_server", &PythonActor::GetAttrEntityServer)
    .def("get_attr_role_aoi", &PythonActor::GetAttrRoleAoi)
    .def("get_attr_role_client", &PythonActor::GetAttrRoleClient)
    .def("get_attr_role_server", &PythonActor::GetAttrRoleServer)
    .def("get_attr_actor_aoi", &PythonActor::GetAttrActorAoi)
    .def("get_attr_actor_client", &PythonActor::GetAttrActorClient)
    .def("get_attr_actor_server", &PythonActor::GetAttrActorServer)
    .def("set_attr_entity_client", &PythonActor::SetAttrEntityClient)
    .def("set_attr_entity_server", &PythonActor::SetAttrEntityServer)
    .def("set_attr_role_aoi", &PythonActor::SetAttrRoleAoi)
    .def("set_attr_role_client", &PythonActor::SetAttrRoleClient)
    .def("set_attr_role_server", &PythonActor::SetAttrRoleServer)
    .def("set_attr_actor_aoi", &PythonActor::SetAttrActorAoi)
    .def("set_attr_actor_client", &PythonActor::SetAttrActorClient)
    .def("set_attr_actor_server", &PythonActor::SetAttrActorServer)
    .def("get_basic_status", &PythonActor::GetBasicStatus)
    .def("add_resource", &PythonActor::AddResource)
    .def("get_pos_x", &PythonActor::GetPosx)
    .def("get_pos_y", &PythonActor::GetPosy)
    .def("get_name", &PythonActor::GetName)
    ;
  boost::python::register_ptr_to_python<PythonActorPtr>();

  // 注册 PythonNpc 类
  boost::python::class_<PythonNpc>("PythonNpc")
    .def("get_attr_entity_client", &PythonNpc::GetAttrEntityClient)
    .def("get_attr_entity_server", &PythonNpc::GetAttrEntityServer)
    .def("get_attr_role_aoi", &PythonNpc::GetAttrRoleAoi)
    .def("get_attr_role_client", &PythonNpc::GetAttrRoleClient)
    .def("get_attr_role_server", &PythonNpc::GetAttrRoleServer)
    .def("get_attr_npc_aoi", &PythonNpc::GetAttrNpcAoi)
    .def("get_attr_npc_client", &PythonNpc::GetAttrNpcClient)
    .def("set_attr_entity_client", &PythonNpc::SetAttrEntityClient)
    .def("set_attr_entity_server", &PythonNpc::SetAttrEntityServer)
    .def("set_attr_role_aoi", &PythonNpc::SetAttrRoleAoi)
    .def("set_attr_role_client", &PythonNpc::SetAttrRoleClient)
    .def("set_attr_role_server", &PythonNpc::SetAttrRoleServer)
    .def("set_attr_npc_aoi", &PythonNpc::SetAttrNpcAoi)
    .def("set_attr_npc_client", &PythonNpc::SetAttrNpcClient)
    .def("get_pos_x", &PythonNpc::GetPosx)
    .def("get_pos_y", &PythonNpc::GetPosy)
    ;
  boost::python::register_ptr_to_python<PythonNpcPtr>();

  // 注册 RequestProxy 类
  boost::python::class_<RequestProxy>("RequestProxy")
    .def("request", &RequestProxy::Request)
    .def("get_result", &RequestProxy::GetResult)
    ;
  boost::python::register_ptr_to_python<RequestProxyPtr>();

  // 注册 LogProxy 类
  boost::python::class_<LogProxy>("LogProxy")
    .def("log_info", &LogProxy::LogInfo)
    .def("log_debug", &LogProxy::LogDebug)
    .def("log_error", &LogProxy::LogError)
    .def("log_warning", &LogProxy::LogWarning)
    ;
  boost::python::register_ptr_to_python<LogProxyPtr>();

  // 注册 TimerProxy 类
  boost::python::class_<TimerProxy>("TimerProxy")
    .def("add_timer", &TimerProxy::AddTimer)
    .def("remove_timer", &TimerProxy::RemoveTimer)
    ;
  boost::python::register_ptr_to_python<TimerProxyPtr>();

  // 注册 FunctionProxy 类
  boost::python::class_<FunctionProxy>("FunctionProxy")
    .def("send_inform_message", &FunctionProxy::SendInformMessage)
    .def("broadcast_inform_message", &FunctionProxy::BroadcastInformMessage)
    .def("get_playing_configure_file", &FunctionProxy::GetPlayingConfigureFile)
    ;
  boost::python::register_ptr_to_python<FunctionProxyPtr>();
}

PythonEngine::PythonEngine() {}
PythonEngine::~PythonEngine() {}

bool PythonEngine::Initialize(const std::string &module) {
  this->module_ = NULL;

  char module_name[255];
  snprintf(module_name, sizeof(module_name), "%s", module.c_str());

  // 得到 Python 主模块目录，构造导入命令
  char sys_path_addpend[512];
  snprintf(sys_path_addpend, sizeof(sys_path_addpend),
      "sys.path.append('%s')", ::dirname(module_name));

  global::LogDebug("import dir (%s)", sys_path_addpend);

  snprintf(module_name, sizeof(module_name), "%s", module.c_str());
  // 从文件目录得到 Python 主模块名称
  std::string module_str(::basename(module_name));
  size_t pos = module_str.rfind(".py");
  if(pos == std::string::npos) {
    global::LogError("%s:%d (%s) Python 主模块文件(%s) 错误",
        __FILE__, __LINE__, __FUNCTION__, module.c_str());
    return false;
  }
  module_str = module_str.substr(0, pos);

  // 分配实体管理器
  this->entity_manager_.reset(new (std::nothrow) PythonEntityManager());
  if(this->entity_manager_.get() == NULL) {
    global::LogError("%s:%d (%s) 分配 PythonEntityManager 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 分配 CommunicatorProxy
  this->communicator_proxy_.reset(new (std::nothrow) CommunicatorProxy());
  if(this->communicator_proxy_.get() == NULL) {
    global::LogError("%s:%d (%s) 分配 CommunicatorProxy 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  // 初始化 CommunicatorProxy
  if(this->communicator_proxy_->Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 CommunicatorProxy 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 分配 FunctionProxy
  this->function_proxy_.reset(new (std::nothrow) FunctionProxy());
  if (this->function_proxy_.get() == NULL) {
    LOG_ERROR("分配 FunctionProxy 失败");
    return false;
  }

  // 分配 RequestProxy
  this->request_proxy_.reset(new (std::nothrow) RequestProxy());
  if(this->request_proxy_.get() == NULL) {
    global::LogError("%s:%d (%s) 分配 RequestProxy 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  // 初始化 RequestProxy
  if(this->request_proxy_->Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 RequestProxy 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 分配 LogProxy
  this->log_proxy_.reset(new (std::nothrow) LogProxy());
  if(this->log_proxy_.get() == NULL) {
    LOG_ERROR("分配 LogProxy 失败");
    return false;
  }

  // 分配 TimerProxy
  this->timer_proxy_.reset(new (std::nothrow) TimerProxy());
  if(this->timer_proxy_.get() == NULL) {
    LOG_ERROR("分配 TimerProxy 失败");
    return false;
  }
  // 初始化 TimerProxy
  if(this->timer_proxy_->Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 TimerProxy 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  Py_Initialize();

  try {

    // 注册 cctype内容 到 Python虚拟机
    initcctype();

    // 导入 Python sys模块
    PyRun_SimpleString("import sys");
    // 导入 脚本目录
    PyRun_SimpleString(sys_path_addpend);

    // 载入 Python 主模块
    this->module_ = new (std::nothrow) boost::python::handle<>(
        PyImport_ImportModule(module_str.c_str()));
    if(this->module_ == NULL) {
      global::LogError("%s:%d (%s) 载入 Python 主模块(%s) 失败",
          __FILE__, __LINE__, __FUNCTION__, module.c_str());
      return false;
    }

    // 把实体管理器写入 __main__ 模块的变量表
    boost::python::object main = boost::python::import("__main__");
    main.attr(variable::g_python_variable_constants.kVariableStr[
        variable::Variable::ENTITY_MANAGER].c_str()) = this->entity_manager_;
    main.attr(variable::g_python_variable_constants.kVariableStr[
        variable::Variable::CURRENT_ACTOR].c_str()) = PythonActorPtr();
    main.attr(variable::g_python_variable_constants.kVariableStr[
        variable::Variable::CURRENT_NPC].c_str()) = PythonNpcPtr();
    main.attr(variable::g_python_variable_constants.kVariableStr[
        variable::Variable::COMMUNICATOR_PROXY].c_str()) = this->communicator_proxy_;
    main.attr(variable::g_python_variable_constants.kVariableStr[
        variable::Variable::REQUEST_PROXY].c_str()) = this->request_proxy_;
    main.attr(variable::g_python_variable_constants.kVariableStr[
        variable::Variable::LOG_PROXY].c_str()) = this->log_proxy_;
    main.attr(variable::g_python_variable_constants.kVariableStr[
        variable::Variable::TIMER_PROXY].c_str()) = this->timer_proxy_;
    main.attr(variable::g_python_variable_constants.kVariableStr[
        variable::Variable::FUNCTION_PROXY].c_str()) = this->function_proxy_;

    if (boost::python::call_method<bool>(this->module_->get(), "initialize") == false) {
      LOG_ERROR("call_method initialize failed.");
      return false;
    }

    return true;

  } catch (boost::python::error_already_set) {

    // TODO: 替换为异常信息写入日志
    PyErr_Print();
    global::LogError("%s:%d (%s) 初始化 PythonEngine module(%s) 错误",
        __FILE__, __LINE__, __FUNCTION__, module.c_str());

    // 释放 Python 主模块对象
    delete this->module_;

    return false;

  }
}

void PythonEngine::Finalize() {
  try {

    boost::python::call_method<void>(this->module_->get(), "finalize");

    this->timer_proxy_->Finalize();
    this->request_proxy_->Finalize();
    this->communicator_proxy_->Finalize();

    delete this->module_;
    Py_Finalize();

  } catch (boost::python::error_already_set) {

    // TODO: 替换为异常信息写入日志
    PyErr_Print();
    global::LogError("%s:%d (%s) 结束 PythonEngine 错误",
        __FILE__, __LINE__, __FUNCTION__);
  }
}

}  // namespace python

}  // namespace server

}  // namespace game

