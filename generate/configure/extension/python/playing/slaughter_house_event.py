
# coding=utf-8

import sys
sys.path.append('ccentity')
sys.path.append('ccevent')
sys.path.append('ccrequest')
sys.path.append('ccvariable')

import __main__

import random
import time

import ccentity
import ccentity.entity
import ccentity.entity.ttypes
import ccentity.role
import ccentity.role.ttypes
import ccentity.playing
import ccentity.playing.ttypes

import ccevent
import ccevent.ttypes
import ccevent.actor.ttypes
import ccevent.playing.ttypes
import ccevent.role.ttypes
import ccevent.skill.ttypes

import ccrequest
import ccrequest.ttypes
import ccrequest.playing.ttypes
import ccrequest.scene.ttypes
import ccrequest.skill.ttypes

import ccvariable
import ccvariable.ttypes

import proxy
import facade_request

import slaughter_house_types
import playing_types

from thrift import TSerialization


wave_interval_sec = 360
boss_interval_sec = 180
"""
wave_interval_sec = 30
boss_interval_sec = 15
"""

# 副本创建时
def playing_slaughter_house_on_event_playing_create(message_type, channel, channel_type, serialize):
  message = ccevent.playing.ttypes.EventPlayingCreate()
  TSerialization.deserialize(message, serialize)

  playing_config = slaughter_house_types.Config.get(message.template_)
  if playing_config == None:
    proxy.Logging.error("[slaughter_house] slaughter_house_types.Config.get(%d) failed."\
        % message.template_)
    return None

  playing = slaughter_house_types.Playing(message.playing_, message.template_, message.scene_)
  slaughter_house_types.PlayingManager.add(playing)

  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_ENTER,\
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING,\
      "playing_slaughter_house_on_event_playing_actor_enter")
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_LEAVE,\
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING,\
      "playing_slaughter_house_on_event_playing_actor_leave")

  # 副本开始后进行30秒倒计时
  proxy.Timer.add(message.playing_, 30 * 1000, 1, "playing_slaughter_house_on_timer_start")
  proxy.Timer.add(message.playing_, 3000, -1, "playing_slaughter_house_on_timer_sync_ranking")

  proxy.Logging.debug("[slaughter_house] playing(%d,%d,%d) create"\
      % (message.playing_, message.template_, message.scene_))


# 副本结束
def playing_slaughter_house_on_event_playing_destory(message_type, channel, channel_type, serialize):
  message = ccevent.playing.ttypes.EventPlayingDestory()
  TSerialization.deserialize(message, serialize)

  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_ENTER,\
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING,\
      "playing_slaughter_house_on_event_playing_actor_enter")
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_LEAVE,\
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING,\
      "playing_slaughter_house_on_event_playing_actor_leave")

  playing = slaughter_house_types.PlayingManager.get(message.playing_)
  if playing != None:
    proxy.Timer.remove(message.playing_, "playing_slaughter_house_on_timer_start")
    proxy.Timer.remove(message.playing_, "playing_slaughter_house_on_timer_sync_ranking")

  slaughter_house_types.PlayingManager.remove(message.playing_)

  proxy.Logging.debug("[slaughter_house] playing(%d,%d) destory."\
      % (message.playing_, message.template_))


#e玩家进入副本
def playing_slaughter_house_on_event_playing_actor_enter(message_type, channel, channel_type, serialize):
  message = ccevent.playing.ttypes.EventPlayingActorEnter()
  TSerialization.deserialize(message, serialize)

  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_ROLE_KILLED,\
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR,\
      "playing_slaughter_house_on_event_actor_killed")
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_ACTOR_KILL_NPC,\
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR,\
      "playing_slaughter_house_on_event_actor_kill_npc")

  ccactor = proxy.EntityManager.get_actor(message.actor_)
  if ccactor == None:
    return None

  playing = slaughter_house_types.PlayingManager.get(message.playing_)
  if playing == None:
    proxy.Logging.error("[slaughter_house] slaughter_house_types.PlayingManager.get(%d) failed."\
        % message.playing_)
    return None

  actor = playing.get_actor(message.actor_)
  if actor == None:
    actor = slaughter_house_types.Actor(message.actor_, ccactor.get_name())
    playing.add_actor(actor)
    playing.ranking_.add_score(message.actor_, 0)
    sync_ranking_actor(message.playing_, message.actor_)

  actor.leaving_ = False;

  slaughter_house_types.PlayingManager.add_actor(message.actor_, message.playing_)

  now = time.time()

  # 请求初始化玩家
  request = ccrequest.playing.ttypes.RequestPlayingInitializeActor()
  request.actor_ = message.actor_
  request.spend_time_ = now - playing.get_start_time()
  request.datas_ = []
  request.scores_ = []
  request.scores_.append(ccentity.playing.ttypes.PlayingScoreField(
        ccentity.playing.ttypes.PlayingScoreType.STAGE, 0, playing.group_))
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_INITIALIZE_ACTOR,
    request)

  proxy.Logging.debug("[slaughter_house] actor(%d) enter into playing" % message.actor_)


# 玩家离开副本时
def playing_slaughter_house_on_event_playing_actor_leave(message_type, channel, channel_type, serialize):
  message = ccevent.playing.ttypes.EventPlayingActorLeave()
  TSerialization.deserialize(message, serialize)

  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_ROLE_KILLED,\
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR,\
      "playing_slaughter_house_on_event_actor_killed")
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_ACTOR_KILL_NPC,\
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR,\
      "playing_slaughter_house_on_event_actor_kill_npc")

  playing = slaughter_house_types.PlayingManager.get(message.playing_)
  if playing == None:
    return None

  actor = playing.get_actor(message.actor_)
  if actor == None:
    proxy.Logging.error("[slaughter_house] playing.get_actor(%d) failed." % message.actor_)
    return None

  actor.leaving_ = True;

  slaughter_house_types.PlayingManager.remove_actor(message.actor_)

  proxy.Logging.debug("[slaughter_house] actor(%d) leave from playing" % message.actor_)


# 玩家被杀死
def playing_slaughter_house_on_event_actor_killed(message_type, channel, channel_type, serialize):
  message = ccevent.role.ttypes.EventRoleKilled()
  TSerialization.deserialize(message, serialize)

  if message.attacker_type_ != ccentity.entity.ttypes.EntityType.TYPE_ACTOR:
    return None;

  playing_id = slaughter_house_types.PlayingManager.get_actor_playing(message.id_)
  if playing_id == None or playing_id == 0:
    proxy.Logging.error("[slaughter_house] PlayingManager.get_actor_playing(%d) failed."\
        % message.id_)
    return None

  playing = slaughter_house_types.PlayingManager.get(playing_id)
  if playing == None:
    proxy.Logging.error("[slaughter_house] PlayingManager.get(%d) failed." % playing_id)
    return None

  playing_config = slaughter_house_types.Config.get(playing.get_template())
  if playing_config == None:
    proxy.Logging.error("[slaughter_house] slaughter_house_types.Config.get(%d) failed."\
        % playing.get_template())
    return None

  attacker_actor = playing.get_actor(message.attacker_id_)
  if attacker_actor == None:
    proxy.Logging.error("[slaughter_house] playing.get_actor(%d) failed."\
        % message.attacker_id_)
    return None

  attacker_actor.score_ += playing_config.kill_actor_score_
  playing.ranking_.add_score(message.attacker_id_, playing_config.kill_actor_score_)

  proxy.Logging.debug("[slaughter_house] actor(%d) killed by actor(%d)"\
      % (message.id_, message.attacker_id_))


# 玩家杀死NPC
def playing_slaughter_house_on_event_actor_kill_npc(message_type, channel, channel_type, serialize):
  message = ccevent.actor.ttypes.EventActorKillNpc()
  TSerialization.deserialize(message, serialize)

  playing_id = slaughter_house_types.PlayingManager.get_actor_playing(message.actor_)
  if playing_id == None or playing_id == 0:
    proxy.Logging.error("[slaughter_house] PlayingManager.get_actor_playing(%d) failed."\
        % (message.actor_))
    return None

  playing = slaughter_house_types.PlayingManager.get(playing_id)
  if playing == None:
    proxy.Logging.error("[slaughter_house] PlayingManager.get(%d) failed."\
        % (message.actor_))
    return None

  playing_config = slaughter_house_types.Config.get(playing.get_template())
  if playing_config == None:
    proxy.Logging.error("[slaughter_house] slaughter_house_types.Config.get(%d) failed."\
        % (playing.get_template()))
    return None

  playing_cell = playing_types.Config.get(playing.get_template())
  if playing_cell == None:
    proxy.Logging.error("[slaughter_house] playing_types.Config.get(%d) failed."\
        % (playing.get_template()))
    return None

  actor = playing.get_actor(message.actor_)
  if actor == None:
    proxy.Logging.error("[slaughter_house] playing.get_actor(%d) failed."\
        % (message.actor_))
    return None

  actor.score_ += playing_config.get_monster_score(message.npc_template_)
  playing.ranking_.add_score(message.actor_,\
      playing_config.get_monster_score(message.npc_template_))

  if playing.group_ - 1 < len(playing_config.monster_groups_):
    monster_group = playing_config.monster_groups_[playing.group_ - 1]
    if monster_group == None:
      proxy.Logging.error("[slaughter_house] playing_config.monster_groups_[%d] is none."\
          % (playing.group_ - 1))
      return None
 
    if monster_group.boss_id_ == message.npc_template_:
      summon_award_group(playing_id)

  proxy.Logging.debug("[slaughter_house] actor(%d) kill npc(%d) score(%d)"\
      % (message.actor_, message.npc_template_, playing_config.get_monster_score(message.npc_template_)))


# 开始倒计时结束
def playing_slaughter_house_on_timer_start(id):
  summon_next_group(id)
  proxy.Timer.add(id, wave_interval_sec * 1000, -1, "playing_slaughter_house_on_timer_next_group")


# 每波怪的BOSS时间
def playing_slaughter_house_on_timer_boss(id):
  playing = slaughter_house_types.PlayingManager.get(id)
  if playing == None:
    proxy.Logging.error("[slaughter_house] PlayingManager.get(%d) failed." % id)
    return None

  facade_request.clear_scene_npcs(playing.get_scene(), 0, True)

  playing_config = slaughter_house_types.Config.get(playing.get_template())
  if playing_config == None:
    proxy.Logging.error("[slaughter_house] slaughter_house_types.Config.get(%d) failed."\
        % playing.get_template())
    return None

  playing.boss_time_ = True

  monster_group = playing_config.monster_groups_[playing.group_ - 1]
  if monster_group == None:
    proxy.Logging.error("[slaughter_house] playing_config.monster_groups_[%d] is none."\
        % (playing.group_ - 1))
    return None

  rand_int = random.randint(0, len(playing_config.pos_) - 1)
  pos = playing_config.pos_[rand_int]

  facade_request.summon_npcs(playing.get_scene(), monster_group.boss_id_, monster_group.boss_num_,\
      playing_config.boss_pos_[0], playing_config.boss_pos_[1])


# 下一波怪
def playing_slaughter_house_on_timer_next_group(id):
  summon_next_group(id)


def summon_next_group(playing_id):
  playing = slaughter_house_types.PlayingManager.get(playing_id)
  if playing == None:
    proxy.Logging.error("[slaughter_house] PlayingManager.get(%d) failed." % playing_id)
    return None

  playing.boss_time_ = False

  facade_request.clear_scene_npcs(playing.get_scene(), 0, True)

  playing_config = slaughter_house_types.Config.get(playing.get_template())
  if playing_config == None:
    proxy.Logging.error("[slaughter_house] slaughter_house_types.Config.get(%d) failed."\
        % playing.get_template())
    return None

  playing.group_ += 1

  # 同步阶段给所有玩家
  request = ccrequest.playing.ttypes.RequestPlayingSynchronizeScore()
  request.score_ = ccentity.playing.ttypes.PlayingScoreField(
      ccentity.playing.ttypes.PlayingScoreType.STAGE, 0, playing.group_)
  for actor_id in playing.get_actors():
    request.actor_ = actor_id
    proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_SYNCHRONIZE_SCORE, request)

  proxy.Timer.remove(playing_id, "playing_slaughter_house_on_timer_boss")

  if playing.group_ == 8:
    # 最终BOSS
    rand_int = random.randint(0, len(playing_config.pos_) - 1)
    pos = playing_config.pos_[rand_int]
    facade_request.summon_npc(playing.get_scene(), playing_config.last_boss_id_,\
        playing_config.boss_pos_[0], playing_config.boss_pos_[1])
  elif playing.group_ > 8:
    end_playing(playing_id)
    return None
  else:
    monster_group = playing_config.monster_groups_[playing.group_ - 1]
    if monster_group == None:
      proxy.Logging.error("[slaughter_house] playing_config.monster_groups_[%d] is none."\
          % (playing.group_ - 1))
      return None
 
    # 招出各种普通怪
    for pos in playing_config.pos_:
      facade_request.summon_npcs(playing.get_scene(), monster_group.monster1_id_,\
          monster_group.monster1_num_, pos[0], pos[1], 10, 10)
      facade_request.summon_npcs(playing.get_scene(), monster_group.monster2_id_,\
          monster_group.monster2_num_, pos[0], pos[1], 10, 10)

    proxy.Timer.add(playing_id, boss_interval_sec * 1000, 1, "playing_slaughter_house_on_timer_boss")


def summon_award_group(playing_id):
  playing = slaughter_house_types.PlayingManager.get(playing_id)
  if playing == None:
    proxy.Logging.error("[slaughter_house] PlayingManager.get(%d) failed." % playing_id)
    return None

  if playing.boss_time_ == False:
    return None

  playing_config = slaughter_house_types.Config.get(playing.get_template())
  if playing_config == None:
    proxy.Logging.error("[slaughter_house] slaughter_house_types.Config.get(%d) failed."\
        % playing.get_template())
    return None

  monster_group = playing_config.monster_groups_[playing.group_ - 1]
  if monster_group == None:
    proxy.Logging.error("[slaughter_house] playing_config.monster_groups_[%d] is none."\
        % (playing.group_ - 1))
    return None

  facade_request.summon_npcs(playing.get_scene(), monster_group.award_monster1_id_,\
      monster_group.award_monster1_num_, playing_config.award_pos_[0],\
      playing_config.award_pos_[1], playing_config.award_pos_range_, playing_config.award_pos_range_)
  facade_request.summon_npcs(playing.get_scene(), monster_group.award_monster2_id_,\
      monster_group.award_monster2_num_, playing_config.award_pos_[0],\
      playing_config.award_pos_[1], playing_config.award_pos_range_, playing_config.award_pos_range_)


def end_playing(playing_id):
  playing = slaughter_house_types.PlayingManager.get(playing_id)
  if playing == None:
    proxy.Logging.error("[slaughter_house] PlayingManager.get(%d) failed." % playing_id)
    return None

  facade_request.clear_scene_npcs(playing.get_scene(), 0, True)

  proxy.Timer.remove(playing_id, "playing_slaughter_house_on_timer_next_group")

  ranking = 1

  for actor_rank in playing.ranking_.get_ranking():
    actor = playing.get_actor(actor_rank.id_)
    if actor != None and actor.leaving_ == False:
      request = ccrequest.playing.ttypes.RequestPlayingActorComplete()
      request.playing_ = playing_id
      request.draw_award_ = ccentity.playing.ttypes.PlayingAwardField(\
          ccentity.resource.ttypes.ResourceType.MIN, 0, 0)
      request.actor_ = actor_rank.id_
      result = ccentity.playing.ttypes.PlayingResultField()
      result.award_count_ = 1
      result.paid_award_count_ = 0;
      result.awarded_ = False
      # TODO: values_[0]中放入玩家名次
      result.values_ = []
      result.values_.append(ranking);
      ranking += 1
      request.awards_ = []
      request.result_ = result
      proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_ACTOR_COMPLETE,\
          request)


  # 副本完成
  request = ccrequest.playing.ttypes.RequestPlayingComplete()
  request.playing_ = playing_id
  result = ccentity.playing.ttypes.PlayingResultField()
  result.values_ = []
  result.award_count_ = 1
  result.paid_award_count_ = 0;
  result.awarded_ = False
  request.result_ = result
  request.awards_ = []
  request.draw_award_ = ccentity.playing.ttypes.PlayingAwardField(\
      ccentity.resource.ttypes.ResourceType.MIN, 0, 0)
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_COMPLETE,\
      request)

  proxy.Logging.debug("[slaughter_house] end playing(%d)." % playing_id)


def playing_slaughter_house_on_timer_sync_ranking(id):
  sync_ranking(id)


def sync_ranking(playing_id):
  playing = slaughter_house_types.PlayingManager.get(playing_id)
  if playing == None:
    proxy.Logging.error("[slaughter_house] PlayingManager.get(%d) failed." % playing_id)
    return None

  request = ccrequest.playing.ttypes.RequestPlayingSynchronizeRanking()
  request.actors_ = []
  request.ranking_ = []

  for actor_rank in playing.ranking_.get_ranking():
    actor = playing.get_actor(actor_rank.id_)
    if actor != None:
      field = ccentity.playing.ttypes.PlayingRankingField()
      field.name_ = actor.get_name()
      field.value_ = actor_rank.score_
      request.ranking_.append(field)
      request.actors_.append(actor_rank.id_)

  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_SYNCHRONIZE_RANKING, request)


def sync_ranking_actor(playing_id, actor_id):
  playing = slaughter_house_types.PlayingManager.get(playing_id)
  if playing == None:
    proxy.Logging.error("[slaughter_house] PlayingManager.get(%d) failed." % playing_id)
    return None

  request = ccrequest.playing.ttypes.RequestPlayingSynchronizeRanking()
  request.actors_ = []
  request.ranking_ = []
  request.actors_.append(actor_id)

  for actor_rank in playing.ranking_.get_ranking():
    actor = playing.get_actor(actor_rank.id_)
    if actor != None:
      field = ccentity.playing.ttypes.PlayingRankingField()
      field.name_ = actor.get_name()
      field.value_ = actor_rank.score_
      request.ranking_.append(field)

  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_SYNCHRONIZE_RANKING, request)

