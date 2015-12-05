
# coding=utf-8

import sys
sys.path.append('ccentity')
sys.path.append('ccevent')
sys.path.append('ccrequest')
sys.path.append('ccvariable')

import __main__

import time

import ccentity.playing.ttypes

import ccevent.ttypes
import ccevent.actor.ttypes
import ccevent.playing.ttypes
import ccevent.role.ttypes

import ccrequest.ttypes
import ccrequest.playing.ttypes

import proxy
import facade_request
import unreal_soul_types

from thrift import TSerialization


def playing_unreal_soul_on_event_playing_create(message_type, channel, channel_type, serialize):
  message = ccevent.playing.ttypes.EventPlayingCreate()
  TSerialization.deserialize(message, serialize)

  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_ENTER,
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING,
      "playing_unreal_soul_on_event_playing_actor_enter")
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_LEAVE,
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING,
      "playing_unreal_soul_on_event_playing_actor_leave")
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_ROLE_ENTER_FIGHTING_STATUS,
      0, ccevent.ttypes.ChannelType.CHANNEL_NPC,
      "playing_unreal_soul_on_event_role_enter_fighting_status")
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_ROLE_LEAVE_FIGHTING_STATUS,
      0, ccevent.ttypes.ChannelType.CHANNEL_NPC,
      "playing_unreal_soul_on_event_role_leave_fighting_status")

  playing = unreal_soul_types.Playing()
  playing.initialize(message.playing_, message.template_, message.scene_)

  unreal_soul_types.PlayingManager.add(playing)

  now = time.time()
  playing.start_time_ = now

  proxy.Timer.add(message.playing_, unreal_soul_types.Config.PLAYING_MAX_TIME_ * 1000, 1,
    "playing_unreal_soul_on_timer_playing_timeout")
  # 设置石像变换的定时器
  proxy.Timer.add(message.playing_, unreal_soul_types.Config.UNREAL_TRANSFER_INTERVAL * 1000
      , -1, "playing_unreal_soul_on_timer_transfer")
  # 设置战斗状态的怪物二级属性增强
  proxy.Timer.add(message.playing_, 60 * 1000, -1, "playing_unreal_soul_on_time_monster_strengthen")

  playing.goto_next_unreal()

  proxy.Logging.debug("[unreal_soul] playing(%d,%d,%d) create"
      % (message.playing_, message.template_, message.scene_))


def playing_unreal_soul_on_event_playing_destory(message_type, channel, channel_type, serialize):
  message = ccevent.playing.ttypes.EventPlayingDestory()
  TSerialization.deserialize(message, serialize)

  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_ENTER,
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING,
      "playing_unreal_soul_on_event_playing_actor_enter")
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_LEAVE,
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING,
      "playing_unreal_soul_on_event_playing_actor_leave")
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_ROLE_ENTER_FIGHTING_STATUS,
      0, ccevent.ttypes.ChannelType.CHANNEL_NPC,
      "playing_unreal_soul_on_event_role_enter_fighting_status")
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_ROLE_LEAVE_FIGHTING_STATUS,
      0, ccevent.ttypes.ChannelType.CHANNEL_NPC,
      "playing_unreal_soul_on_event_role_leave_fighting_status")

  playing = unreal_soul_types.PlayingManager.get(message.playing_)
  if playing == None:
    proxy.Logging.error("[unreal_soul] PlayingManager.get(%d)" % message.playing_)
    return None

  playing.finalize()

  unreal_soul_types.PlayingManager.remove(message.playing_)

  proxy.Timer.remove(message.playing_, "playing_unreal_soul_on_timer_playing_timeout")
  # 石像变换的定时器
  proxy.Timer.remove(message.playing_, "playing_unreal_soul_on_timer_transfer")
  # 战斗状态的怪物二级属性增强
  proxy.Timer.remove(message.playing_, "playing_unreal_soul_on_time_monster_strengthen")

  proxy.Logging.debug("[unreal_soul] playing(%d,%d) destroy"
      % (message.playing_, message.template_))


def playing_unreal_soul_on_event_playing_actor_enter(message_type, channel, channel_type, serialize):
  message = ccevent.playing.ttypes.EventPlayingActorEnter()
  TSerialization.deserialize(message, serialize)

  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_ACTOR_KILL_NPC,
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR,
      "playing_unreal_soul_on_event_actor_kill_npc")

  playing = unreal_soul_types.PlayingManager.get(message.playing_)
  if playing == None:
    proxy.Logging.error("[unreal_soul] PlayingManager.get(%d) failed." % message.playing_)
    return None

  actor = playing.get_actor(message.actor_)
  if actor == None:
    actor = unreal_soul_types.Actor(message.actor_)
    playing.add_actor(actor)
    increase_actor_playing_complete(message.actor_, playing.template_)

  actor.set_leave(False)

  unreal_soul_types.PlayingManager.add_actor(message.actor_, message.playing_)

  now = time.time()

  # 初始化玩家
  request = ccrequest.playing.ttypes.RequestPlayingInitializeActor()
  request.actor_ = message.actor_
  request.spend_time_ = now - playing.start_time_
  request.datas_ = []
  request.scores_ = []
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_INITIALIZE_ACTOR,
      request)


def playing_unreal_soul_on_event_playing_actor_leave(message_type, channel, channel_type, serialize):
  message = ccevent.playing.ttypes.EventPlayingActorLeave()
  TSerialization.deserialize(message, serialize)

  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_ACTOR_KILL_NPC,
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR,
      "playing_unreal_soul_on_event_actor_kill_npc")

  unreal_soul_types.PlayingManager.remove_actor(message.actor_)

  playing = unreal_soul_types.PlayingManager.get(message.playing_)
  if playing == None:
    proxy.Logging.error("[unreal_soul] PlayingManager.get(%d) failed." % message.playing_)
    return None

  actor = playing.get_actor(message.actor_)
  if actor == None:
    proxy.Logging.error("[unreal_soul] Playing.get_actor(%d) failed." % message.actor_)
    return None;

  actor.set_leave(True)

  proxy.Logging.debug("[unreal_soul] actor(%d) leave from playing" % message.actor_)


def playing_unreal_soul_on_event_actor_kill_npc(message_type, channel, channel_type, serialize):
  message = ccevent.actor.ttypes.EventActorKillNpc()
  TSerialization.deserialize(message, serialize)

  playing_id = unreal_soul_types.PlayingManager.get_actor_playing(message.actor_)
  if playing_id == 0:
    proxy.Logging.error("[unreal_soul] PlayingManager.get_actor_playing(%d) failed."
        % message.actor_)
    return None

  playing = unreal_soul_types.PlayingManager.get(playing_id)
  if playing == None:
    proxy.Logging.error("[unreal_soul] Playing.get(%d) failed." % playing_id)
    return None

  playing.kill_npc(message.npc_template_)

  if message.npc_template_ == unreal_soul_types.Config.BOSS_.template_:
    request = ccrequest.playing.ttypes.RequestPlayingComplete()
    request.playing_ = playing.id_
    result = ccentity.playing.ttypes.PlayingResultField()
    result.values_ = []
    result.award_count_ = 1
    result.paid_award_count_ = 0
    result.awarded_ = False
    request.result_ = result
    request.awards_ = []
    request.awards_.extend(unreal_soul_types.Config.AWARDS_)
    playing.set_draw_award(unreal_soul_types.Config.get_rand_draw())
    request.draw_award_ = playing.get_draw_award()
    proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_COMPLETE, request)
  else:
    playing.remove_monster(message.npc_)
    if playing.get_monster_number(message.npc_template_) <= 0:
      proxy.Timer.remove(playing.id_, "playing_unreal_soul_on_timer_transfer")
      if playing.check_current_unreal_empty() == True:
        if playing.goto_next_unreal() == False:
          playing.summon_boss()
        else:
          proxy.Timer.add(playing.id_, unreal_soul_types.Config.UNREAL_TRANSFER_INTERVAL * 1000
              , -1, "playing_unreal_soul_on_timer_transfer")

  unreal_soul_types.PlayingManager.remove_monster(message.npc_)

  proxy.Logging.debug("[unreal_soul] actor(%d) kill npc" % message.actor_)


def playing_unreal_soul_on_event_role_enter_fighting_status(message_type, channel,
    channel_type, serialize):
  message = ccevent.role.ttypes.EventRoleEnterFightingStatus()
  TSerialization.deserialize(message, serialize)

  proxy.Logging.debug("[unreal_soul] EventRoleEnterFightingStatus")

  if message.type_ != ccentity.entity.ttypes.EntityType.TYPE_NPC:
    return None

  playing_id = unreal_soul_types.PlayingManager.get_monster_playing(message.id_)
  if playing_id == 0:
    proxy.Logging.error("[unreal_soul] PlayingManager.get_monster_playing(%d) failed."
        % message.id_)
    return None

  playing = unreal_soul_types.PlayingManager.get(playing_id)
  if playing == None:
    proxy.Logging.error("[unreal_soul] PlayingManager.get(%d) failed." % playing_id)
    return None

  monster = playing.get_monster(message.id_)
  if monster == None:
    proxy.Logging.error("[unreal_soul] Playing.get_monster(%d) failed." % message.id_)
    return None

  monster.fighting_status_ = True

  proxy.Logging.debug("[unreal_soul] monster(%d) in fighting status" % message.id_)


def playing_unreal_soul_on_event_role_leave_fighting_status(message_type, channel,
    channel_type, serialize):
  message = ccevent.role.ttypes.EventRoleLeaveFightingStatus()
  TSerialization.deserialize(message, serialize)

  proxy.Logging.debug("[unreal_soul] EventRoleLeaveFightingStatus")

  if message.type_ != ccentity.entity.ttypes.EntityType.TYPE_NPC:
    return None

  playing_id = unreal_soul_types.PlayingManager.get_monster_playing(message.id_)
  if playing_id == 0:
    proxy.Logging.error("[unreal_soul] PlayingManager.get_monster_playing(%d) failed."
        % message.id_)
    return None

  playing = unreal_soul_types.PlayingManager.get(playing_id)
  if playing == None:
    proxy.Logging.error("[unreal_soul] PlayingManager.get(%d) failed." % playing_id)
    return None

  monster = playing.get_monster(message.id_)
  if monster == None:
    proxy.Logging.error("[unreal_soul] Playing.get_monster(%d) failed." % message.id_)
    return None

  monster.fighting_status_ = False

  proxy.Logging.debug("[unreal_soul] monster(%d) out of fighting status" % message.id_)


def playing_unreal_soul_on_timer_playing_timeout(playing_id):
  playing = unreal_soul_types.PlayingManager.get(playing_id)
  if playing == None:
    proxy.Logging.error("[unreal_soul] PlayingManager.get(%d) failed" % playing_id)
    return None

  request = ccrequest.playing.ttypes.RequestPlayingFailure()
  request.playing_ = playing_id
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_FAILURE, request)

  proxy.Logging.debug("[unreal_soul] playing_unreal_soul_on_timer_playing_timeout expired")


def playing_unreal_soul_on_timer_transfer(playing_id):
  playing = unreal_soul_types.PlayingManager.get(playing_id)
  if playing == None:
    proxy.Logging.error("[unreal_soul] PlayingManager.get(%d) failed." % playing_id)
    return None

  if playing.goto_next_unreal() == False:
    playing.summon_boss()

  proxy.Logging.debug("[unreal_soul] playing_unreal_soul_on_timer_transfer")


def playing_unreal_soul_on_time_monster_strengthen(playing_id):
  playing = unreal_soul_types.PlayingManager.get(playing_id)
  if playing == None:
    proxy.Logging.error("[unreal_soul] PlayingManager.get(%d) failed." % playing_id)
    return None

  playing.strengthen_monster()

  proxy.Logging.debug("[unreal_soul] playing_unreal_soul_on_time_monster_strengthen")


def increase_actor_playing_complete(actor_id, playing_template):
  request = ccrequest.playing.ttypes.RequestPlayingActorIncreaseComplete()
  request.actor_ = actor_id
  request.playing_template_ = playing_template
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_ACTOR_INCREASE_COMPLETE,
      request)

