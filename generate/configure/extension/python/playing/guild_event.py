
# coding=utf-8

import sys
sys.path.append('ccentity')
sys.path.append('ccevent')
sys.path.append('ccrequest')
sys.path.append('ccvariable')

import time

import ccentity.entity.ttypes
import ccentity.playing.ttypes

import ccevent.ttypes
import ccevent.actor.ttypes
import ccevent.playing.ttypes
import ccevent.role.ttypes
import ccevent.skill.ttypes

import ccrequest.ttypes
import ccrequest.playing.ttypes

import proxy

import guild_types

from thrift import TSerialization


# 副本创建时
def playing_guild_on_event_playing_create(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingCreate()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 获取副本配置
  playing_config = guild_types.Config.get(message.template_)
  if playing_config is None:
    proxy.Logging.error("[guild] guild_types.Config.get(%d) failed" % message.template_)
    return

  # 创建副本对象并加入副本管理器
  playing = guild_types.Playing(message.playing_, message.template_,
      message.scene_, playing_config)
  guild_types.PlayingManager.add(playing)

  # 关注玩家进入/退出副本事件
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_ENTER,
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING,
      "playing_guild_on_event_playing_actor_enter")
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_LEAVE,
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING,
      "playing_guild_on_event_playing_actor_leave")

  # 设置副本开始时间
  playing.set_start_time(time.time())
  # 设置副本超时定时器
  proxy.Timer.add(message.playing_, playing_config.get_time_limit() * 1000, 1,
      "playing_guild_on_timer_expired")

  # 召唤boss
  playing.summon_boss()
  # 副本管理器中建立一个BOSS_NPC_ID和副本ID的映射关系
  guild_types.PlayingManager.add_boss(playing.get_boss_npc_id(), playing.get_id())
  # 关注BOSS属性变化事件
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_ROLE_CHANGE_HP,
      playing.get_boss_npc_id(), ccevent.ttypes.ChannelType.CHANNEL_NPC,
      "playing_guild_on_event_boss_attr_change")
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_ROLE_CHANGE_MAX_HP,
      playing.get_boss_npc_id(), ccevent.ttypes.ChannelType.CHANNEL_NPC,
      "playing_guild_on_event_boss_attr_change")
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_ROLE_CHANGE_MP,
      playing.get_boss_npc_id(), ccevent.ttypes.ChannelType.CHANNEL_NPC,
      "playing_guild_on_event_boss_attr_change")
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_ROLE_CHANGE_MAX_MP,
      playing.get_boss_npc_id(), ccevent.ttypes.ChannelType.CHANNEL_NPC,
      "playing_guild_on_event_boss_attr_change")
  # 关注BOSS脱离战斗事件
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_ROLE_LEAVE_FIGHTING_STATUS,
      playing.get_boss_npc_id(), ccevent.ttypes.ChannelType.CHANNEL_NPC,
      "playing_guild_on_event_role_leave_fighting_status")

  proxy.Logging.debug("[guild] playing(%d,%d,%d) create"
      % (message.playing_, message.template_, message.scene_))


# 副本销毁时
def playing_guild_on_event_playing_destroy(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingDestory()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 移除定时器
  proxy.Timer.remove(message.playing_, "playing_guild_on_timer_expired")

  # 取消关注玩家进入/退出副本
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_ENTER,
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING,
      "playing_guild_on_event_playing_actor_enter")
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_LEAVE,
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING,
      "playing_guild_on_event_playing_actor_leave")

  # 获取副本对象
  playing = guild_types.PlayingManager.get(message.playing_)
  if playing is not None:
    # 取消关注BOSS属性变化事件
    proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_ROLE_CHANGE_HP,
        playing.get_boss_npc_id(), ccevent.ttypes.ChannelType.CHANNEL_NPC,
        "playing_guild_on_event_boss_attr_change")
    proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_ROLE_CHANGE_MAX_HP,
        playing.get_boss_npc_id(), ccevent.ttypes.ChannelType.CHANNEL_NPC,
        "playing_guild_on_event_boss_attr_change")
    proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_ROLE_CHANGE_MP,
        playing.get_boss_npc_id(), ccevent.ttypes.ChannelType.CHANNEL_NPC,
        "playing_guild_on_event_boss_attr_change")
    proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_ROLE_CHANGE_MAX_MP,
        playing.get_boss_npc_id(), ccevent.ttypes.ChannelType.CHANNEL_NPC,
        "playing_guild_on_event_boss_attr_change")
    # 取消关注BOSS脱离战斗事件
    proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_ROLE_LEAVE_FIGHTING_STATUS,
        playing.get_boss_npc_id(), ccevent.ttypes.ChannelType.CHANNEL_NPC,
        "playing_guild_on_event_role_leave_fighting_status")
    # 副本管理器中删除BOSS_NPC_ID和副本ID的映射关系
    guild_types.PlayingManager.remove_boss(playing.get_boss_npc_id())

  # 从副本管理器中删除副本对象
  guild_types.PlayingManager.remove(message.playing_)

  proxy.Logging.debug("[guild] playing(%d,%d) destroy"
      % (message.playing_, message.template_))


# 玩家进入副本时
def playing_guild_on_event_playing_actor_enter(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingActorEnter()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 关注玩家杀死NPC事件、玩家攻击命中事件
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_ACTOR_KILL_NPC,
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR,
      "playing_guild_on_event_actor_kill_npc")
  proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_SKILL_ATTACK_HIT,
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR,
      "playing_guild_on_event_skill_attack_hit")

  # 获取副本对象
  playing = guild_types.PlayingManager.get(message.playing_)
  if playing is None:
    proxy.Logging.error("[guild] PlayingManager.get(%d) failed" % message.playing_)
    return

  # 获取副本玩家对象
  actor = playing.get_actor(message.actor_)
  if actor is None:
    ccactor = proxy.EntityManager.get_actor(message.actor_)
    if ccactor is None:
      proxy.Logging.error("[guild] proxy.EntityManager.get_actor(%d) failed" % message.actor_)
      return
    actor = guild_types.Actor(message.actor_, ccactor.get_name())
    playing.add_actor(actor)

  # 副本管理器中建立一个玩家ID和副本ID的映射关系
  guild_types.PlayingManager.add_actor(message.actor_, message.playing_)
  # 设置玩家在线
  actor.set_leave(False)

  # 请求初始化玩家
  request = ccrequest.playing.ttypes.RequestPlayingInitializeActor()
  request.actor_ = message.actor_
  request.spend_time_ = time.time() - playing.get_start_time()
  request.datas_ = []
  request.scores_ = []
  boss_info = playing.get_boss_info()
  # BOSS当前血量
  request.scores_.append(ccentity.playing.ttypes.PlayingScoreField(
      ccentity.playing.ttypes.PlayingScoreType.POWER, 0, boss_info.current_hp_))
  # BOSS最大血量
  request.scores_.append(ccentity.playing.ttypes.PlayingScoreField(
      ccentity.playing.ttypes.PlayingScoreType.POWER, 1, boss_info.max_hp_))
  # BOSS当前魔法值
  request.scores_.append(ccentity.playing.ttypes.PlayingScoreField(
      ccentity.playing.ttypes.PlayingScoreType.POWER, 2, boss_info.current_mp_))
  # BOSS最大魔法值
  request.scores_.append(ccentity.playing.ttypes.PlayingScoreField(
      ccentity.playing.ttypes.PlayingScoreType.POWER, 3, boss_info.max_mp_))
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_INITIALIZE_ACTOR,
      request)

  # 同步伤害排行
  playing.synchronize_damage_ranking([message.actor_])

  proxy.Logging.debug("[guild] actor(%d) enter into playing" % message.actor_)


# 玩家离开副本时
def playing_guild_on_event_playing_actor_leave(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingActorLeave()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 取消关注事件
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_ACTOR_KILL_NPC,
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR,
      "playing_guild_on_event_actor_kill_npc")
  proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_SKILL_ATTACK_HIT,
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR,
      "playing_guild_on_event_skill_attack_hit")

  # 获取副本对象
  playing = guild_types.PlayingManager.get(message.playing_)
  if playing is None:
    proxy.Logging.error("[guild] PlayingManager.get(%d) failed" % message.playing_)
    return

  # 获取副本玩家对象
  actor = playing.get_actor(message.actor_)
  if actor is None:
    proxy.Logging.error("[guild] playing.get_actor(%d) failed" % message.actor_)
    return

  # 设置玩家离线
  actor.set_leave(True)
  # 副本管理器中删除玩家ID和副本ID的映射关系
  guild_types.PlayingManager.remove_actor(message.actor_)

  proxy.Logging.debug("[guild] actor(%d) leave from playing" % message.actor_)


# 玩家杀死NPC
def playing_guild_on_event_actor_kill_npc(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.actor.ttypes.EventActorKillNpc()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 获取玩家所在副本对象
  playing = guild_types.PlayingManager.get_actor_playing(message.actor_)
  if playing is None:
    proxy.Logging.error("[guild] PlayingManager.get_actor_playing(%d) failed" % message.actor_)
    return

  if playing.get_boss_npc_id() == message.npc_:
    # 结束副本
    playing.finish()


# 玩家攻击命中
def playing_guild_on_event_skill_attack_hit(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.skill.ttypes.EventSkillAttackHit()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  if message.dest_type_ != ccentity.entity.ttypes.EntityType.TYPE_NPC:
    return
  if message.type_ != ccentity.entity.ttypes.EntityType.TYPE_ACTOR:
    return
  if message.hurt_type_ != ccentity.skill.ttypes.SkillHurtType.PHYSICS_ATTACK and \
     message.hurt_type_ != ccentity.skill.ttypes.SkillHurtType.MAGIC_ATTACK and \
     message.hurt_type_ != ccentity.skill.ttypes.SkillHurtType.REDUCE_HP:
    return

  # 获取玩家所在副本对象
  playing = guild_types.PlayingManager.get_actor_playing(message.id_)
  if playing is None:
    proxy.Logging.error("[guild] PlayingManager.get_actor_playing(%d) failed" % message.id_)
    return

  # 获取副本玩家对象
  actor = playing.get_actor(message.id_)
  if actor is None:
    proxy.Logging.error("[guild] playing.get_actor(%d) failed" % message.id_)
    return

  # 增加伤害
  actor.add_damage(message.value_)
  # 同步伤害排行
  playing.broadcast_damage_ranking()


# BOSS属性变化
def playing_guild_on_event_boss_attr_change(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.role.ttypes.EventRoleLeaveFightingStatus()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  if message.type_ != ccentity.entity.ttypes.EntityType.TYPE_NPC:
    return

  # 获取BOSS所在副本对象
  playing = guild_types.PlayingManager.get_boss_playing(message.id_)
  if playing is None:
    proxy.Logging.error("[guild] PlayingManager.get_boss_playing(%d) failed" % message.id_)
    return

  # 同步BOSS信息
  playing.broadcast_boss_info()


# BOSS脱离战斗
def playing_guild_on_event_role_leave_fighting_status(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.role.ttypes.EventRoleLeaveFightingStatus()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  if message.type_ != ccentity.entity.ttypes.EntityType.TYPE_NPC:
    return

  # 获取BOSS所在副本对象
  playing = guild_types.PlayingManager.get_boss_playing(message.id_)
  if playing is None:
    proxy.Logging.error("[guild] PlayingManager.get_boss_playing(%d) failed" % message.id_)
    return

  # 清空伤害
  playing.clear_damage_ranking()
  # 同步伤害排行
  playing.broadcast_damage_ranking()


# 副本超时定时器回调
def playing_guild_on_timer_expired(id):
  # 获取副本对象
  playing = guild_types.PlayingManager.get(id)
  if playing is None:
    proxy.Logging.error("[guild] PlayingManager.get(%d) failed" % id)
    return

  # 副本失败
  request = ccrequest.playing.ttypes.RequestPlayingFailure()
  request.playing_ = id
  proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_FAILURE, request)

  proxy.Logging.debug("[guild] playing(%d) playing_guild_on_timer_expired"
      % playing.get_id())

