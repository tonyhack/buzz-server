
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

import ccvariable
import ccvariable.ttypes

import proxy

import slime_types

from thrift import TSerialization


# 增加奖励(玩家ID, 副本模板ID)
def add_award(actor, template):
  proxy.Logging.debug("slime.add_award(%d,%d)" % (actor, template))
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
  # 请求增加副本奖励
  request = ccrequest.playing.ttypes.RequestPlayingAddAward()
  request.actor_ = actor
  request.playing_template_ = template
  # 抽奖
  draw_awards_size = len(draw_awards)
  if draw_awards_size > 0:
    request.draw_award_ = draw_awards[random.randint(0, draw_awards_size - 1)]
  else:
    request.draw_award_ = ccentity.playing.ttypes.PlayingAwardField(\
        ccentity.resource.ttypes.ResourceType.MIN, 0, 0)
  # 固定奖励
  request.awards_ = award_config.get_awards()
  # 发送请求
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_ADD_AWARD, request)


# 请求领取奖励
def award_actor(actor, template, result):
  proxy.Logging.debug("[slime] award_actor(%d,%d)" % (actor, template))
  if result.awarded_ == True:
    return None
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
  # 请求增加副本奖励
  request = ccrequest.playing.ttypes.RequestPlayingAwardActor()
  request.actor_ = actor
  request.playing_template_ = template
  request.awards_ = []
  # 固定奖励
  awards = award_config.get_awards()
  for i in range(0, len(awards) - 1):
    request.awards_.append(awards[i])
  # 抽奖，如果在当前副本中，则使用已抽奖结果
  draw_award = None
  playing_id = slime_types.PlayingManager.get_actor_playing(actor)
  if playing_id != 0:
    playing = slime_types.PlayingManager.get(playing_id)
    if playing != None:
      draw_award = playing.get_draw_award()
  if draw_award == None:
    draw_awards_size = len(draw_awards)
    if draw_awards_size > 0:
      request.awards_.append(draw_awards[random.randint(0, draw_awards_size - 1)])
    else:
      request.awards_.append(ccentity.playing.ttypes.PlayingAwardField(\
          ccentity.resource.ttypes.ResourceType.MIN, 0, 0))
  else:
    request.awards_.append(draw_award)
  # 发送请求
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_AWARD_ACTOR, request)
  proxy.Logging.debug("[slime] request REQUEST_PLAYING_AWARD_ACTOR")


# 更新记录
def update_record(actor, playing_template, result, record):
  if record.spend_secs_ <= 0:
    # 更新副本记录
    request = ccrequest.playing.ttypes.RequestPlayingUpdateRecord()
    request.actor_ = actor
    request.playing_template_ = playing_template
    request.record_ = ccentity.playing.ttypes.PlayingRecordField(record.period_count_, 1, record.first_pass_awarded_)
    request.record_.values_ = []
    # 发送请求
    proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_UPDATE_RECORD, request)
    proxy.Logging.debug("[slime] request REQUEST_PLAYING_UPDATE_RECORD")


# 自动完成副本
def auto_finish(actor, playing_template, record, result, finish_count):
  if finish_count <= 0:
    return None
  spend_secs = record.spend_secs_
  # 更新记录
  request = ccrequest.playing.ttypes.RequestPlayingUpdateRecord()
  request.actor_ = actor
  request.playing_template_ = playing_template
  request.record_ = record
  request.record_.period_count_ += finish_count
  # 发送请求
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_UPDATE_RECORD, request)
  proxy.Logging.debug("[plot] request REQUEST_PLAYING_UPDATE_RECORD")
  # 更新结果
  request = ccrequest.playing.ttypes.RequestPlayingUpdateResult()
  request.actor_ = actor
  request.playing_template_ = playing_template
  request.result_ = ccentity.playing.ttypes.PlayingResultField(\
      False, finish_count, result.paid_award_count_, [])
  # 发送请求
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_UPDATE_RESULT, request)
  proxy.Logging.debug("[plot] request REQUEST_PLAYING_UPDATE_RESULT, finish_count=%d" % finish_count)


# 初始化
def initialize():
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_CREATE, 15,\
      ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_slime_on_event_playing_create")
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_DESTORY, 15,\
      ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_slime_on_event_playing_destory")

  # 4个水晶NPC
  slime_types.Config.add(200801)
  slime_types.Config.add(200802)
  slime_types.Config.add(200803)
  slime_types.Config.add(200804)

  # boss怪
  slime_types.Config.set_boss(200805)
  slime_types.Config.set_boss_position(15, 15)

  # 玩家体力值
  slime_types.Config.set_max_power(50)

  # 经验
  slime_types.Config.set_kill_exp(10000)
  slime_types.Config.set_multi_kill_num(4)
  slime_types.Config.set_multi_kill_exp(20000)

  # 奖励
  awards = []
  awards.append(ccentity.playing.ttypes.PlayingAwardField(\
      ccentity.resource.ttypes.ResourceType.RESOURCE, ccentity.resource.ttypes.ResourceID.EXP, 10000))
  awards.append(ccentity.playing.ttypes.PlayingAwardField(\
      ccentity.resource.ttypes.ResourceType.RESOURCE, ccentity.resource.ttypes.ResourceID.NIMBUS, 500))
  awards.append(ccentity.playing.ttypes.PlayingAwardField(\
      ccentity.resource.ttypes.ResourceType.RESOURCE, ccentity.resource.ttypes.ResourceID.GOLD, 3000))
  awards.append(ccentity.playing.ttypes.PlayingAwardField(\
      ccentity.resource.ttypes.ResourceType.ITEM, 144314001))
  draw_awards = []
  draw_awards.append(ccentity.playing.ttypes.PlayingAwardField(\
      ccentity.resource.ttypes.ResourceType.RESOURCE, ccentity.resource.ttypes.ResourceID.GOLD, 100))
  draw_awards.append(ccentity.playing.ttypes.PlayingAwardField(\
      ccentity.resource.ttypes.ResourceType.RESOURCE, ccentity.resource.ttypes.ResourceID.EXP, 10000))
  draw_awards.append(ccentity.playing.ttypes.PlayingAwardField(\
      ccentity.resource.ttypes.ResourceType.RESOURCE, ccentity.resource.ttypes.ResourceID.RESTRICT_DOLLARS, 100))
  draw_awards.append(ccentity.playing.ttypes.PlayingAwardField(\
      ccentity.resource.ttypes.ResourceType.RESOURCE, ccentity.resource.ttypes.ResourceID.NIMBUS, 100))
  draw_awards.append(ccentity.playing.ttypes.PlayingAwardField(\
      ccentity.resource.ttypes.ResourceType.RESOURCE, ccentity.resource.ttypes.ResourceID.WAR_SOUL, 100))
  draw_awards.append(ccentity.playing.ttypes.PlayingAwardField(\
      ccentity.resource.ttypes.ResourceType.RESOURCE, ccentity.resource.ttypes.ResourceID.FREEDOM_DOLLARS, 100))
  draw_awards.append(ccentity.playing.ttypes.PlayingAwardField(\
      ccentity.resource.ttypes.ResourceType.RESOURCE, ccentity.resource.ttypes.ResourceID.GEM_SHARD, 100))
  slime_types.Config.set_award(slime_types.AwardConfig(awards, draw_awards))

  # 消除宝石时间
  slime_types.Config.set_slime_time(1200)
  # 打BOSS时间
  slime_types.Config.set_boss_time(600)

  proxy.Logging.debug("slime initialize.")

  return True


# 结束
def finalize():
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_CREATE, 15,\
      ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_slime_on_event_playing_create")
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_DESTORY, 15,\
      ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_slime_on_event_playing_destory")

  proxy.Logging.debug("slime finalize.")

