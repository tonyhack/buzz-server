
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
def playing_slime_on_event_playing_create(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingCreate()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 关注玩家进入/退出副本事件
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_ENTER, \
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING, \
      "playing_slime_on_event_playing_actor_enter")
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_LEAVE, \
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING, \
      "playing_slime_on_event_playing_actor_leave")

  # 创建副本对象并加入副本管理器
  playing = slime_types.Playing(message.playing_, message.template_, message.scene_)
  slime_types.PlayingManager.add(playing)

  proxy.Logging.debug("[slime] playing create success")


# 副本销毁时
def playing_slime_on_event_playing_destory(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingDestory()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 取消关注玩家进入/退出副本
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_ENTER, \
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING, \
      "playing_slime_on_event_playing_actor_enter")
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_LEAVE, \
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING, \
      "playing_slime_on_event_playing_actor_leave")

  # 从副本管理器中删除副本对象
  slime_types.PlayingManager.remove(message.playing_)

  proxy.Logging.debug("[slime] playing destory success")


# 玩家进入副本时
def playing_slime_on_event_playing_actor_enter(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingActorEnter()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 关注玩家杀死NPC事件、请求完成副本事件
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_ACTOR_KILL_NPC,\
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR,\
      "playing_slime_on_event_actor_kill_npc")
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_REQUEST_COMPLETE,\
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR,\
      "playing_slime_on_event_playing_actor_request_complete")

  playing = slime_types.PlayingManager.get(message.playing_)
  if playing == None:
    return None

  now = time.time()

  actor = playing.get_actor()
  if actor == 0:
    playing.set_start_time(now)
    playing.set_actor(message.actor_)
    # 消耗玩家副本次数
    request = ccrequest.playing.ttypes.RequestPlayingIncreaseComplete()
    # request.actor_ = message.actor_
    # request.playing_template_ = message.template_
    request.playing_ = message.playing_
    # 发送请求
    proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_INCREASE_COMPLETE,\
        request)
    proxy.Timer.add(message.playing_, slime_types.Config.get_slime_time() * 1000, 1, "playing_slime_on_timer_slime")

  # 副本管理器中建立一个玩家ID和副本ID的映射关系
  slime_types.PlayingManager.add_actor(message.actor_, message.playing_)

  # 请求初始化玩家
  request = ccrequest.playing.ttypes.RequestPlayingInitializeActor()
  request.actor_ = message.actor_
  request.spend_time_ = now - playing.get_start_time()
  # datas = [剩余体力, 阶段, 当前阶段剩余时间]
  request.datas_ = []
  request.datas_.append(slime_types.Config.get_max_power() - playing.get_spend_power())
  if playing.get_section_boss() == True:
    request.datas_.append(2)
    request.datas_.append(now - playing.get_boss_summon_time())
  else:
    request.datas_.append(1)
    # 第一阶段剩下的时间
    elapse_time = now - playing.get_start_time()
    if elapse_time <= slime_types.Config.get_slime_time():
      request.datas_.append(slime_types.Config.get_slime_time() - elapse_time)
    else:
      request.datas_.append(0)
  request.scores_ = []
  request.scores_.append(ccentity.playing.ttypes.PlayingScoreField(\
    ccentity.playing.ttypes.PlayingScoreType.KILL_NPC, 0, playing.get_score()))
  request.scores_.append(ccentity.playing.ttypes.PlayingScoreField(\
    ccentity.playing.ttypes.PlayingScoreType.POWER, 0,\
    slime_types.Config.get_max_power() - playing.get_spend_power()))
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_INITIALIZE_ACTOR,\
      request)

  # 转为临时技能形态
  facade_request.change_skill_form(ccentity.entity.ttypes.EntityType.TYPE_ACTOR,\
      message.actor_, ccentity.skill.ttypes.SkillFormType.TEMP)
  # 增加临时技能
  facade_request.add_temp_skill(ccentity.entity.ttypes.EntityType.TYPE_ACTOR,\
      message.actor_, 100, 1)
  facade_request.add_temp_skill(ccentity.entity.ttypes.EntityType.TYPE_ACTOR,\
      message.actor_, 200, 1)
  facade_request.add_temp_skill(ccentity.entity.ttypes.EntityType.TYPE_ACTOR,\
      message.actor_, 300, 1)

  proxy.Logging.debug("[slime] actor(%d) enter into playing" % message.actor_)



# 玩家离开副本时
def playing_slime_on_event_playing_actor_leave(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingActorLeave()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 取消关注玩家杀死NPC事件
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_ACTOR_KILL_NPC,\
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR,\
      "playing_slime_on_event_actor_kill_npc")
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_REQUEST_COMPLETE,\
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR,\
      "playing_slime_on_event_playing_actor_request_complete")

  # 副本管理器中删除玩家ID和副本ID的映射关系
  slime_types.PlayingManager.remove_actor(message.actor_)

  # 移除临时技能
  facade_request.remove_temp_skill(ccentity.entity.ttypes.EntityType.TYPE_ACTOR,\
      message.actor_, 100)
  facade_request.remove_temp_skill(ccentity.entity.ttypes.EntityType.TYPE_ACTOR,\
      message.actor_, 200)
  facade_request.remove_temp_skill(ccentity.entity.ttypes.EntityType.TYPE_ACTOR,\
      message.actor_, 300)
  # 转为普通技能形态
  facade_request.change_skill_form(ccentity.entity.ttypes.EntityType.TYPE_ACTOR,\
      message.actor_, ccentity.skill.ttypes.SkillFormType.COMMON)

  proxy.Logging.debug("[slime] actor(%d) leave from playing" % message.actor_)



# 玩家杀死npc时
def playing_slime_on_event_actor_kill_npc(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.actor.ttypes.EventActorKillNpc()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 获取玩家所在副本ID
  playing_id = slime_types.PlayingManager.get_actor_playing(message.actor_)
  if playing_id == None or playing_id == 0:
    proxy.Logging.error("[slime] PlayingManager.get_actor_playing(%d) failed" % message.actor_)
    return None

  # 获取副本对象
  playing = slime_types.PlayingManager.get(playing_id)
  if playing == None:
    proxy.Logging.error("[slime] PlayingManager.get(%d) failed" % playing_id)
    return None

  if message.npc_template_ != slime_types.Config.get_boss():
    # 获取副本棋盘
    chessboard = playing.get_chessboard()
    if chessboard == None:
      proxy.Logging.error("[slime] playing.get_chessboard() failed")
      return None
 
    # 获取当前NPC的坐标
    x, y = chessboard.get_pos(message.npc_)
    if x < 0 or y < 0:
      proxy.Logging.error("[slime] chessboard.get_pos(%d) failed" % message.npc_)
      return None
 
    # 删除当前NPC
    if chessboard.erase_by_pos(x, y) == False:
      proxy.Logging.error("[slime] chessboard.erase_by_pos(%d, %d) failed" % (x, y))
      return None
 
    score = chessboard.erase_associated_by_pos(message.npc_template_, x, y) + 1

    # 增加分数
    playing.add_score(score)

    # 通知客户端得分
    request = ccrequest.playing.ttypes.RequestPlayingSynchronizeScore()
    request.actor_ = message.actor_
    request.score_ = ccentity.playing.ttypes.PlayingScoreField(ccentity.playing.ttypes.PlayingScoreType.KILL_NPC,\
        0, playing.get_score())
    proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_SYNCHRONIZE_SCORE, request)
 
    # 增加消耗的体力值
    playing.add_spend_power(1);

    # 同步体力值
    request = ccrequest.playing.ttypes.RequestPlayingSynchronizeScore()
    request.actor_ = message.actor_
    request.score_ = ccentity.playing.ttypes.PlayingScoreField(ccentity.playing.ttypes.PlayingScoreType.POWER,\
        0, slime_types.Config.get_max_power() - playing.get_spend_power())
    proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_SYNCHRONIZE_SCORE, request)

    # 计算经验
    exp = 0
    if score >= slime_types.Config.get_multi_kill_num():
      exp = score * slime_types.Config.get_multi_kill_exp()
    else:
      exp = score * slime_types.Config.get_kill_exp()

    playing.add_exp(exp)

    # 获取玩家对象
    actor = proxy.EntityManager.get_actor(message.actor_)
    if actor == None:
      proxy.Logging.error("[slime] get actor(%d) failed" % message.actor_)
      return None

    # 增加资源
    actor.add_resource(ccentity.resource.ttypes.ResourceID.EXP, exp)

    # 通知客户端经验增加
    request.actor_ = message.actor_
    request.score_ = ccentity.playing.ttypes.PlayingScoreField(ccentity.playing.ttypes.PlayingScoreType.EXP,\
        0, playing.get_exp())
    proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_SYNCHRONIZE_SCORE, request)
 
    if playing.get_spend_power() == slime_types.Config.get_max_power():
      # 删除所有水晶NPC
      chessboard.erase_all()
      x, y = slime_types.Config.get_boss_position()
      facade_request.summon_npc(playing.get_scene(), slime_types.Config.get_boss(), x, y)
      proxy.Timer.add(playing_id, slime_types.Config.get_boss_time() * 1000, 1, "playing_slime_on_timer_boss")
      # 设置为Boss阶段
      playing.set_section_boss()
      playing.set_boss_summon_time(time.time())
    else:
      # 刷新当前NPC
      # refresh_by_pos(x, y)
      chessboard.refresh_all()
  else:
    playing.set_kill_boss()
    # 通知客户端得分
    request = ccrequest.playing.ttypes.RequestPlayingSynchronizeScore()
    request.actor_ = message.actor_
    request.score_ = ccentity.playing.ttypes.PlayingScoreField(ccentity.playing.ttypes.PlayingScoreType.KILL_NPC,\
        message.npc_template_, 1)
    proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_SYNCHRONIZE_SCORE, request)
    proxy.Timer.remove(playing_id, "playing_slime_on_timer_boss")
    proxy.Logging.debug("[slime] actor kill boss")

  proxy.Logging.debug("[slime] actor(%d) kill npc, score=%d" % (message.actor_, playing.get_score()))



def playing_slime_on_timer_slime(id):
  # 获取副本对象
  playing = slime_types.PlayingManager.get(id)
  if playing == None:
    proxy.Logging.error("[slime] PlayingManager.get(%d) failed" % id)
    return None
  chessboard = playing.get_chessboard()
  if chessboard == None:
    proxy.Logging.error("[slime] playing.get_chessboard() failed")
    return None
  chessboard.erase_all()
  x, y = slime_types.Config.get_boss_position()
  facade_request.summon_npc(playing.get_scene(), slime_types.Config.get_boss(), x, y)
  proxy.Timer.add(id, slime_types.Config.get_boss_time() * 1000, 1, "playing_slime_on_timer_boss")
  # 设置为Boss阶段
  playing.set_section_boss()
  playing.set_boss_summon_time(time.time())
  proxy.Logging.debug("[slime] actor(%d) slime time expired." % playing.get_actor())


def playing_slime_on_timer_boss(id):
  # 获取副本对象
  playing = slime_types.PlayingManager.get(id)
  if playing == None:
    proxy.Logging.error("[slime] PlayingManager.get(%d) failed" % id)
    return None

  # 获取玩家对象
  actor = proxy.EntityManager.get_actor(playing.get_actor())
  if actor == None:
    proxy.Logging.error("[slime] get actor(%d) failed" % playing.get_actor())
    return None

  status = actor.get_basic_status()
  if status == ccentity.role.ttypes.RoleBasicStatus.STATUS_DIE:
    # 先复活玩家
    request = ccrequest.scene.ttypes.RequestSceneRevive()
    request.actor_ = message.id_
    request.stay_revive_ = True
    # 序列化
    request_data = TSerialization.serialize(request)
    # 发送请求
    request_proxy.request(ccrequest.ttypes.RequestType.REQUEST_SCENE_REVIVE, \
        request_data, len(request_data))

  request = ccrequest.playing.ttypes.RequestPlayingFailure()
  request.playing_ = id
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_FAILURE, request)
  proxy.Logging.debug("[slime] actor(%d) boss time expired." % playing.get_actor())



# 玩家请求完成副本
def playing_slime_on_event_playing_actor_request_complete(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingActorRequestComplete()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 获取副本对象
  playing = slime_types.PlayingManager.get(message.playing_)
  if playing == None:
    proxy.Logging.error("[slime] PlayingManager.get(%d) failed." % message.playing_)
    return None

  if playing.get_section_boss() == False:
    proxy.Logging.error("[slime] finish failed get_section_boss().")
    return None

  if playing.get_kill_boss() == False:
    proxy.Logging.error("[slime] finish failed, get_kill_boss()")
    return None

  # 请求完成消息
  request = ccrequest.playing.ttypes.RequestPlayingComplete()
  # request.actor_ = message.actor_
  request.playing_ = message.playing_

  result = ccentity.playing.ttypes.PlayingResultField()
  result.values_ = []
  result.award_count_ = 1
  result.paid_award_count_ = 0;
  result.awarded_ = False
  request.result_ = result

  # 得到奖励配置对象
  award_config = slime_types.Config.get_award()
  if award_config == None:
    proxy.Logging.error("[slime] Config.get_award() failed.")
    return None

  # 抽奖
  draw_awards = award_config.get_draw_awards()
  if draw_awards == None:
    proxy.Logging.error("[slime] award_config.get_draw_awards() failed.")
    return None
  draw_awards_size = len(draw_awards)
  if draw_awards_size > 0:
    request.draw_award_ = draw_awards[random.randint(0, draw_awards_size - 1)]
    playing.set_draw_award(request.draw_award_)
  else:
    return None

  # 固定奖励
  # request.awards_ = award_config.get_awards()
  request.awards_ = []
  awards = award_config.get_awards()
  for i in range(0, len(awards) - 1):
    request.awards_.append(awards[i])

  # 发送请求
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_COMPLETE,\
      request)

  # 取消BOSS定时器
  proxy.Timer.remove(message.playing_, "playing_slime_on_timer_boss")

  proxy.Logging.debug("[slime] actor(%d) complete playing.")

