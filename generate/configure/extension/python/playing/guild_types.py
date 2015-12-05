
# coding=utf-8

import ccentity.playing.ttypes
import ccentity.resource.ttypes
import ccentity.role.ttypes

import ccrequest.ttypes
import ccrequest.playing.ttypes

import proxy
import facade_request

import playing_types

from operator import attrgetter


# 副本玩家
class Actor:
  def __init__(self, id, name):
    self.id_ = id
    self.name_ = name
    self.damage_ = 0
    self.leave_ = False

  def get_id(self):
    return self.id_

  def get_name(self):
    return self.name_

  def get_damage(self):
    return self.damage_

  def add_damage(self, damage):
    self.damage_ += damage

  def clear_damage(self):
    self.damage_ = 0

  def get_leave(self):
    return self.leave_

  def set_leave(self, leave):
    self.leave_ = leave


# BOSS信息
class BossInfo:
  def __init__(self):
    self.current_hp_ = 0
    self.max_hp_ = 0
    self.current_mp_ = 0
    self.max_mp_ = 0


# 副本对象
class Playing:
  def __init__(self, id, template, scene, config):
    self.id_ = id
    self.template_ = template
    self.scene_ = scene
    self.config_ = config
    self.start_time_ = 0
    self.actors_ = {}
    self.boss_npc_id_ = 0

  def get_id(self):
    return self.id_

  def get_template(self):
    return self.template_

  def get_scene(self):
    return self.scene_

  def get_start_time(self):
    return self.start_time_

  def set_start_time(self, time):
    self.start_time_ = time

  def get_actor(self, id):
    if id in self.actors_:
      return self.actors_[id]
    else:
      return None

  def add_actor(self, actor):
    self.actors_[actor.get_id()] = actor

  def clear_damage_ranking(self):
    for i in self.actors_.keys():
      self.actors_[i].clear_damage()

  def summon_boss(self):
    if self.boss_npc_id_ != 0:
      return

    cell = self.config_.get_boss()
    self.boss_npc_id_ = facade_request.summon_npc(self.scene_,
        cell.template_id_, cell.x_, cell.y_)

  def get_boss_npc_id(self):
    return self.boss_npc_id_

  def get_boss_info(self):
    boss_info = BossInfo()

    if self.boss_npc_id_ != 0:
      ccnpc = proxy.EntityManager.get_npc(self.boss_npc_id_)
      if ccnpc is not None:
        boss_info.current_hp_ = ccnpc.get_attr_role_aoi(ccentity.role.ttypes.RoleAoiFields.CURRENT_HP)
        boss_info.max_hp_ = ccnpc.get_attr_role_aoi(ccentity.role.ttypes.RoleAoiFields.MAX_HP)
        boss_info.current_mp_ = ccnpc.get_attr_role_aoi(ccentity.role.ttypes.RoleAoiFields.CURRENT_MP)
        boss_info.max_mp_ = ccnpc.get_attr_role_aoi(ccentity.role.ttypes.RoleAoiFields.MAX_MP)

    return boss_info

  def synchronize_boss_info(self, actor_ids):
    boss_info = self.get_boss_info()

    for actor_id in actor_ids:
      request = ccrequest.playing.ttypes.RequestPlayingSynchronizeScore()
      request.actor_ = actor_id
      # BOSS当前血量
      request.score_ = ccentity.playing.ttypes.PlayingScoreField(
          ccentity.playing.ttypes.PlayingScoreType.POWER, 0, boss_info.current_hp_)
      proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_SYNCHRONIZE_SCORE, request)
      # BOSS最大血量
      request.score_ = ccentity.playing.ttypes.PlayingScoreField(
          ccentity.playing.ttypes.PlayingScoreType.POWER, 1, boss_info.max_hp_)
      proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_SYNCHRONIZE_SCORE, request)
      # BOSS当前魔法值
      request.score_ = ccentity.playing.ttypes.PlayingScoreField(
          ccentity.playing.ttypes.PlayingScoreType.POWER, 2, boss_info.current_mp_)
      proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_SYNCHRONIZE_SCORE, request)
      # BOSS最大魔法值
      request.score_ = ccentity.playing.ttypes.PlayingScoreField(
          ccentity.playing.ttypes.PlayingScoreType.POWER, 3, boss_info.max_mp_)
      proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_SYNCHRONIZE_SCORE, request)

  def synchronize_damage_ranking(self, actor_ids):
    ranking = sorted(self.actors_.values(), key=attrgetter('damage_'), reverse=True)

    request = ccrequest.playing.ttypes.RequestPlayingSynchronizeRanking()
    request.actors_ = []
    request.actors_.extend(actor_ids)
    request.ranking_ = []
    for actor in ranking:
      field = ccentity.playing.ttypes.PlayingRankingField()
      field.name_ = actor.get_name()
      field.value_ = actor.get_damage()
      request.ranking_.append(field)
    proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_SYNCHRONIZE_RANKING, request)

  def broadcast_boss_info(self):
    actor_ids = []
    for actor in self.actors_.values():
      if actor.get_leave() is False:
        actor_ids.append(actor.get_id())
    self.synchronize_boss_info(actor_ids)

  def broadcast_damage_ranking(self):
    actor_ids = []
    for actor in self.actors_.values():
      if actor.get_leave() is False:
        actor_ids.append(actor.get_id())
    self.synchronize_damage_ranking(actor_ids)

  def finish(self):
    # 结束副本
    request = ccrequest.playing.ttypes.RequestPlayingComplete()
    request.playing_ = self.get_id()
    result = ccentity.playing.ttypes.PlayingResultField()
    result.values_ = []
    result.award_count_ = 1
    result.paid_award_count_ = 0
    result.awarded_ = False
    request.result_ = result
    request.awards_ = []
    request.awards_.extend(self.config_.get_awards())
    proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_COMPLETE, request)

    for actor in self.actors_.values():
      if actor.get_leave():
        continue

      # 扣副本次数
      request = ccrequest.playing.ttypes.RequestPlayingActorIncreaseComplete()
      request.playing_template_ = self.get_template()
      request.actor_ = actor.get_id()
      proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_ACTOR_INCREASE_COMPLETE,
          request)
      # 发放奖励
      request = ccrequest.playing.ttypes.RequestPlayingAwardActor()
      request.actor_ = actor.get_id()
      request.playing_template_ = self.get_template()
      request.awards_ = []
      request.awards_.extend(self.config_.get_awards())
      proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_AWARD_ACTOR, request)


# 副本管理器
class PlayingManager:
  PLAYINGS_ = {}
  ACTORS_ = {}
  BOSSES_ = {}

  @staticmethod
  def add(playing):
    PlayingManager.PLAYINGS_[playing.get_id()] = playing

  @staticmethod
  def remove(id):
    if id in PlayingManager.PLAYINGS_:
      del PlayingManager.PLAYINGS_[id]

  @staticmethod
  def get(id):
    if id in PlayingManager.PLAYINGS_:
      return PlayingManager.PLAYINGS_[id]
    else:
      return None

  @staticmethod
  def add_actor(id, playing_id):
    PlayingManager.ACTORS_[id] = playing_id

  @staticmethod
  def remove_actor(id):
    if id in PlayingManager.ACTORS_:
      del PlayingManager.ACTORS_[id]

  @staticmethod
  def get_actor_playing(id):
    if id in PlayingManager.ACTORS_:
      return PlayingManager.get(PlayingManager.ACTORS_[id])
    else:
      return None

  @staticmethod
  def add_boss(id, playing_id):
    PlayingManager.BOSSES_[id] = playing_id

  @staticmethod
  def remove_boss(id):
    if id in PlayingManager.BOSSES_:
      del PlayingManager.BOSSES_[id]

  @staticmethod
  def get_boss_playing(id):
    if id in PlayingManager.BOSSES_:
      return PlayingManager.get(PlayingManager.BOSSES_[id])
    else:
      return None


# BOSS配置
class BossCell:
  def __init__(self):
    self.x_ = 0
    self.y_ = 0
    self.template_id_ = 0


# 副本配置
class PlayingConfig:
  def __init__(self, id):
    self.id_ = id
    self.boss_ = None
    self.awards_ = []
    self.time_limit_ = 0

  def get_id(self):
    return self.id_

  def get_boss(self):
    return self.boss_

  def get_awards(self):
    return self.awards_

  def get_time_limit(self):
    return self.time_limit_


# 配置
class Config:
  PLAYING_TEMPLATE_IDS_ = range(201, 225)
  PLAYINGS_ = {}

  @staticmethod
  def initialize():
    for i in Config.PLAYING_TEMPLATE_IDS_:
      playing_cell = playing_types.Config.get(i)
      if playing_cell is None:
        proxy.Logging.error("[guild] playing_types.Config.get(%d) failed" % i)
        return False

      if len(playing_cell.monster_order_) != 1:
        proxy.Logging.error("[guild] playing_cell monsters is invalid")
        return False

      playing_config = PlayingConfig(i)
      playing_config.boss_ = BossCell()
      playing_config.boss_.x_ = 48
      playing_config.boss_.y_ = 38
      playing_config.boss_.template_id_ = playing_cell.monster_order_[0]
      playing_config.awards_ = playing_cell.awards_
      playing_config.time_limit_ = playing_cell.finish_time_
      Config.PLAYINGS_[i] = playing_config

    return True

  @staticmethod
  def get(id):
    if id in Config.PLAYINGS_:
      return Config.PLAYINGS_[id]
    else:
      return None

