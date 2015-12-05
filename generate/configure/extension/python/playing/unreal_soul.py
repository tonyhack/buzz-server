
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

import unreal_soul_types
import playing_types

from thrift import TSerialization


def award_actor(actor, template, result, record):
  proxy.Logging.debug("[unreal_soul] award_actor(%d,%d)" % (actor, template))

  if result.awarded_ == True:
    proxy.Logging.error("[unreal_soul] award_actor(%d,%d) has awarded."\
        % (actor, template))
    return None

  request = ccrequest.playing.ttypes.RequestPlayingAwardActor()
  request.actor_ = actor
  request.playing_template_ = template
  request.first_pass_award_ = True
  request.awards_ = []

  for award in unreal_soul_types.Config.AWARDS_:
    request.awards_.append(award)

  if proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_AWARD_ACTOR,\
    request) == False:
    proxy.Logging.error("[unreal_soul] request REQUEST_PLAYING_AWARD_ACTOR failed.")
    return None

  proxy.Logging.debug("[unreal_soul] request REQUEST_PLAYING_AWARD_ACTOR")


def update_record(actor, playing_template, result, record):
  if record.spend_secs_ <= 0:
    request = ccrequest.playing.ttypes.RequestPlayingUpdateRecord()
    request.actor_ = actor
    request.playing_template_ = playing_template
    request.record_ = ccentity.playing.ttypes.PlayingRecordField(record.period_count_, 1,\
        record.first_pass_awarded_)
    request.record_.values_ = []
    proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_UPDATE_RECORD, request)
    proxy.Logging.debug("[unreal_soul] REQUEST_PLAYING_UPDATE_RECORD")


def auto_finish(actor, playing_template, record, result, finish_count):
  if finish_count <= 0:
    return None

  request = ccrequest.playing.ttypes.RequestPlayingUpdateRecord()
  request.actor_ = actor
  request.playing_template_ = playing_template
  request.record_ = record
  request.record_.period_count_ += finish_count
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_UPDATE_RECORD, request)

  request = ccrequest.playing.ttypes.RequestPlayingUpdateResult()
  request.actor_ = actor
  request.playing_template_ = playing_template
  request.result_ = ccentity.playing.ttypes.PlayingResultField(\
      False, finish_count, result.paid_award_count_, [])
  # 发送请求
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_UPDATE_RESULT, request)


def initialize():
  playing_cell = playing_types.Config.get(17)
  if playing_cell == None:
    proxy.Logging.error("[unreal_soul] playing_types.Config.get(17) failed.")
    return False

  if unreal_soul_types.Config.initialize(playing_cell) == False:
    proxy.Logging.error("[unreal_soul] Config.initialize failed")
    return False

  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_CREATE, 17,
      ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_unreal_soul_on_event_playing_create")
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_DESTORY, 17,
      ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_unreal_soul_on_event_playing_destory")

  proxy.Logging.debug("[unreal_soul] initialize")

  return True


def finalize():
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_CREATE, 17,
      ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_unreal_soul_on_event_playing_create")
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_DESTORY, 17,
      ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_unreal_soul_on_event_playing_destroy")

  proxy.Logging.debug("[unreal_soul] finalize")

