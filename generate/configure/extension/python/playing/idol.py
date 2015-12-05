
# coding=utf-8

import sys
sys.path.append('ccentity')
sys.path.append('ccevent')
sys.path.append('ccrequest')
sys.path.append('ccvariable')

import __main__

import time
import random

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

import log
import variable_table

import idol_types
import proxy

from thrift import TSerialization


# 增加奖励(玩家ID, 副本模板ID)
def add_award(actor, template):
  log.log_debug("idol.add_award(%d, %d)" % (actor, template))

  request_proxy = variable_table.get_variable(ccvariable.ttypes.Variable.REQUEST_PROXY)

  # 获取副本配置
  playing_config = idol_types.Config.get(template)
  if playing_config == None:
    log.log_error("获取 副本配置失败(%d)" % template)
    return None

  # 请求增加副本奖励
  request = ccrequest.playing.ttypes.RequestPlayingAddAward()
  request.actor_ = actor
  request.playing_template_ = template
  # 奖励
  request.awards_ = playing_config.get_awards()
  # 抽奖
  draw_awards = playing_config.get_draw_awards()
  draw_awards_size = len(draw_awards)
  if draw_awards_size > 0:
    request.draw_award_ = draw_awards[random.randint(0, draw_awards_size - 1)]
  else:
    request.draw_award_ = ccentity.playing.ttypes.PlayingAwardField(\
        ccentity.resource.ttypes.ResourceType.MIN, 0, 0)

  # 序列化
  request_data = TSerialization.serialize(request)
  # 发送请求
  request_proxy.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_ADD_AWARD,\
      request_data, len(request_data))



# 请求领取奖励
def award_actor(actor, template, result):
  proxy.Logging.debug("[idol] award_actor(%d,%d)" % (actor, template))
  if result.awarded_ == True:
    return None
  # 获取副本配置
  playing_config = idol_types.Config.get(template)
  if playing_config == None:
    log.log_error("获取 副本配置失败(%d)" % template)
    return None
  # 抽奖
  draw_awards = playing_config.get_draw_awards()
  if draw_awards == None:
    proxy.Logging.error("[idol] playing_config.get_draw_awards() failed.")
    return None
  # 请求增加副本奖励
  request = ccrequest.playing.ttypes.RequestPlayingAwardActor()
  request.actor_ = actor
  request.playing_template_ = template
  # 固定奖励
  request.awards_ = playing_config.get_awards()
  # 抽奖
  draw_award = None
  playing_id = idol_types.PlayingManager.get_actor_playing(actor)
  if playing_id != 0:
    playing = idol_types.PlayingManager.get(playing_id)
    if playing != None:
      playing_actor = playing.get_actor(actor)
      if playing_actor != None:
        draw_award = playing_actor.get_draw_award_item()
  if draw_award == None:
    draw_items_size = len(draw_awards)
    if draw_items_size > 0:
      request.awards_.append(draw_awards[random.randint(0, draw_items_size - 1)])
  else:
    request.awards_.append(draw_award)
  # 发送请求
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_AWARD_ACTOR, request)
  proxy.Logging.debug("[idol] request REQUEST_PLAYING_AWARD_ACTOR")



# 更新记录
def update_record(actor, playing_template, result, record):
  result_value_size = len(result.values_)
  if result_value_size != 1:
    proxy.Logging.error("[idol] update_record(%d,%d) result values size=%d" %\
        (actor, playing_template, result_value_size))
    return None
  # 副本时间
  result_spend_secs = result.values_[0]
  # 副本时间
  record_spend_secs = record.spend_secs_
  # 如果时间不小于当前记录，直接返回
  if record_spend_secs > 0 and result_spend_secs >= record_spend_secs:
    return None
  else:
    # 更新副本记录
    request = ccrequest.playing.ttypes.RequestPlayingUpdateRecord()
    request.actor_ = actor
    request.playing_template_ = playing_template
    request.record_ = ccentity.playing.ttypes.PlayingRecordField(record.period_count_,
      result_spend_secs, record.first_pass_awarded_)
    request.record_.values_ = []
    # 发送请求
    proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_UPDATE_RECORD, request)
    proxy.Logging.debug("[idol] request REQUEST_PLAYING_UPDATE_RECORD")


# 自动完成副本
def auto_finish(actor, playing_template, record, result, finish_count):
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
  request.result_.values_.append(spend_secs)
  # 发送请求
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_UPDATE_RESULT, request)
  proxy.Logging.debug("[plot] request REQUEST_PLAYING_UPDATE_RESULT")


def initialize():
  for pos in range(7, 15):
    playing_config = idol_types.PlayingConfig(pos, 60, 10)
    if playing_config.initialize() == False:
      proxy.Logging.error("[idol] init PlayingConfig(%d) failed." % pos)
      return False
    idol_types.Config.add(playing_config)

  # 获取事件交互代理
  communicator_proxy = variable_table.get_variable(ccvariable.ttypes.Variable.COMMUNICATOR_PROXY)

  # 关注增加副本奖励事件
  communicator_proxy.follow(ccevent.ttypes.EventType.EVENT_PLAYING_REQUEST_ADD_AWARD, 1, \
      ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_idol_on_event_playing_request_add_award")

  # 关注副本创建/销毁事件(副本7~14)
  for pos in range(7, 15):
    communicator_proxy.follow(ccevent.ttypes.EventType.EVENT_PLAYING_CREATE, pos, \
        ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_idol_on_event_playing_create")
    communicator_proxy.follow(ccevent.ttypes.EventType.EVENT_PLAYING_DESTORY, pos, \
        ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_idol_on_event_playing_destory")

  proxy.Logging.debug("[idol] initialize")


def finalize():
  communicator_proxy = variable_table.get_variable(ccvariable.ttypes.Variable.COMMUNICATOR_PROXY)

  # 关注副本创建/销毁事件(副本7~14)
  for pos in range(7, 15):
    communicator_proxy.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_CREATE, pos, \
        ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_idol_on_event_playing_create")
    communicator_proxy.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_DESTORY, pos, \
        ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_idol_on_event_playing_destory")

  proxy.Logging.debug("[idol] finalize")

