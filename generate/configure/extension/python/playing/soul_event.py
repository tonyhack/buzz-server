
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

import soul_types

from thrift import TSerialization


# 副本创建
def playing_soul_on_event_playing_create(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingCreate()
  TSerialization.deserialize(message, serialize)

  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_ENTER,\
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING,\
      "playing_soul_on_event_playing_actor_enter")
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_LEAVE,\
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING,\
      "playing_soul_on_event_playing_actor_leave")

  playing = soul_types.Playing(message.playing_, message.template_, message.scene_)
  soul_types.PlayingManager.add(playing)

  # 定时器，副本时间
  proxy.Timer.add(message.playing_, soul_types.Config.get_playing_secs() * 1000, 1,\
      "playing_soul_on_timer_playing")

  proxy.Logging.debug("[soul] playing(%d,%d,%d) create"\
      % (message.playing_, message.template_, message.scene_))


# 副本结束
def playing_soul_on_event_playing_destory(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingDestory()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_ENTER,\
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING,\
      "playing_soul_on_event_playing_actor_enter")
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_LEAVE,\
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING,\
      "playing_soul_on_event_playing_actor_leave")

  playing = soul_types.PlayingManager.get(message.playing_)
  if playing != None and playing.get_complete() == False:
    proxy.Timer.remove(message.playing_, "playing_soul_on_timer_playing")

  soul_types.PlayingManager.remove(message.playing_)

  proxy.Logging.debug("[soul] playing(%d,%d) destory"\
      % (message.playing_, message.template_))


# 玩家进入副本
def playing_soul_on_event_playing_actor_enter(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingActorEnter()
  TSerialization.deserialize(message, serialize)

  # 关注玩家杀死NPC事件、请求完成副本事件
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_ACTOR_KILL_NPC,\
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR,\
      "playing_soul_on_event_actor_kill_npc")

  playing = soul_types.PlayingManager.get(message.playing_)
  if playing == None:
    return None

  actor = playing.get_actor(message.actor_)
  if actor == None:
    actor = soul_types.Actor(message.actor_)
    playing.add_actor(actor)

  # 副本管理器中建立一个玩家ID和副本ID的映射关系
  soul_types.PlayingManager.add_actor(message.actor_, message.playing_)

  now = time.time()

  # 请求初始化玩家
  request = ccrequest.playing.ttypes.RequestPlayingInitializeActor()
  request.actor_ = message.actor_
  request.spend_time_ = now - playing.get_start_time()
  request.datas_ = []
  request.scores_ = []
  # 发送请求
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_INITIALIZE_ACTOR,\
      request)

  proxy.Logging.debug("[soul] actor(%d) enter into playing" % message.actor_)


# 玩家离开副本时
def playing_soul_on_event_playing_actor_leave(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingActorLeave()
  TSerialization.deserialize(message, serialize)

  # 取消关注玩家杀死NPC事件
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_ACTOR_KILL_NPC,\
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR,\
      "playing_soul_on_event_actor_kill_npc")

  # 副本管理器中删除玩家ID和副本ID的映射关系
  soul_types.PlayingManager.remove_actor(message.actor_)

  proxy.Logging.debug("[soul] actor(%d) leave from playing" % message.actor_)


# 玩家杀死NPC
def playing_soul_on_event_actor_kill_npc(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.actor.ttypes.EventActorKillNpc()
  TSerialization.deserialize(message, serialize)

  # 获取玩家所在副本ID
  playing_id = soul_types.PlayingManager.get_actor_playing(message.actor_)
  if playing_id == None or playing_id == 0:
    proxy.Logging.error("[soul] PlayingManager.get_actor_playing(%d) failed" % message.actor_)
    return None

  playing = soul_types.PlayingManager.get(playing_id)
  if playing == None:
    proxy.Logging.error("[soul] PlayingManager.get(%d) failed" % playing_id)
    return None

  actor = playing.get_actor(message.actor_)
  if actor == None:
    proxy.Logging.error("[soul] playing.get_actor(%d) failed" % message.actor_)
    return None

  if message.npc_template_ == soul_types.Config.get_boss():
    request = ccrequest.playing.ttypes.RequestPlayingComplete()
    request.playing_ = playing_id
    result = ccentity.playing.ttypes.PlayingResultField()
    result.values_ = []
    result.award_count_ = 1
    result.paid_award_count_ = 0;
    result.awarded_ = False
    request.result_ = result
    request.awards_ = []
    # 得到奖励
    awards = soul_types.Config.get_awards()
    if awards == None:
      proxy.Logging.error("[soul] Config.get_awards() failed")
      return None
    for i in range(0, len(awards) - 1):
      request.awards_.append(awards[i])
    request.draw_awards_ = []
    # 得到抽奖奖励
    draw_awards = soul_types.Config.get_draw_awards()
    if draw_awards == None:
      proxy.Logging.error("[soul] Config.get_draw_awards() failed")
      return None
    draw_awards_size = len(draw_awards)
    if draw_awards_size > 0:
      request.draw_award_ = draw_awards[random.randint(0, draw_awards_size - 1)]
      actor.set_draw_award(request.draw_award_)
    else:
      return None
    # 发送请求
    proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_COMPLETE,\
        request)
    # 消耗玩家副本次数
    request = ccrequest.playing.ttypes.RequestPlayingIncreaseComplete()
    request.playing_ = playing_id
    # 发送请求
    proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_INCREASE_COMPLETE,\
        request)
    playing.set_complete()
    # 取消定时器
    proxy.Timer.remove(playing_id, "playing_soul_on_timer_playing")
    proxy.Logging.debug("[soul] complete playing")

  proxy.Logging.debug("[soul] actor(%d) kill npc(%d)" % (message.actor_, message.npc_template_))


# 副本时间定时器
def playing_soul_on_timer_playing(id):
  # 获取副本对象
  playing = soul_types.PlayingManager.get(id)
  if playing == None:
    proxy.Logging.error("[soul] PlayingManager.get(%d) failed" % id)
    return None

  request = ccrequest.playing.ttypes.RequestPlayingFailure()
  request.playing_ = id
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_FAILURE, request)
  proxy.Logging.debug("[soul] playing time expired")

