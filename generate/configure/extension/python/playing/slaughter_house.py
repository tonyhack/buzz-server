
# coding=utf-8

import sys
sys.path.append('ccentity')
sys.path.append('ccevent')
sys.path.append('ccrequest')
sys.path.append('ccvariable')

import __main__

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

import slaughter_house_types
import playing_types

from thrift import TSerialization


def award_actor(actor, template, result, record):
  proxy.Logging.debug("[slaughter_house] award_actor(%d,%d)" % (actor, template))

  if result.awarded_ == True:
    proxy.Logging.error("[slaughter_house] award_actor(%d,%d) has awarded."\
        % (actor, template))
    return None

  if result.values_ == None:
    proxy.Logging.error("[slaughter_house] result.values_ is none.")
    return None

  playing_config = slaughter_house_types.Config.get(template)
  if playing_config == None:
    proxy.Logging.error("[slaughter_house] slaughter_house_types.Config.get_ranking_award failed.")
    return None

  playing_cell = playing_types.Config.get(template)
  if playing_cell == None:
    proxy.Logging.error("[slaughter_house] playing_types.Config.get(%d) failed." % template)
    return None

  request = ccrequest.playing.ttypes.RequestPlayingAwardActor()
  request.actor_ = actor
  request.playing_template_ = template
  request.awards_ = []

  multiple_awards = playing_config.get_ranking_award_multiple(result.values_[0])
  for i in range(0, multiple_awards):
    for award in playing_cell.awards_:
      request.awards_.append(award)

  if proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_AWARD_ACTOR,\
    request) == False:
    proxy.Logging.error("[slaughter_house] request REQUEST_PLAYING_AWARD_ACTOR failed.")
    return None

  proxy.Logging.debug("[slaughter_house] request REQUEST_PLAYING_AWARD_ACTOR")


def update_record(actor, playing_template, result, record):
  if record.spend_secs_ <= 0:
    # 更新副本记录
    request = ccrequest.playing.ttypes.RequestPlayingUpdateRecord()
    request.actor_ = actor
    request.playing_template_ = playing_template
    request.record_ = ccentity.playing.ttypes.PlayingRecordField(record.period_count_, 1,\
        record.first_pass_awarded_)
    request.record_.values_ = []
    # 发送请求
    proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_UPDATE_RECORD, request)
    proxy.Logging.debug("[slaughter_house] request REQUEST_PLAYING_UPDATE_RECORD")


def initialize():
  summon_pos = [[43,5], [55,6], [62,23], [70,36], [71,49], [60,58], [48,72], [28,71],\
               [20,59], [10,48], [6,37], [17,22]]

  boss_pos = [40, 40]
  award_pos = [40, 40]
  award_pos_range = 38

  for i in range(18, 22):
    playing_cell = playing_types.Config.get(i)
    if playing_cell == None:
      proxy.Logging.error("[slaughter_house] playing_types.Config.get(%d) failed." % i)
      return False

    playing_config = slaughter_house_types.PlayingConfig(i)
    playing_config.playing_time_ = playing_cell.finish_time_
    playing_config.pos_ = summon_pos
    playing_config.boss_pos_ = boss_pos
    playing_config.award_pos_ = award_pos
    playing_config.award_pos_range_ = award_pos_range

    for group in range(0, 8):
      monster_group = slaughter_house_types.MonsterGroupConfig()
      monster_group.boss_id_ = playing_cell.monster_number_order_[group*2][0]
      monster_group.boss_num_ = playing_cell.monster_number_order_[group*2][1]
      # 小boss10分
      playing_config.scores_[monster_group.boss_id_] = 10

      monster_group.award_monster1_id_ = playing_cell.monster_number_order_[group*2+1][0]
      monster_group.award_monster1_num_ = playing_cell.monster_number_order_[group*2+1][1]
      playing_config.scores_[monster_group.award_monster1_id_] = 1

      monster_group.monster1_id_ = playing_cell.monster_number_order_[16][0]
      monster_group.monster1_num_ = playing_cell.monster_number_order_[16][1]
      playing_config.scores_[monster_group.monster1_id_] = 1

      monster_group.monster2_id_ = playing_cell.monster_number_order_[17][0]
      monster_group.monster2_num_ = playing_cell.monster_number_order_[17][1]
      playing_config.scores_[monster_group.monster2_id_] = 1

      monster_group.award_monster2_id_ = playing_cell.monster_number_order_[18][0]
      monster_group.award_monster2_num_ = playing_cell.monster_number_order_[18][1]
      playing_config.monster_groups_.append(monster_group)
      playing_config.scores_[monster_group.award_monster2_id_] = 1

    playing_config.last_boss_id_ = playing_cell.monster_number_order_[19][0]
    # 大boss50分
    playing_config.scores_[playing_config.last_boss_id_] = 50
    # 杀人得1分
    playing_config.kill_actor_score_ = 1
    slaughter_house_types.Config.add(playing_config)

    proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_CREATE, i,\
        ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_slaughter_house_on_event_playing_create")
    proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_DESTORY, i,\
        ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_slaughter_house_on_event_playing_destory")

  proxy.Logging.debug("[slaughter_house] initialize")


def finalize():
  for i in range(18, 22):
    proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_CREATE, i,\
        ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_slaughter_house_on_event_playing_create")
    proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_DESTORY, i,\
        ccevent.ttypes.ChannelType.CHANNEL_PLAYING, "playing_slaughter_house_on_event_playing_destory")

  proxy.Logging.debug("[slaughter_house] finalize")

