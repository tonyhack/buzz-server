
# coding=utf-8

import sys
sys.path.append('ccentity')
sys.path.append('ccevent')
sys.path.append('ccrequest')
sys.path.append('ccvariable')

import __main__

import copy
import random
import time

import ccentity
import ccentity.entity
import ccentity.entity.ttypes
import ccentity.role
import ccentity.role.ttypes
import ccentity.playing
import ccentity.playing.ttypes

import ccevent
import ccevent.ttypes
import ccevent.actor.ttypes
import ccevent.playing.ttypes
import ccevent.role.ttypes
import ccevent.skill.ttypes

import ccrequest
import ccrequest.ttypes
import ccrequest.playing.ttypes
import ccrequest.scene.ttypes
import ccrequest.skill.ttypes

import ccvariable
import ccvariable.ttypes

import proxy
import facade_request

import slime_types

from thrift import TSerialization


# 副本创建时
def playing_monsters_on_event_playing_create(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingCreate()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 关注玩家进入/退出副本事件
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_ENTER, \
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING, \
      "playing_monsters_on_event_playing_actor_enter")
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_LEAVE, \
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING, \
      "playing_monsters_on_event_playing_actor_leave")

  # 创建副本对象并加入副本管理器
  playing = monsters_types.Playing(message.playing_, message.template_, message.scene_)
  monsters_types.PlayingManager.add(playing)

  proxy.Logging.debug("[monsters] playing create success")


# 副本销毁时
def playing_monsters_on_event_playing_destory(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingDestory()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 取消关注玩家进入/退出副本
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_ENTER, \
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING, \
      "playing_monsters_on_event_playing_actor_enter")
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_LEAVE, \
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING, \
      "playing_monsters_on_event_playing_actor_leave")

  # 从副本管理器中删除副本对象
  monsters_types.PlayingManager.remove(message.playing_)

  proxy.Logging.debug("[monsters] playing create success")


# 玩家进入副本时
def playing_monsters_on_event_playing_actor_enter(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingActorEnter()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 关注玩家杀死NPC事件、请求完成副本事件
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_ACTOR_KILL_NPC,\
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR,\
      "playing_monsters_on_event_actor_kill_npc")
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_REQUEST_COMPLETE,\
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR,\
      "playing_monsters_on_event_playing_actor_request_complete")

  playing = monsters_types.PlayingManager.get(message.playing_)
  if playing == None:
    return None

  now = time.time()

  actor = playing.get_actor()
  if actor == 0:
    playing.set_start_time(now)
    playing.set_actor(message.actor_)
    # 消耗玩家副本次数
    request = ccrequest.playing.ttypes.RequestPlayingIncreaseComplete()
    request.actor_ = message.actor_
    request.playing_template_ = message.template_
    # 发送请求
    proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_INCREASE_COMPLETE,\
        request)
    proxy.Timer.add(message.playing_, monsters_types.Config.get_playing_time(),\
        1, "playing_monsters_on_timer_playing_time")

  # 副本管理器中建立一个玩家ID和副本ID的映射关系
  monsters_types.PlayingManager.add_actor(message.actor_, message.playing_)

  # 请求初始化玩家
  request = ccrequest.playing.ttypes.RequestPlayingInitializeActor()
  request.actor_ = message.actor_
  request.spend_time_ = now - playing.get_start_time()
  ########################################
  # TODO: 考虑做为分数进行同步
  # datas = [祭坛1(0为未完成1为完成)+祭坛2+祭坛3+祭坛4+祭坛5+祭坛6]
  request.datas_ = []
  cages = playing.get_cages()
  for i in range(0, len(cages)):
    if cages[i].get_current_round() < cages[i].get_max_round():
      request.datas_.append(0)
    else:
      request.datas_.append(1)
  ########################################
  request.scores_ = []
  # 循环获取玩家杀死的每个NPC
  for (k,v) in actor.get_kill_npcs().items():
    score_field = ccentity.playing.ttypes.PlayingScoreField()
    score_field.type_ = ccentity.playing.ttypes.PlayingScoreType.KILL_NPC
    score_field.key_ = k;
    score_field.value_ = v;
    request.scores_.append(score_field);
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_INITIALIZE_ACTOR,\
      request)

  proxy.Logging.debug("[monsters] actor(%d) enter into playing" % message.actor_)



# 玩家离开副本时
def playing_monsters_on_event_playing_actor_leave(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingActorLeave()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 取消关注玩家杀死NPC事件
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_ACTOR_KILL_NPC,\
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR,\
      "playing_monsters_on_event_actor_kill_npc")
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_REQUEST_COMPLETE,\
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR,\
      "playing_monsters_on_event_playing_actor_request_complete")

  # 副本管理器中删除
  monsters_types.PlayingManager.remove_actor(message.actor_)

  proxy.Logging.debug("[monsters] actor(%d) leave from playing" % message.actor_)



# 玩家杀死npc时
def playing_monsters_on_event_actor_kill_npc(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.actor.ttypes.EventActorKillNpc()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 获取玩家所在副本ID
  playing_id = monsters_types.PlayingManager.get_actor_playing(message.actor_)
  if playing_id == None or playing_id == 0:
    proxy.Logging.error("[monsters] PlayingManager.get_actor_playing(%d) failed" % message.actor_)
    return None

  # 获取副本对象
  playing = monsters_types.PlayingManager.get(playing_id)
  if playing == None:
    proxy.Logging.error("[monsters] PlayingManager.get(%d) failed" % playing_id)
    return None

  # 获取当前的怪物笼子
  current_cage = playing.get_current_cage()
  if current_cage == None:
    proxy.Logging.error("[monsters] playing.get_current_cage failed.")
    return None

  # 增加杀死NPC数量
  current_cage.add_kill_npc_num(1)

  if current_cage.check_finish_cage() == True:
    


