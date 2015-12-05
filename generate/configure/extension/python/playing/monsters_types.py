
# coding=utf-8

import random

import ccentity
import ccentity.entity
import ccentity.entity.ttypes

import proxy
import facade_request


# 笼子
class Cage:
  def __init__(self, id, npcs, max_round, interval_secs):
    self.id_ = id
    self.npcs_ = copy.deepcopy(npcs)
    self.max_round_ = round
    self.current_round_ = 0
    self.interval_secs_ = interval_secs
    self.kill_npc_num_ = 0

  def get_id(self):
    return self.id_
  def set_id(self, id):
    self.id_ = id

  def get_npcs(self):
    return self.npcs_

  def get_current_round(self):
    return self.round_
  def set_current_round(self, round):
    self.round_ = round

  def get_max_round(self):
    return self.max_round_

  def get_interval_secs(self):
    return self.interval_secs_

  def get_kill_npc_num(self):
    return self.kill_npc_num_
  def add_kill_npc_num(self, inc):
    return self.kill_npc_num_ += inc
  def check_finish_cage(self):
    if self.kill_npc_num_ >= len(self.npcs_):
      return True
    else:
      return False


# 笼子管理器
class CageSet
  def __init__(self, playing, cage_num, round_num, round_interval_secs):
    self.playing_ = playing
    self.cage_num_ = cage_num
    self.round_num_ = round_num
    self.round_interval_secs_ = round_interval_secs
    self.cage_order_ = []
    self.current_cage_order_index_ = 0
    for i in range(cage_num):
      cage = Cage(i, Config.get_cage_npcs(i), round_num, round_interval_secs)
      self.cages_.append(cage)
      self.cage_order_.append(i)
    range.shuffle(self.cage_order_)

  def get_cage(self, cage_id):
    len = len(self.cages_)
    if len < cage_id:
      return None
    else:
      return self.cages_[cage_id]

  def get_current_cage(self):
    len = len(self.cage_order_)
    if len < self.current_cage_order_index_:
      return None
    else:
      return self.get_cage(self.cage_order_[self.current_cage_order_index_]

  def set_next_cage(self):
    self.cage_order_ += 1


# 副本对象
class Playing:
  def __init__(self, id, template):
    self.id_ = id
    self.template_ = template
    self.scene_ = scene
    self.actor_ = 0
    self.start_time_ = 0
    self.cage_num_ = Config.get_cage_num()
    self.cages_ = CageSet(id, Config.get_cage_num(), Config.get_cage_round(),\
      Config.get_round_interval_secs())
    self.kill_npcs_ = {}

  def get_actor(self):
    return self.actor_

  def set_actor(self, actor):
    self.actor_ = actor

  def get_id(self):
    return self.id_

  def get_template(self):
    return self.template_

  def get_scene(self):
    return self.scene_

  def get_id(self):
    return self.id_

  def get_start_time(self):
    return self.start_time_

  def set_start_time(self, time):
    self.start_time_ = time

  def get_cage_num(self):
    return self.cage_num_

  def get_cages(self):
    return self.cages_

  def get_current_cage(self):
    return self.cages_.get_current_cage()

  def set_next_cage(self):
    self.cages_.set_next_cage()

  def get_kill_npcs(self):
    return self.kill_npcs_

  def get_kill_npc(self, npc):
    if npc in self.kill_npcs_:
      return self.kill_npcs_[npc]
    else:
      return 0

  def get_kill_npc_num(self):
    num = 0
    for (k,v) in self.kill_npcs_.items():
      num += v
    return num

  def inc_kill_npc(self, npc):
    if npc not in self.kill_npcs_:
      self.kill_npcs_[npc] = 1
    else:
      self.kill_npcs_[npc] += 1


# 副本管理器
class PlayingManager:
  PLAYINGS_ = {}
  ACTORS_ = {}

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
      return PlayingManager.ACTORS_[id]
    else:
      return 0


# 奖励配置
class AwardConfig:
  def __init__(self, awards, draw_awards):
    self.awards_ = awards
    self.draw_awards_ = draw_awards

  def get_awards(self):
    return self.awards_

  def get_draw_awards(self):
    return self.draw_awards_


# 配置
class Config:
  PLAYING_TIME_ = 0
  AWARD_ = None
  CAGE_NPCS_ = {}
  CAGE_NUM_ = 0
  CAGE_ROUND_ = 0
  ROUND_INTERVAL_SECS_ = 0

  @staticmethod
  def add_cage_npcs(cage_id, npcs_):
    Config.CAGE_NPCS_[cage_id] = npcs

  @staticmethod
  def get_cage_npcs(cage_id):
    if cage_id in Config.CAGE_NPCS_:
      return Config.CAGE_NPCS_[cage_id]
    return None

  @staticmethod
  def set_award(award):
    Config.AWARD_ = award

  @staticmethod
  def get_award():
    return Config.AWARD_

  @staticmethod
  def set_playing_time(time):
    Config.PLAYING_TIME_ = time

  @staticmethod
  def get_playing_time():
    return Config.PLAYING_TIME_

  @staticmethod
  def set_cage_num(num):
    Config.CAGE_NUM_ = num

  @staticmethod
  def get_cage_num():
    return Config.CAGE_NUM_

  @staticmethod
  def set_cage_round(round):
    Config.CAGE_ROUND_ = round

  @staticmethod
  def get_cage_round():
    return Config.CAGE_ROUND_

  @staticmethod
  def set_round_interval_secs(secs):
    Config.ROUND_INTERVAL_SECS_ = secs

  @staticmethod
  def get_round_interval_secs():
    return Config.ROUND_INTERVAL_SECS_

