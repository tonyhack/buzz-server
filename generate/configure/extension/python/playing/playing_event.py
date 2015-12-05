# coding=utf-8

import ccevent
import ccevent.playing.ttypes

import ccvariable
import ccvariable.ttypes

import variable_table
import coliseum
import guild
import idol
import plot
import slaughter_house
import slime
import team
import unreal_soul

from thrift import TSerialization

# 请求增加副本奖励事件时
def playing_on_event_playing_request_add_award(message_type, channel, channel_type, serialize):
  # 事件对象
  message = ccevent.playing.ttypes.EventPlayingRequestAddAward()
  # 反序列化
  TSerialization.deserialize(message, serialize)
  # 如果副本为日常副本时
  if message.template_ >= 1 and message.template_ <= 6:
    plot.add_award(message.actor_, message.template_, message.score_)
  elif message.template_ >= 7 and message.template_ <= 14:
    idol.add_award(message.actor_, message.template_)
  elif message.template_ == 15:
    slime.add_award(message.actor_, message.template_)
  elif message.template_ == 17:
    unreal_soul.add_award(message.actor_, message.template_)
  else:
    None

# 玩家请求领取奖励
def playing_on_event_playing_actor_request_award(message_type, channel, channel_type, serialize):
  # 事件对象
  message = ccevent.playing.ttypes.EventPlayingActorRequestAward()
  # 反序列化
  TSerialization.deserialize(message, serialize)
  # 如果副本为日常副本时
  if message.template_ >= 1 and message.template_ <= 6:
    plot.award_actor(message.actor_, message.template_, message.result_)
  elif message.template_ >= 7 and message.template_ <= 14:
    idol.award_actor(message.actor_, message.template_, message.result_)
  elif message.template_ == 15:
    slime.award_actor(message.actor_, message.template_, message.result_)
  elif message.template_ == 16:
    coliseum.award_actor(message.actor_, message.template_, message.result_)
  elif message.template_ == 17:
    unreal_soul.award_actor(message.actor_, message.template_, message.result_, message.record_)
  elif message.template_ == 18 and message.template_ <= 21:
    slaughter_house.award_actor(message.actor_, message.template_, message.result_, message.record_)
  elif message.template_ >= 100 and message.template_ <= 107:
    team.award_actor(message.actor_, message.template_, message.result_, message.record_)
  elif message.template_ >= 201 and message.template_ <= 224:
    guild.award_actor(message.actor_, message.template_, message.result_, message.record_)
  else:
    None

# 玩家请求更新记录
def playing_on_event_playing_request_update_record(message_type, channel, channel_type, serialize):
  # 事件对象
  message = ccevent.playing.ttypes.EventPlayingRequestUpdateRecord()
  # 反序列化
  TSerialization.deserialize(message, serialize)
  if message.template_ >= 1 and message.template_ <= 6:
    plot.update_record(message.actor_, message.template_,
        message.result_, message.record_)
  elif message.template_ >= 7 and message.template_ <= 14:
    idol.update_record(message.actor_, message.template_,
        message.result_, message.record_)
  elif message.template_ == 15:
    slime.update_record(message.actor_, message.template_,
        message.result_, message.record_)
  elif message.template_ == 16:
    coliseum.update_record(message.actor_, message.template_,
        message.result_, message.record_)
  elif message.template_ == 17:
    unreal_soul.update_record(message.actor_, message.template_,
        message.result_, message.record_)
  elif message.template_ == 18 and message.template_ <= 21:
    slaughter_house.update_record(message.actor_, message.template_,
        message.result_, message.record_)
  elif message.template_ >= 100 and message.template_ <= 107:
    team.update_record(message.actor_, message.template_,
        message.result_, message.record_)
  elif message.template_ >= 201 and message.template_ <= 224:
    guild.update_record(message.actor_, message.template_,
        message.result_, message.record_)
  else:
    None

# 玩家请求完成自动副本
def playing_on_event_playing_request_auto_finish(message_type, channel, channel_type, serialize):
  # 事件对象
  message = ccevent.playing.ttypes.EventPlayingRequestAutoFinish()
  # 反序列化
  TSerialization.deserialize(message, serialize)
  if message.template_ >= 1 and message.template_ <= 6:
    plot.auto_finish(message.actor_, message.template_,
        message.record_, message.result_, message.finish_count_)
  elif message.template_ >= 7 and message.template_ <= 14:
    idol.auto_finish(message.actor_, message.template_,
        message.record_, message.result_, message.finish_count_)
  elif message.template_ == 15:
    slime.auto_finish(message.actor_, message.template_,
        message.record_, message.result_, message.finish_count_)
  elif message.template_ == 16:
    coliseum.auto_finish(message.actor_, message.template_,
        message.record_, message.result_, message.finish_count_)
  elif message.template_ == 17:
    unreal_soul.auto_finish(message.actor, message.template_,
        message.record_, message.result_, message.finish_count_)
  else:
    None

