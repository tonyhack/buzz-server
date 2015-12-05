
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

import log
import proxy
import variable_table

import plot_types

from thrift import TSerialization


# 增加奖励(玩家ID, 副本模板ID, 分数)
def add_award(actor, template, score):
  log.log_debug("plot.add_award(%d, %d, %d)" % (actor, template, score))

  playing_config = plot_types.Config.get(template)
  if playing_config == None:
    log.log_error("找不到日常副本配置(%d)" % template)
    return None

  request = ccrequest.playing.ttypes.RequestPlayingAddAward()
  request.actor_ = actor
  request.playing_template_ = template

  award_field = ccentity.playing.ttypes.PlayingAwardField()
  award_field.type_ = ccentity.resource.ttypes.ResourceType.RESOURCE

  request.awards_ = []

  # 奖励
  award_field.type_ = ccentity.resource.ttypes.ResourceType.ITEM
  for i in playing_config.get_awards():
    request.awards_.append(i)

  # 抽奖奖励
  draw_items = playing_config.get_draw_items();
  draw_items_size = len(draw_items)
  if draw_items_size > 0:
    request.draw_award_ = ccentity.playing.ttypes.PlayingAwardField( \
        ccentity.resource.ttypes.ResourceType.ITEM, \
        draw_items[random.randint(0, draw_items_size - 1)], 1)

  if proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_ADD_AWARD,\
    request) == False:
    proxy.Logging.error("发送请求 REQUEST_PLAYING_ADD_AWARD 出错");
    return None

  proxy.Logging.debug("增加奖励(玩家=%d) 成功" % actor)


# 请求领取奖励
def award_actor(actor, template, result):
  proxy.Logging.debug("[plot] award_actor(%d,%d)" % (actor, template))
  if result.awarded_ == True:
    return None
  # 配置
  playing_config = plot_types.Config.get(template)
  if playing_config == None:
    log.log_error("找不到日常副本配置(%d)" % template)
    return None
  # 抽奖
  draw_items = playing_config.get_draw_items();
  if draw_items == None:
    proxy.Logging.error("[plot] playing_config.get_draw_items() failed.")
    return None
  score = result.values_[1]
  # 请求增加副本奖励
  request = ccrequest.playing.ttypes.RequestPlayingAwardActor()
  request.actor_ = actor
  request.playing_template_ = template
  request.awards_ = []
  # 固定奖励
  award_field = ccentity.playing.ttypes.PlayingAwardField()
  award_field.type_ = ccentity.resource.ttypes.ResourceType.ITEM
  for award in playing_config.get_awards():
    request.awards_.append(award)
  # 抽奖
  draw_award = None
  playing_id = plot_types.PlayingManager.get_actor_playing(actor)
  if playing_id != 0:
    playing = plot_types.PlayingManager.get(playing_id)
    if playing != None:
      playing_actor = playing.get_actor(actor)
      if playing_actor != None:
        draw_award = playing_actor.get_draw_award_item()
  if draw_award == None:
    draw_items_size = len(draw_items)
    if draw_items_size > 0:
      request.awards_.append(draw_items[random.randint(0, draw_items_size - 1)])
  else:
    request.awards_.append(draw_award)
  # 发送请求
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_AWARD_ACTOR, request)
  proxy.Logging.debug("[plot] request REQUEST_PLAYING_AWARD_ACTOR")


# 更新记录
def update_record(actor, playing_template, result, record):
  result_value_size = len(result.values_)
  if result_value_size != 2:
    proxy.Logging.error("[plot] update_record(%d,%d) result values size=%d" %\
        (actor, playing_template, result_value_size))
    return None
  # 副本时间
  result_spend_secs = result.values_[0]
  # 星级
  result_score = result.values_[1]
  record_score = 0
  # 记录变量个数
  record_value_size = len(record.values_)
  if record_value_size == 1:
    # 星级
    record_score = record.values_[0]
  # 副本时间
  record_spend_secs = record.spend_secs_
  # 如果星级小于当前记录，直接返回
  # 如果星级相同，时间不小于当前记录，直接返回
  if result_score < record_score:
    return None
  elif result_score == record_score and result_spend_secs >= record_spend_secs:
    return None
  else:
    # 更新副本记录
    request = ccrequest.playing.ttypes.RequestPlayingUpdateRecord()
    request.actor_ = actor
    request.playing_template_ = playing_template
    request.record_ = ccentity.playing.ttypes.PlayingRecordField(record.period_count_,
      result_spend_secs, record.first_pass_awarded_)
    request.record_.values_ = []
    request.record_.values_.append(result_score)
    # 发送请求
    proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_UPDATE_RECORD, request)
    proxy.Logging.debug("[plot] request REQUEST_PLAYING_UPDATE_RECORD")


# 自动完成副本
def auto_finish(actor, playing_template, record, result, finish_count):
  spend_secs = record.spend_secs_
  score = record.values_[0]
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
  request.result_.values_.append(score)
  # 发送请求
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_UPDATE_RESULT, request)
  proxy.Logging.debug("[plot] request REQUEST_PLAYING_UPDATE_RESULT")


# 初始化
def initialize():
  # 日常副本配置
  for i in range(1, 7):
    playing_config = plot_types.PlayingConfig(i)
    if playing_config.initialize() == False:
      proxy.Logging.error("[plot] init PlayingConfig(%d) failed." % i)
      return False
    playing_config.set_hide_npc(200106)
    plot_types.Config.add(playing_config)

  # 获取事件交互代理
  communicator_proxy = variable_table.get_variable(ccvariable.ttypes.Variable.COMMUNICATOR_PROXY)

  # 关注增加副本奖励事件
  communicator_proxy.follow(ccevent.ttypes.EventType.EVENT_PLAYING_REQUEST_ADD_AWARD, 1, \
      ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_plot_on_event_playing_request_add_award")

  # 关注副本创建/销毁事件(副本1~6)
  for pos in range(1, 7):
    communicator_proxy.follow(ccevent.ttypes.EventType.EVENT_PLAYING_CREATE, pos, \
        ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_plot_on_event_playing_create")
    communicator_proxy.follow(ccevent.ttypes.EventType.EVENT_PLAYING_DESTORY, pos, \
        ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_plot_on_event_playing_destory")

  proxy.Logging.debug("[plot] initialize")

  return True


# 结束时
def finalize():
  communicator_proxy = variable_table.get_variable(ccvariable.ttypes.Variable.COMMUNICATOR_PROXY)

  # 取消关注副本创建/销毁
  for pos in range(1, 7):
    communicator_proxy.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_CREATE, pos, \
        ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_plot_on_event_playing_create")
    communicator_proxy.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_DESTORY, pos, \
        ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_plot_on_event_playing_destory")

  proxy.Logging.debug("[plot] finalize")

