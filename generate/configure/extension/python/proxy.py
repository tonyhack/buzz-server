# coding=utf-8

import ccevent
import ccevent.ttypes

import ccrequest
import ccrequest.ttypes

import ccvariable
import ccvariable.ttypes

import variable_table

import ccentity
import ccentity.inform
import ccentity.inform.ttypes

from thrift import TSerialization


# 事件
class Communicator:
  COMMUNICATOR_PROXY = None

  @staticmethod
  def initialize():
    Communicator.COMMUNICATOR_PROXY = variable_table.get_variable(ccvariable.ttypes.Variable.COMMUNICATOR_PROXY)

  @staticmethod
  def follow(event_type, channel, channel_type, callback):
    Communicator.COMMUNICATOR_PROXY.follow(event_type, channel, channel_type, callback)

  @staticmethod
  def unfollow(event_type, channel, channel_type, callback):
    Communicator.COMMUNICATOR_PROXY.unfollow(event_type, channel, channel_type, callback)


# 请求
class Request:
  REQUEST_PROXY = None

  @staticmethod
  def initialize():
    Request.REQUEST_PROXY = variable_table.get_variable(ccvariable.ttypes.Variable.REQUEST_PROXY)

  @staticmethod
  def request(request_type, request):
    # 序列化请求
    data = TSerialization.serialize(request)
    # 发送请求
    if Request.REQUEST_PROXY.request(request_type, data, len(data)) != 0:
      return False
    return True

  @staticmethod
  def get_result():
    return Request.REQUEST_PROXY.get_result()


# 日志
class Logging:
  LOG_PROXY = None

  @staticmethod
  def initialize():
    Logging.LOG_PROXY = variable_table.get_variable(ccvariable.ttypes.Variable.LOG_PROXY)

  @staticmethod
  def debug(log):
    Logging.LOG_PROXY.log_debug("[Python] %s" % log)

  @staticmethod
  def info(log):
    Logging.LOG_PROXY.log_info("[Python] %s" % log)

  @staticmethod
  def error(log):
    Logging.LOG_PROXY.log_error("[Python] %s" % log)


# 定时器
class Timer:
  TIMER_PROXY = None

  @staticmethod
  def initialize():
    Timer.TIMER_PROXY = variable_table.get_variable(ccvariable.ttypes.Variable.TIMER_PROXY)

  @staticmethod
  def add(id, msecs, count, callback):
    return Timer.TIMER_PROXY.add_timer(id, msecs, count, callback)

  @staticmethod
  def remove(id, callback):
    Timer.TIMER_PROXY.remove_timer(id, callback)


# 角色对象
class EntityManager:
  ENTITY_PROXY = None

  @staticmethod
  def initialize():
    EntityManager.ENTITY_PROXY = variable_table.get_variable(ccvariable.ttypes.Variable.ENTITY_MANAGER)

  @staticmethod
  def get_actor(id):
    if EntityManager.ENTITY_PROXY.get_actor(id) == False:
      return None;
    return variable_table.get_variable(ccvariable.ttypes.Variable.CURRENT_ACTOR)

  @staticmethod
  def get_npc(id):
    if EntityManager.ENTITY_PROXY.get_npc(id) == False:
      return None;
    return variable_table.get_variable(ccvariable.ttypes.Variable.CURRENT_NPC)



# Function
class Function:
  FUNCTION_PROXY = None

  @staticmethod
  def initialize():
    Function.FUNCTION_PROXY = variable_table.get_variable(ccvariable.ttypes.Variable.FUNCTION_PROXY)

  @staticmethod
  def send_inform_message(actor, inform_type, **params):
    return Function.FUNCTION_PROXY.send_inform_message(actor, inform_type, params);

  # Function.broadcast_inform_message(ccentity.inform.ttypes.InformType.ATTR_SWORD_TOP_LEVEL, ["aaa", "bbb"]);
  @staticmethod
  def broadcast_inform_message(inform_type, params):
    return Function.FUNCTION_PROXY.broadcast_inform_message(inform_type, params);

  @staticmethod
  def get_playing_configure_file():
    return Function.FUNCTION_PROXY.get_playing_configure_file();



def initialize():
  Communicator.initialize();
  Request.initialize();
  Logging.initialize();
  Timer.initialize();
  EntityManager.initialize();
  Function.initialize();

