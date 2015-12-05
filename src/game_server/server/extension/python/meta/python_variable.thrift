namespace cpp game.server.variable
namespace py ccvariable

enum Variable {
  MIN = 0,

  ENTITY_MANAGER = 0,             // 实体管理器
  CURRENT_ACTOR,                  // 当前玩家
  CURRENT_NPC,                    // 当前NPC
  COMMUNICATOR_PROXY,             // communicator代理
  REQUEST_PROXY,                  // request代理
  LOG_PROXY,                      // log代理
  TIMER_PROXY,                    // timer代理
  FUNCTION_PROXY,                 // function代理

  MAX,
}

const list<string> kVariableStr = [
  "CC_ENTITY_MANAGER",
  "CC_CURRENT_ACTOR",
  "CC_CURRENT_NPC",
  "CC_COMMUNICATOR_PROXY",
  "CC_REQUEST_PROXY",
  "CC_LOG_PROXY",
  "CC_TIMER_PROXY",
  "CC_FUNCTION_PROXY"
]

