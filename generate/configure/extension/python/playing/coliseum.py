
# coding=utf-8

import sys
sys.path.append('ccentity')
sys.path.append('ccevent')
sys.path.append('ccrequest')
sys.path.append('ccvariable')

import ccentity.playing.ttypes
import ccentity.resource.ttypes

import ccevent.ttypes

import ccrequest.ttypes
import ccrequest.playing.ttypes

import proxy

import playing_types
import coliseum_types


# 请求领取奖励
def award_actor(actor, template, result):
  proxy.Logging.debug("[coliseum] award_actor(%d,%d)" % (actor, template))
  if result.awarded_:
    proxy.Logging.error("[coliseum] actor(%d) has been awarded" % actor)
    return

  # 请求增加副本奖励
  request = ccrequest.playing.ttypes.RequestPlayingAwardActor()
  request.actor_ = actor
  request.playing_template_ = template
  # 固定奖励
  request.awards_ = []
  request.awards_.extend(coliseum_types.Config.get_awards())
  # 抽奖奖励, 如果在当前副本中, 则使用已抽奖结果
  playing = coliseum_types.PlayingManager.get_actor_playing(actor)
  if playing is None or playing.get_draw_award_item() is None:
    request.awards_.append(coliseum_types.Config.get_rand_draw_item())
  else:
    request.awards_.append(playing.get_draw_award_item())
  # 快速完成额外获得经验
  if len(result.values_) == 0:
    request.awards_.append(ccentity.playing.ttypes.PlayingAwardField(
        ccentity.resource.ttypes.ResourceType.RESOURCE,
        ccentity.resource.ttypes.ResourceID.EXP,
        coliseum_types.Config.get_auto_finish_extra_exp()))
  # 发送请求
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_AWARD_ACTOR, request)


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
    proxy.Logging.debug("[coliseum] request REQUEST_PLAYING_UPDATE_RECORD")


# 自动完成副本
def auto_finish(actor, playing_template, record, result, finish_count):
  if finish_count <= 0:
    return None
  # 更新记录
  request = ccrequest.playing.ttypes.RequestPlayingUpdateRecord()
  request.actor_ = actor
  request.playing_template_ = playing_template
  request.record_ = record
  request.record_.period_count_ += finish_count
  # 发送请求
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_UPDATE_RECORD, request)
  proxy.Logging.debug("[coliseum] request REQUEST_PLAYING_UPDATE_RECORD")
  # 更新结果
  request = ccrequest.playing.ttypes.RequestPlayingUpdateResult()
  request.actor_ = actor
  request.playing_template_ = playing_template
  request.result_ = ccentity.playing.ttypes.PlayingResultField(
      False, finish_count, result.paid_award_count_, [])
  # 发送请求
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_UPDATE_RESULT, request)
  proxy.Logging.debug("[coliseum] request REQUEST_PLAYING_UPDATE_RESULT, finish_count=%d"
      % finish_count)


# 初始化
def initialize():
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_CREATE, 16,
      ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_coliseum_on_event_playing_create")
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_DESTORY, 16,
      ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_coliseum_on_event_playing_destroy")

  playing_cell = playing_types.Config.get(16)
  if playing_cell is None:
    proxy.Logging.error("[coliseum] playing_types.Config.get(16) failed")
    return False

  if coliseum_types.Config.initialize(playing_cell) is False:
    proxy.Logging.error("[coliseum] initialize config failed")
    return False

  proxy.Logging.debug("[coliseum] initialize")
  return True


# 结束时
def finalize():
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_CREATE, 16,
      ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_coliseum_on_event_playing_create")
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_DESTORY, 16,
      ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_coliseum_on_event_playing_destroy")

  proxy.Logging.debug("[coliseum] finalize")

