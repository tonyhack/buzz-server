
# coding=utf-8

import sys
sys.path.append('ccentity')
sys.path.append('ccevent')
sys.path.append('ccrequest')
sys.path.append('ccvariable')

import __main__

import random
import time

import ccentity
import ccentity.entity
import ccentity.entity.ttypes
import ccentity.playing
import ccentity.playing.ttypes

import ccevent
import ccevent.ttypes
import ccevent.actor.ttypes
import ccevent.playing.ttypes
import ccevent.role.ttypes

import ccrequest
import ccrequest.ttypes
import ccrequest.playing.ttypes
import ccrequest.scene.ttypes

import ccvariable
import ccvariable.ttypes

import variable_table
import log

import idol_types
import facade_request

from thrift import TSerialization


# 副本创建时
def playing_idol_on_event_playing_create(message_type, channel, channel_type, serialize):
  # 获取事件交互代理
  communicator_proxy = variable_table.get_variable(ccvariable.ttypes.Variable.COMMUNICATOR_PROXY)

  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingCreate()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 关注玩家进入/退出副本事件
  communicator_proxy.follow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_ENTER, \
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING, \
      "playing_idol_on_event_playing_actor_enter")
  communicator_proxy.follow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_LEAVE, \
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING, \
      "playing_idol_on_event_playing_actor_leave")

  # 创建副本对象并加入管理器中
  playing = idol_types.Playing(message.playing_, message.template_, message.scene_)
  idol_types.PlayingManager.add(playing)

  # 获取副本配置
  playing_config = idol_types.Config.get(message.template_)
  if playing_config == None:
    log.log_error("获取 副本配置失败(%d)" % message.template_)
    return None

  # 召唤一个BOSS
  facade_request.summon_npc(message.scene_, playing_config.get_pass_kill_npc(),\
      playing_config.get_pass_npc_x(), playing_config.get_pass_npc_y());

  log.log_debug("idol副本(id=%d,template=%d) 创建成功" % (message.playing_, message.template_))



# 副本销毁时
def playing_idol_on_event_playing_destory(message_type, channel, channel_type, serialize):
  # 获取事件交互代理
  communicator_proxy = variable_table.get_variable(ccvariable.ttypes.Variable.COMMUNICATOR_PROXY)

  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingDestory()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 取消关注玩家进入/退出副本
  communicator_proxy.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_ENTER, \
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING, \
      "playing_idol_on_event_playing_actor_enter")
  communicator_proxy.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_LEAVE, \
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING, \
      "playing_idol_on_event_playing_actor_leave")

  # 从管理器中删除副本对象
  idol_types.PlayingManager.remove(message.playing_)

  log.log_debug("副本(%d) 销毁成功" % message.playing_)



# 玩家进入副本时
def playing_idol_on_event_playing_actor_enter(message_type, channel, channel_type, serialize):
  # 获取事件交互代理
  communicator_proxy = variable_table.get_variable(ccvariable.ttypes.Variable.COMMUNICATOR_PROXY)

  # 获取请求代理
  request_proxy = variable_table.get_variable(ccvariable.ttypes.Variable.REQUEST_PROXY)

  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingActorEnter()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 关注玩家杀死NPC事件、角色被杀死事件
  communicator_proxy.follow(ccevent.ttypes.EventType.EVENT_ACTOR_KILL_NPC, \
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR, \
      "playing_idol_on_event_actor_kill_npc")
  communicator_proxy.follow(ccevent.ttypes.EventType.EVENT_ROLE_KILLED, \
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR, \
      "playing_idol_on_event_actor_killed")
  communicator_proxy.follow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_REQUEST_COMPLETE,\
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR,\
      "playing_idol_on_event_playing_actor_request_complete")

  # 获取副本对象
  playing = idol_types.PlayingManager.get(message.playing_)
  if playing == None:
    log.log_error("idol副本 获取 playing(%d) 失败" % message.playing_)
    return None

  now = time.time()

  # 获取玩家对象
  actor = playing.get_actor(message.actor_)
  if actor == None:
    # 玩家不存在时，创建并加入副本管理器中
    actor = idol_types.Actor(message.actor_, now)
    playing.add_actor(actor)

  # 副本管理器中加入一个玩家ID到副本ID的对应关系
  idol_types.PlayingManager.add_actor(message.actor_, message.playing_)

  # 请求初始化玩家
  request = ccrequest.playing.ttypes.RequestPlayingInitializeActor()
  request.actor_ = message.actor_
  request.spend_time_ = now - actor.get_start_time()
  request.scores_ = []
  request.datas_ = []

  # 序列化消息
  request_data = TSerialization.serialize(request)

  # 发送请求
  request_proxy.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_INITIALIZE_ACTOR, \
      request_data, len(request_data))

  log.log_debug("玩家(%d) 进入 idol副本(id=%d,template=%d)" % \
      (message.actor_, message.playing_, message.template_))



# 玩家离开副本时
def playing_idol_on_event_playing_actor_leave(message_type, channel, channel_type, serialize):
  # 获取事件交互代理
  communicator_proxy = variable_table.get_variable(ccvariable.ttypes.Variable.COMMUNICATOR_PROXY)

  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingActorLeave()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 取消关注玩家杀死NPC事件、角色被杀死事件
  communicator_proxy.unfollow(ccevent.ttypes.EventType.EVENT_ROLE_KILLED, \
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR, \
      "playing_idol_on_event_actor_killed")
  communicator_proxy.unfollow(ccevent.ttypes.EventType.EVENT_ACTOR_KILL_NPC, \
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR, \
      "playing_idol_on_event_actor_kill_npc")
  communicator_proxy.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_REQUEST_COMPLETE,\
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR,\
      "playing_idol_on_event_playing_actor_request_complete")

  # 副本管理器中删除玩家对象
  idol_types.PlayingManager.remove_actor(message.actor_)

  log.log_debug("玩家(%d) 离开 idol副本(id=%d,template=%d)" % \
      (message.actor_, message.playing_, message.template_))



# 玩家杀死NPC时
def playing_idol_on_event_actor_kill_npc(message_type, channel, channel_type, serialize):
  # 获取请求代理
  request_proxy = variable_table.get_variable(ccvariable.ttypes.Variable.REQUEST_PROXY)

  # 事件消息
  message = ccevent.actor.ttypes.EventActorKillNpc()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 获取玩家所在副本ID
  playing_id = idol_types.PlayingManager.get_actor_playing(message.actor_)
  if playing_id == None or playing_id == 0:
    return None

  # 获取副本对象
  playing = idol_types.PlayingManager.get(playing_id)
  if playing == None:
    return None

  # 获取副本配置
  playing_config = idol_types.Config.get(playing.get_template_id())
  if playing_config == None:
    log.log_error("获取 副本配置失败(%d)" % playing.get_template_id())
    return None

  # 获取玩家通关需要杀死的NPC
  pass_kill_npc = playing_config.get_pass_kill_npc()
  if message.npc_template_ != pass_kill_npc:
    log.log_error("玩家(%d) 杀死npc(%d) 不是通关npc(%d)" % \
        (message.actor_, message.npc_template_, pass_kill_npc))
    return None

  # 获取副本玩家对象
  actor = playing.get_actor(message.actor_)
  if actor == None:
    log.log_error("idol副本 获取actor(%d) 失败" % message.actor_)
    return None

  if actor.get_finish() == 0 and actor.get_dead_count() <= 0:
    actor.set_finish()
    # 请求同步玩家得分
    request = ccrequest.playing.ttypes.RequestPlayingSynchronizeScore()
    request.actor_ = message.actor_
    request.score_ = ccentity.playing.ttypes.PlayingScoreField(ccentity.playing.ttypes.PlayingScoreType.KILL_NPC, \
        message.npc_template_, 1)
    # 序列化消息
    request_data = TSerialization.serialize(request)
    # 发送请求
    request_proxy.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_SYNCHRONIZE_SCORE, \
        request_data, len(request_data))



# 玩家被杀死事件
def playing_idol_on_event_actor_killed(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.role.ttypes.EventRoleKilled()

  # 消息反序列化
  TSerialization.deserialize(message, serialize)

  # 主角类型只能是玩家
  if message.type_ != ccentity.entity.ttypes.EntityType.TYPE_ACTOR:
    return None

  # 根据玩家ID获取所在副本ID
  playing_id = idol_types.PlayingManager.get_actor_playing(message.id_)
  if playing_id == None or playing_id == 0:
    return None

  # 获取副本对象
  playing = idol_types.PlayingManager.get(playing_id)
  if playing == None:
    return None

  # 获取玩家对象
  actor = playing.get_actor(message.id_)
  if actor == None:
    return None

  # 玩家死亡次数增加
  actor.inc_dead_count()

  log.log_debug("idol副本 玩家(%d) 死亡次数(%d)" % (message.id_, actor.get_dead_count()))

  # 获取请求代理
  request_proxy = variable_table.get_variable(ccvariable.ttypes.Variable.REQUEST_PROXY)

  # 先复活玩家
  request = ccrequest.scene.ttypes.RequestSceneRevive()
  request.actor_ = message.id_
  request.stay_revive_ = True
  # 序列化
  request_data = TSerialization.serialize(request)
  # 发送请求
  request_proxy.request(ccrequest.ttypes.RequestType.REQUEST_SCENE_REVIVE, \
      request_data, len(request_data))

  # 请求失败消息
  request = ccrequest.playing.ttypes.RequestPlayingFailure()
  request.playing_ = playing_id
  # 序列化
  request_data = TSerialization.serialize(request)
  # 发送请求
  request_proxy.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_FAILURE, \
      request_data, len(request_data))

  log.log_debug("idol副本 玩家(%d) 失败" % message.id_)
  


# 玩家请求完成副本
def playing_idol_on_event_playing_actor_request_complete(message_type, channel, channel_type, serialize):
  # 获取请求代理
  request_proxy = variable_table.get_variable(ccvariable.ttypes.Variable.REQUEST_PROXY)

  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingActorRequestComplete()

  # 消息反序列化
  TSerialization.deserialize(message, serialize)

  # 获取副本对象
  playing = idol_types.PlayingManager.get(message.playing_)
  if playing == None:
    log.log_error("获取 副本对象(%d) 失败" % message.playing_)
    return None

  # 获取玩家对象
  actor = playing.get_actor(message.actor_)
  if actor == None:
    log.log_error("获取 玩家对象(%d) 失败" % message.actor_)
    return None

  # 玩家是否完成副本
  if actor.get_finish() == 0:
    log.log_error("玩家(%d) 未成完成idol副本" % message.actor_)
    return None

  # 获取副本配置
  playing_config = idol_types.Config.get(message.template_)
  if playing_config == None:
    log.log_error("获取 副本配置失败(%d)" % message.template_)
    return None

  # 消耗玩家副本次数
  request = ccrequest.playing.ttypes.RequestPlayingIncreaseComplete()
  # request.actor_ = message.actor_
  # request.playing_template_ = message.template_
  request.playing_ = message.playing_
  # 序列化
  request_data = TSerialization.serialize(request)
  # 发送请求
  request_proxy.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_INCREASE_COMPLETE, \
      request_data, len(request_data))

  # 请求完成消息
  request = ccrequest.playing.ttypes.RequestPlayingComplete()
  # request.actor_ = message.actor_
  request.playing_ = message.playing_

  # 抽奖
  actor.draw_award_item(playing_config.get_draw_awards())
  request.draw_award_ = actor.get_draw_award_item()

  # 奖励
  request.awards_ = playing_config.get_awards()

  # 副本耗时
  playing_result = ccentity.playing.ttypes.PlayingResultField()
  playing_result.awarded_ = False
  playing_result.award_count_ = 1
  playing_result.paid_award_count_ = 0;
  playing_result.values_ = []

  # result.value1: 花费时间
  playing_result.values_.append(int(time.time() - actor.get_start_time()))

  # 副本结果
  request.result_ = playing_result

  # 序列化
  request_data = TSerialization.serialize(request)
  # 发送请求
  request_proxy.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_COMPLETE, \
      request_data, len(request_data))

  log.log_debug("玩家完成idol副本(%d)" % message.template_)
  
  # 请求清除场景中的NPC
  request_clear_npc = ccrequest.scene.ttypes.RequestSceneClearAllNpc()
  request_clear_npc.scene_ = playing.get_scene()
  request_clear_npc.delay_secs_ = 5

  # 序列化
  request_clear_npc_data = TSerialization.serialize(request_clear_npc)
  # 发送请求
  request_proxy.request(ccrequest.ttypes.RequestType.REQUEST_SCENE_CLEAR_ALL_NPC, \
      request_clear_npc_data, len(request_clear_npc_data))

  log.log_debug("请求清除 场景(%d) npc" % request_clear_npc.scene_);


