
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

import team_types
import playing_types

from thrift import TSerialization



# 请求领取奖励
def award_actor(actor, template, result, record):
  proxy.Logging.debug("[team] award_actor(%d,%d)" % (actor, template))

  playing_config = playing_types.Config.get(template)
  if playing_config == None:
    proxy.Logging.debug("[team] playing_types.Config.get(%d) failed." % template)
    return None

  request = ccrequest.playing.ttypes.RequestPlayingAwardActor()
  request.actor_ = actor
  request.playing_template_ = template
  request.first_pass_award_ = True
  request.awards_ = []

  if record.first_pass_awarded_ == False:
    for award in playing_config.first_pass_awards_:
      request.awards_.append(award)
    proxy.Logging.debug("[team] add_award first_pass_award")

  """
  if result.awarded_ == False:
    for award in playing_config.awards_:
      request.awards_.append(award)
    proxy.Logging.debug("[team] add_award award")
  """

  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_AWARD_ACTOR, request)
  proxy.Logging.debug("[team] request REQUEST_PLAYING_AWARD_ACTOR")


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
    proxy.Logging.debug("[team] request REQUEST_PLAYING_UPDATE_RECORD")


# 初始化
def initialize():
  monster_pos = [[117,40], [68,72], [17,39], [67,13], [66,37]]

  for i in range(100, 108):
    playing_cell = playing_types.Config.get(i)
    if playing_cell == None:
      proxy.Logging.error("[soul] playing_types.Config.get(%d) failed." % i)
      return False

    playing_config = team_types.PlayingConfig(i)
    playing_config.set_playing_time(playing_cell.finish_time_)
    playing_config.set_pass_boss(playing_cell.pass_boss_)
    team_types.Config.add(playing_config)

    pos = 0
    for npc_id in playing_cell.monster_order_:
      if pos <= len(monster_pos):
        playing_config.push_next_npc(npc_id, monster_pos[pos][0], monster_pos[pos][1])
      else:
        playing_config.push_next_npc(npc_id, monster_pos[0][0], monster_pos[0][1])
      pos += 1

    proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_CREATE, i,\
        ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_team_on_event_playing_create")
    proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_DESTORY, i,\
        ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_team_on_event_playing_destory")

  proxy.Logging.debug("[team] initialize")


def finalize():
  for i in range(100, 108):
    proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_CREATE, i,\
        ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_team_on_event_playing_create")
    proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_DESTORY, i,\
        ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_team_on_event_playing_destory")

  proxy.Logging.debug("[team] finalize")

