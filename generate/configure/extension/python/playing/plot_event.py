
# coding=utf-8

import sys
sys.path.append('ccentity')
sys.path.append('ccevent')
sys.path.append('ccrequest')
sys.path.append('ccvariable')

import __main__

import copy
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
import ccrequest.scene.ttypes

import ccvariable
import ccvariable.ttypes

import variable_table
import proxy
import log
import facade_request

import plot_types

from thrift import TSerialization


# 副本创建时
def playing_plot_on_event_playing_create(message_type, channel, channel_type, serialize):
  # 获取事件交互代理
  communicator_proxy = variable_table.get_variable(ccvariable.ttypes.Variable.COMMUNICATOR_PROXY)

  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingCreate()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 关注玩家进入/退出副本事件
  communicator_proxy.follow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_ENTER, \
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING, \
      "playing_plot_on_event_playing_actor_enter")
  communicator_proxy.follow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_LEAVE, \
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING, \
      "playing_plot_on_event_playing_actor_leave")

  # 创建副本对象并加入管理器中
  playing = plot_types.Playing(message.playing_, message.template_, message.scene_)
  plot_types.PlayingManager.add(playing)

  log.log_debug("副本(id=%d,template=%d) 创建成功" % (message.playing_, message.template_))



# 副本销毁时
def playing_plot_on_event_playing_destory(message_type, channel, channel_type, serialize):
  # 获取事件交互代理
  communicator_proxy = variable_table.get_variable(ccvariable.ttypes.Variable.COMMUNICATOR_PROXY)

  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingDestory()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 取消关注玩家进入/退出副本
  communicator_proxy.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_ENTER, \
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING, \
      "playing_plot_on_event_playing_actor_enter")
  communicator_proxy.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_LEAVE, \
      message.template_, ccevent.ttypes.ChannelType.CHANNEL_PLAYING, \
      "playing_plot_on_event_playing_actor_leave")

  # 从管理器中删除副本对象
  plot_types.PlayingManager.remove(message.playing_)

  log.log_debug("副本(%d) 销毁成功" % message.playing_)



# 玩家进入副本时
def playing_plot_on_event_playing_actor_enter(message_type, channel, channel_type, serialize):
  # 获取事件交互代理
  communicator_proxy = variable_table.get_variable(ccvariable.ttypes.Variable.COMMUNICATOR_PROXY)

  # 获取请求代理
  request_proxy = variable_table.get_variable(ccvariable.ttypes.Variable.REQUEST_PROXY)

  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingActorEnter()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 关注玩家杀死NPC事件、请求完成副本事件、角色被杀死事件
  communicator_proxy.follow(ccevent.ttypes.EventType.EVENT_ACTOR_KILL_NPC, \
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR, \
      "playing_plot_on_event_actor_kill_npc")
  communicator_proxy.follow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_REQUEST_COMPLETE,\
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR,\
      "playing_plot_on_event_playing_actor_request_complete")
  communicator_proxy.follow(ccevent.ttypes.EventType.EVENT_ROLE_KILLED, \
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR, \
      "playing_plot_on_event_actor_killed")
  # 副本1需要关注玩家血量改变的事件
  if message.template_ == 1:
    proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_ROLE_CHANGE_HP, \
        message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR, \
        "playing_plot_on_event_role_change_hp");

  # 获取副本对象
  playing = plot_types.PlayingManager.get(message.playing_)
  if playing == None:
    proxy.Logging.error("plot_types.PlayingManager.get(%d) failed." % message.playing_)
    return None

  now = time.time()

  # 获取玩家对象
  actor = playing.get_actor(message.actor_)
  if actor == None:
    # 玩家不存在时，创建并加入副本中
    actor = plot_types.Actor(message.actor_, now)
    playing.add_actor(actor)
    # 消耗玩家副本次数
    request = ccrequest.playing.ttypes.RequestPlayingIncreaseComplete()
    # request.actor_ = message.actor_
    # request.playing_template_ = message.template_
    request.playing_ = message.playing_
    # 序列化
    request_data = TSerialization.serialize(request)
    # 发送请求
    request_proxy.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_INCREASE_COMPLETE, \
        request_data, len(request_data))

  # 副本管理器中加入一个玩家ID到副本ID的对应关系
  plot_types.PlayingManager.add_actor(message.actor_, message.playing_)

  # 请求初始化玩家
  request = ccrequest.playing.ttypes.RequestPlayingInitializeActor()
  request.actor_ = message.actor_
  request.spend_time_ = now - actor.get_start_time()
  request.scores_ = []
  request.datas_ = []
  # 循环获取玩家杀死的每个NPC
  for (k,v) in actor.get_kill_npcs().items():
    score_field = ccentity.playing.ttypes.PlayingScoreField()
    score_field.type_ = ccentity.playing.ttypes.PlayingScoreType.KILL_NPC
    score_field.key_ = k;
    score_field.value_ = v;
    request.scores_.append(score_field);

  # 序列化消息
  request_data = TSerialization.serialize(request)

  # 发送请求
  request_proxy.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_INITIALIZE_ACTOR, \
      request_data, len(request_data))

  log.log_debug("玩家(%d)进入副本(id=%d,template=%d)" % \
      (message.actor_, message.playing_, message.template_))



# 玩家离开副本时
def playing_plot_on_event_playing_actor_leave(message_type, channel, channel_type, serialize):
  # 获取事件交互代理
  communicator_proxy = variable_table.get_variable(ccvariable.ttypes.Variable.COMMUNICATOR_PROXY)

  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingActorLeave()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 取消关注玩家杀死NPC事件、请求完成副本事件、角色被杀死事件
  communicator_proxy.unfollow(ccevent.ttypes.EventType.EVENT_ROLE_KILLED, \
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR, \
      "playing_plot_on_event_actor_killed")
  communicator_proxy.unfollow(ccevent.ttypes.EventType.EVENT_PLAYING_ACTOR_REQUEST_COMPLETE,\
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR,\
      "playing_plot_on_event_playing_actor_request_complete")
  communicator_proxy.unfollow(ccevent.ttypes.EventType.EVENT_ACTOR_KILL_NPC, \
      message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR, \
      "playing_plot_on_event_actor_kill_npc")
  # 副本1需要关注玩家血量改变的事件
  if message.template_ == 1:
    proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_ROLE_CHANGE_HP, \
        message.actor_, ccevent.ttypes.ChannelType.CHANNEL_ACTOR, \
        "playing_plot_on_event_role_change_hp");

  # 副本管理器中删除玩家对象
  plot_types.PlayingManager.remove_actor(message.actor_)

  log.log_debug("玩家(%d)离开副本(id=%d,template=%d)" % \
      (message.actor_, message.playing_, message.template_))



# 玩家杀死NPC时
def playing_plot_on_event_actor_kill_npc(message_type, channel, channel_type, serialize):
  # 获取请求代理
  request_proxy = variable_table.get_variable(ccvariable.ttypes.Variable.REQUEST_PROXY)

  # 事件消息
  message = ccevent.actor.ttypes.EventActorKillNpc()
  # 反序列化
  TSerialization.deserialize(message, serialize)

  # 获取玩家所在副本ID
  playing_id = plot_types.PlayingManager.get_actor_playing(message.actor_)
  if playing_id == None or playing_id == 0:
    proxy.Logging.error("plot_types.PlayingManager.get_actor_playing(%d) failed." % message.actor_)
    return None

  # 获取副本对象
  playing = plot_types.PlayingManager.get(playing_id)
  if playing == None:
    proxy.Logging.error("plot_types.PlayingManager.get(%d) failed." % playing_id)
    return None

  # 获取副本玩家对象
  actor = playing.get_actor(message.actor_)
  if actor == None:
    proxy.Logging.error("playing.get_actor(%d) failed." % message.actor_)
    return None

  # 增加杀死NPC个数
  actor.inc_kill_npc(message.npc_template_)

  # 请求消息
  request = ccrequest.playing.ttypes.RequestPlayingSynchronizeScore()
  # 玩家ID
  request.actor_ = message.actor_
  # 分数对象(得分类型=杀死NPC, key=NPC模板ID, value=当前杀死的个数)
  score_field = ccentity.playing.ttypes.PlayingScoreField(ccentity.playing.ttypes.PlayingScoreType.KILL_NPC, \
      message.npc_template_, actor.get_kill_npc(message.npc_template_))
  request.score_ = score_field
  # 序列化消息
  request_data = TSerialization.serialize(request)
  # 发送请求
  request_proxy.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_SYNCHRONIZE_SCORE, \
      request_data, len(request_data))

  log.log_debug("玩家(%d) 杀死 NPC(%d) 个数(%d)" % (message.actor_, score_field.key_, \
        score_field.value_))



# 玩家被杀死事件
def playing_plot_on_event_actor_killed(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.role.ttypes.EventRoleKilled()

  # 消息反序列化
  TSerialization.deserialize(message, serialize)

  # 主角类型只能是玩家
  if message.type_ != ccentity.entity.ttypes.EntityType.TYPE_ACTOR:
    return None

  # 根据玩家ID获取所在副本ID
  playing_id = plot_types.PlayingManager.get_actor_playing(message.id_)
  if playing_id == None or playing_id == 0:
    proxy.Logging.error("plot_types.PlayingManager.get_actor_playing(%d) failed." % message.id_)
    return None

  # 获取副本对象
  playing = plot_types.PlayingManager.get(playing_id)
  if playing == None:
    proxy.Logging.error("plot_types.PlayingManager.get(%d) failed." % playing_id)
    return None

  # 获取玩家对象
  actor = playing.get_actor(message.id_)
  if actor == None:
    proxy.Logging.error("playing.get_actor(%d) failed." % message.id_)
    return None

  # 玩家死亡次数增加
  actor.inc_dead_count(1)

  log.log_debug("玩家(%d) 死亡次数(%d)" % (message.id_, actor.get_dead_count()))



# 玩家请求完成副本
def playing_plot_on_event_playing_actor_request_complete(message_type, channel, channel_type, serialize):
  # 获取请求代理
  request_proxy = variable_table.get_variable(ccvariable.ttypes.Variable.REQUEST_PROXY)

  # 事件消息
  message = ccevent.playing.ttypes.EventPlayingActorRequestComplete()

  # 消息反序列化
  TSerialization.deserialize(message, serialize)

  # 获取副本对象
  playing = plot_types.PlayingManager.get(message.playing_)
  if playing == None:
    log.log_error("获取 副本对象(%d) 失败" % message.playing_)
    return None

  # 获取玩家对象
  actor = playing.get_actor(message.actor_)
  if actor == None:
    log.log_error("获取 玩家对象(%d) 失败" % message.actor_)
    return None 
  # 玩家得分(星数) 应该为0
  if actor.get_score() != 0:
    log.log_error("玩家 得分(%d) 错误" % actor.get_score())
    return None

  # 获取副本配置
  playing_config = plot_types.Config.get(message.template_)
  if playing_config == None:
    log.log_error("获取 副本配置失败(%d)" % message.template_)
    return None

  # 获取玩家通关需要杀死的NPC
  pass_kill_npc = playing_config.get_pass_kill_npc()
  if actor.get_kill_npc(pass_kill_npc) <= 0:
    log.log_error("玩家 未能杀死NPC(%d) 不能通关" % pass_kill_npc)
    return None

  # 随机抽取一个道具
  actor.draw_award_item(playing_config.get_draw_items())

  # 统计副本花费时间
  spend_time = time.time() - actor.get_start_time()

  # 请求消息
  request = ccrequest.playing.ttypes.RequestPlayingComplete()

  # 玩家星数加1
  actor.inc_score(1);

  # 死亡次数低于配置次数星数加1
  if actor.get_dead_count() <= playing_config.get_dead_count():
    actor.inc_score(1)

  # 花费时间低于配置时间星数加1
  if spend_time <= playing_config.get_spend_time():
    actor.inc_score(1)

  # 杀死NPC个数大于配置个数星数加1
  if actor.get_kill_npc_num() >= playing_config.get_npc_number():
    actor.inc_score(1)

  # 杀死隐藏NPC星数加1
  if actor.get_kill_npc(playing_config.get_hide_npc()):
    actor.inc_score(1)

  # request.actor_ = message.actor_
  request.playing_ = message.playing_
  request.score_ = actor.get_score()
  request.awards_ = []

  award_field = ccentity.playing.ttypes.PlayingAwardField()
  award_field.type_ = ccentity.resource.ttypes.ResourceType.RESOURCE

  score = actor.get_score()

  # 道具
  for award in playing_config.get_awards():
    if award.type_ == ccentity.resource.ttypes.ResourceType.RESOURCE:
      award.number_ = award.number_ * score * 20 / 100
    request.awards_.append(award)

  # 抽奖奖励
  request.draw_award_ = actor.get_draw_award_item()

  # 副本耗时, 分数
  playing_result = ccentity.playing.ttypes.PlayingResultField()
  playing_result.awarded_ = False
  playing_result.award_count_ = 1
  playing_result.paid_award_count_ = 0;
  playing_result.values_ = []

  # result.value1: 花费时间
  playing_result.values_.append(int(spend_time))
  # result.value2: 分数
  playing_result.values_.append(score)

  log.log_debug("[plot] spend_time=%d" % spend_time)

  # 副本结果
  request.result_ = playing_result

  # 序列化
  request_data = TSerialization.serialize(request)
  # 发送请求
  request_proxy.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_COMPLETE,\
      request_data, len(request_data))

  # 请求清除场景中的NPC
  request_clear_npc = ccrequest.scene.ttypes.RequestSceneClearAllNpc()
  request_clear_npc.scene_ = playing.get_scene()
  request_clear_npc.delay_secs_ = 5

  # 序列化
  request_clear_npc_data = TSerialization.serialize(request_clear_npc)
  # 发送请求
  request_proxy.request(ccrequest.ttypes.RequestType.REQUEST_SCENE_CLEAR_ALL_NPC, \
      request_clear_npc_data, len(request_clear_npc_data))

  log.log_debug("请求清除 场景(%d) npc" % request_clear_npc.scene_)




# 玩家血量改变事件
def playing_plot_on_event_role_change_hp(message_type, channel, channel_type, serialize):
  # 事件消息
  message = ccevent.role.ttypes.EventRoleChangeHp()

  # 消息反序列化
  TSerialization.deserialize(message, serialize)

  # 主角类型只能是玩家
  if message.type_ != ccentity.entity.ttypes.EntityType.TYPE_ACTOR:
    return None

  # 根据玩家ID获取所在副本ID
  playing_id = plot_types.PlayingManager.get_actor_playing(message.id_)
  if playing_id == None or playing_id == 0:
    proxy.Logging.error("plot_types.PlayingManager.get_actor_playing(%d) failed." % message.id_)
    return None

  # 获取副本对象
  playing = plot_types.PlayingManager.get(playing_id)
  if playing == None:
    proxy.Logging.error("plot_types.PlayingManager.get(%d) failed." % playing_id)
    return None

  # 获取玩家对象
  playing_actor = playing.get_actor(message.id_)
  if playing_actor == None:
    proxy.Logging.error("plot_types.PlayingManager.get_actor(%d) failed." % message.id_)
    return None

  # 隐藏NPC是否已召唤
  if playing_actor.get_hide_npc() > 0:
    return None

  # 获得玩家交互对象
  actor = proxy.EntityManager.get_actor(message.id_)
  if actor == None:
    proxy.Logging.error("proxy.EntityManager.get_actor(%d) failed." % message.id_)
    return None

  max_hp = actor.get_attr_role_aoi(ccentity.role.ttypes.RoleAoiFields.MAX_HP)
  current_hp = actor.get_attr_role_aoi(ccentity.role.ttypes.RoleAoiFields.CURRENT_HP)

  if current_hp > (max_hp * 20 / 100):
    return None

  playing_config = plot_types.Config.get(playing.get_template())
  if playing_config == None:
    proxy.Logging.error("获取 副本配置失败(%d)" % playing.get_template())
    return None

  playing_actor.inc_hide_npc(1)

  facade_request.summon_npc(playing.get_scene(), playing_config.get_hide_npc(),\
      actor.get_pos_x(), actor.get_pos_y(), 5, 5)

  proxy.Logging.debug("玩家血量改变，触发隐藏BOSS");

