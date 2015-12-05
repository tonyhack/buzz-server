
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

import soul_types
import playing_types

from thrift import TSerialization


# 请求领取奖励
def award_actor(actor_id, template, result):
  proxy.Logging.debug("[soul] award_actor(%d,%d)" % (actor_id, template))
  if result.awarded_ == True:
    return None
  # 得到奖励配置对象
  award_config = soul_types.Config.get_awards()
  if award_config == None:
    proxy.Logging.error("[soul] Config.get_awards() failed.")
    return None
  draw_award_config = soul_types.Config.get_draw_awards()
  if draw_award_config == None:
    proxy.Logging.error("[soul] Config.get_draw_awards() failed.")
    return None
  # 请求增加副本奖励
  request = ccrequest.playing.ttypes.RequestPlayingAwardActor()
  request.actor_ = actor_id
  request.playing_template_ = template
  request.awards_ = []
  # 固定奖励
  for i in range(0, len(award_config) - 1):
    request.awards_.append(award_config[i])
  # 抽奖，如果在当前副本中，则使用已抽奖结果
  draw_award = None
  playing_id = soul_types.PlayingManager.get_actor_playing(actor_id)
  if playing_id > 0:
    draw_award = None
  playing = soul_types.PlayingManager.get(playing_id)
  if playing == None:
    draw_award = None
  else:
    actor = playing.get_actor(actor_id)
    if actor != None:
      draw_award = actor.get_draw_award()
  if draw_award != None:
    request.awards_.append(draw_award)
  else:
    # 抽奖
    draw_awards = soul_types.Config.get_draw_awards()
    if draw_awards == None:
      proxy.Logging.error("[soul] Config.get_draw_awards() failed.")
      return None
    draw_awards_size = len(draw_awards)
    if draw_awards_size > 0:
      request.awards_.append(draw_awards[random.randint(0, draw_awards_size - 1)])
    else:
      request.awards_.append(ccentity.playing.ttypes.PlayingAwardField(\
            ccentity.resource.ttypes.ResourceType.MIN, 0, 0))
  # 发送请求
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_AWARD_ACTOR, request)
  proxy.Logging.debug("[soul] request REQUEST_PLAYING_AWARD_ACTOR")


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
    proxy.Logging.debug("[soul] request REQUEST_PLAYING_UPDATE_RECORD")


# 自动完成副本
def auto_finish(actor, playing_template, record, result, finish_count):
  proxy.Logging.debug("[soul] auto_finish(%d,%d)" % (actor, playing_template))


# 初始化
def initialize():
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_CREATE, 17,\
      ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_soul_on_event_playing_create")
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_DESTORY, 17,\
      ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_soul_on_event_playing_destory")

  playing_cell = playing_types.Config.get(17)
  if playing_cell == None:
    proxy.Logging.error("[soul] playing_types.Config.get(17) failed.")
    return False

  soul_types.Config.set_boss(playing_cell.pass_boss_)
  soul_types.Config.set_playing_secs(playing_cell.finish_time_)
  soul_types.Config.set_draw_awards(playing_cell.draw_items_)

  awards = []
  for i in range(0, len(playing_cell.awards_)):
    awards.append(ccentity.playing.ttypes.PlayingAwardField(\
        playing_cell.awards_[i].type_, playing_cell.awards_[i].id_, playing_cell.awards_[i].number_))

  soul_types.Config.set_awards(awards);

  proxy.Logging.debug("[soul] initialize.")

  return True


# 结束
def finalize():
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_CREATE, 17,\
      ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_soul_on_event_playing_create")
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_DESTORY, 17,\
      ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_soul_on_event_playing_destory")

  proxy.Logging.debug("[soul] finalize.")

