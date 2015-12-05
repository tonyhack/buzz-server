
# coding=utf-8

import sys
sys.path.append('ccentity')
sys.path.append('ccevent')
sys.path.append('ccrequest')
sys.path.append('ccvariable')

import time

import ccentity.playing.ttypes

import ccevent.ttypes
import ccevent.actor.ttypes
import ccevent.playing.ttypes

import ccrequest.ttypes
import ccrequest.playing.ttypes

import proxy

import coliseum_types

from thrift import TSerialization


# 副本创建时
def playing_coliseum_on_event_playing_create(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingCreate()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 关注玩家进入/退出副本事件
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_ENTER,
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING,
      "playing_coliseum_on_event_playing_actor_enter")
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_LEAVE,
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING,
      "playing_coliseum_on_event_playing_actor_leave")

  # 创建副本对象并加入副本管理器
  playing = coliseum_types.Playing(message.playing_, message.template_, message.scene_)
  coliseum_types.PlayingManager.add(playing)

  proxy.Logging.debug("[coliseum] playing(%d,%d,%d) create"
      % (message.playing_, message.template_, message.scene_))


# 副本销毁时
def playing_coliseum_on_event_playing_destroy(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingDestory()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 移除定时器
  proxy.Timer.remove(message.playing_, "playing_coliseum_on_timer_playing_start")
  proxy.Timer.remove(message.playing_, "playing_coliseum_on_timer_summon_next_wave")
  proxy.Timer.remove(message.playing_, "playing_coliseum_on_timer_remove_rune")
  proxy.Timer.remove(message.playing_, "playing_coliseum_on_timer_expired")

  # 取消关注玩家进入/退出副本
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_ENTER,
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING,
      "playing_coliseum_on_event_playing_actor_enter")
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_LEAVE,
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING,
      "playing_coliseum_on_event_playing_actor_leave")

  # 从副本管理器中删除副本对象
  coliseum_types.PlayingManager.remove(message.playing_)

  proxy.Logging.debug("[coliseum] playing(%d,%d) destroy"
      % (message.playing_, message.template_))


# 玩家进入副本时
def playing_coliseum_on_event_playing_actor_enter(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingActorEnter()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 关注玩家杀死NPC事件、玩家被杀死事件、请求完成副本事件
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_ACTOR_KILL_NPC,
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR,
      "playing_coliseum_on_event_actor_kill_npc")
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_REQUEST_COMPLETE,
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR,
      "playing_coliseum_on_event_playing_actor_request_complete")

  playing = coliseum_types.PlayingManager.get(message.playing_)
  if playing is None:
    proxy.Logging.error("[coliseum] PlayingManager.get(%d) failed" % message.playing_)
    return

  now = time.time()

  actor = playing.get_actor()
  if actor == 0:
    playing.set_start_time(now)
    playing.set_actor(message.actor_)
    # 消耗玩家副本次数
    request = ccrequest.playing.ttypes.RequestPlayingIncreaseComplete()
    request.playing_ = message.playing_
    proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_INCREASE_COMPLETE,
        request)
    # 设置副本开始定时器
    proxy.Timer.add(message.playing_, coliseum_types.Config.get_start_countdown() * 1000, 1,
        "playing_coliseum_on_timer_playing_start")
    # 设置副本超时定时器
    proxy.Timer.add(message.playing_, coliseum_types.Config.get_time_limit() * 1000, 1,
        "playing_coliseum_on_timer_expired")

  # 副本管理器中建立一个玩家ID和副本ID的映射关系
  coliseum_types.PlayingManager.add_actor(message.actor_, message.playing_)

  # 请求初始化玩家
  request = ccrequest.playing.ttypes.RequestPlayingInitializeActor()
  request.actor_ = message.actor_
  request.spend_time_ = now - playing.get_start_time()
  # datas = [祭坛激活顺序(6个数字)]
  request.datas_ = []
  request.datas_.extend(playing.get_circle_seq())
  request.scores_ = []
  request.scores_.append(ccentity.playing.ttypes.PlayingScoreField(
      ccentity.playing.ttypes.PlayingScoreType.STAGE, 0, playing.get_current_stage()))
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_INITIALIZE_ACTOR,
      request)

  proxy.Logging.debug("[coliseum] actor(%d) enter into playing" % message.actor_)


# 玩家离开副本时
def playing_coliseum_on_event_playing_actor_leave(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingActorLeave()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 取消关注事件
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_ACTOR_KILL_NPC,
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR,
      "playing_coliseum_on_event_actor_kill_npc")
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_REQUEST_COMPLETE,
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR,
      "playing_coliseum_on_event_playing_actor_request_complete")

  # 副本管理器中删除玩家ID和副本ID的映射关系
  coliseum_types.PlayingManager.remove_actor(message.actor_)

  proxy.Logging.debug("[coliseum] actor(%d) leave from playing" % message.actor_)


# 玩家杀死NPC时
def playing_coliseum_on_event_actor_kill_npc(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.actor.ttypes.EventActorKillNpc()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 获取玩家所在副本对象
  playing = coliseum_types.PlayingManager.get_actor_playing(message.actor_)
  if playing is None:
    proxy.Logging.error("[coliseum] PlayingManager.get_actor_playing(%d) failed" % message.actor_)
    return

  if not playing.is_in_boss_stage():
    # 增加杀死npc数量
    playing.inc_kill_npc_number()
    # 检查是否杀完了所有的怪
    if playing.check_circle_finished():
      playing.summon_rune()
      try_activate_next_circle(playing)
  else:
    # 杀死了boss 副本结束
    if message.npc_template_ == coliseum_types.Config.get_boss().template_id_:
      playing.set_finished()


# 玩家请求完成副本
def playing_coliseum_on_event_playing_actor_request_complete(message_type, channel,
                                                             channel_type, serialize):
  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingActorRequestComplete()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 获取副本对象
  playing = coliseum_types.PlayingManager.get(message.playing_)
  if playing is None:
    proxy.Logging.error("[coliseum] PlayingManager.get(%d) failed" % message.playing_)
    return

  if not playing.check_finished():
    proxy.Logging.error("[coliseum] playing(%d) finish failed check_finished()"
        % message.playing_)
    return

  # 请求完成消息
  request = ccrequest.playing.ttypes.RequestPlayingComplete()
  request.playing_ = message.playing_

  result = ccentity.playing.ttypes.PlayingResultField()
  # 表示不是快速完成
  result.values_ = [0]
  result.award_count_ = 1
  result.paid_award_count_ = 0
  result.awarded_ = False
  request.result_ = result
  # 固定奖励
  request.awards_ = []
  request.awards_.extend(coliseum_types.Config.get_awards())
  # 抽奖奖励
  playing.set_draw_award_item(coliseum_types.Config.get_rand_draw_item())
  request.draw_award_ = playing.get_draw_award_item()

  # 发送请求
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_COMPLETE, request)

  proxy.Logging.debug("[coliseum] actor(%d) complete playing" % message.actor_)


# 副本开始定时器回调
def playing_coliseum_on_timer_playing_start(id):
  # 获取副本对象
  playing = coliseum_types.PlayingManager.get(id)
  if playing is None:
    proxy.Logging.error("[coliseum] PlayingManager.get(%d) failed" % id)
    return
  proxy.Logging.debug("[coliseum] actor(%d) on_timer_playing_start" % playing.get_actor())

  # 设置移除符文定时器
  proxy.Timer.add(id, 1000, -1, "playing_coliseum_on_timer_remove_rune")

  try_activate_next_circle(playing)


# 副本超时定时器回调
def playing_coliseum_on_timer_expired(id):
  # 获取副本对象
  playing = coliseum_types.PlayingManager.get(id)
  if playing is None:
    proxy.Logging.error("[coliseum] PlayingManager.get(%d) failed" % id)
    return

  # 副本失败
  request = ccrequest.playing.ttypes.RequestPlayingFailure()
  request.playing_ = id
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_FAILURE, request)

  proxy.Logging.debug("[coliseum] actor(%d) playing_coliseum_on_timer_expired"
      % playing.get_actor())


# 召唤下一波怪物定时器回调
def playing_coliseum_on_timer_summon_next_wave(id):
  # 获取副本对象
  playing = coliseum_types.PlayingManager.get(id)
  if playing is None:
    proxy.Logging.error("[coliseum] PlayingManager.get(%d) failed" % id)
    return

  proxy.Logging.debug("[coliseum] actor(%d) playing_coliseum_on_timer_summon_next_wave"
      % playing.get_actor())

  if (playing.summon_next_wave() is False) or playing.is_end_wave():
    # 移除刷怪定时器
    proxy.Timer.remove(id, "playing_coliseum_on_timer_summon_next_wave")
    return


# 移除符文定时器回调
def playing_coliseum_on_timer_remove_rune(id):
  # 获取副本对象
  playing = coliseum_types.PlayingManager.get(id)
  if playing is None:
    proxy.Logging.error("[coliseum] PlayingManager.get(%d) failed" % id)
    return

  playing.remove_rune()


# 尝试激活下一个法阵
def try_activate_next_circle(playing):
  # 没有下一个法阵, 召唤BOSS
  if playing.activate_next_circle() is False:
    playing.summon_boss()
    return

  # 召唤怪物
  playing.summon_next_wave()
  # 定时召唤怪物
  proxy.Timer.add(playing.get_id(), coliseum_types.Config.get_wave_interval() * 1000, -1,
      "playing_coliseum_on_timer_summon_next_wave")

