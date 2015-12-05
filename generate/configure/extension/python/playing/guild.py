
# coding=utf-8

import sys
sys.path.append('ccentity')
sys.path.append('ccevent')
sys.path.append('ccrequest')
sys.path.append('ccvariable')

import ccentity.playing.ttypes

import ccevent.ttypes

import ccrequest.ttypes
import ccrequest.playing.ttypes

import proxy

import guild_types


# 请求领取奖励
def award_actor(actor, template, result):
  pass


# 更新记录
def update_record(actor, playing_template, result, record):
  if record.spend_secs_ <= 0:
    # 更新副本记录
    request = ccrequest.playing.ttypes.RequestPlayingUpdateRecord()
    request.actor_ = actor
    request.playing_template_ = playing_template
    request.record_ = ccentity.playing.ttypes.PlayingRecordField(
        record.period_count_, 1, record.first_pass_awarded_)
    request.record_.values_ = []
    # 发送请求
    proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_UPDATE_RECORD, request)
    proxy.Logging.debug("[guild] request REQUEST_PLAYING_UPDATE_RECORD")


# 初始化
def initialize():
  for i in guild_types.Config.PLAYING_TEMPLATE_IDS_:
    proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_CREATE, i,
        ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_guild_on_event_playing_create")
    proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_DESTORY, i,
        ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_guild_on_event_playing_destroy")

  if guild_types.Config.initialize() is False:
    proxy.Logging.error("[guild] initialize config failed")

  proxy.Logging.debug("[guild] initialize")
  return True


# 结束时
def finalize():
  for i in guild_types.Config.PLAYING_TEMPLATE_IDS_:
    proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_CREATE, i,
        ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_guild_on_event_playing_create")
    proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_DESTORY, i,
        ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_guild_on_event_playing_destroy")

  proxy.Logging.debug("[guild] finalize")

