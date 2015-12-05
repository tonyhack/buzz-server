
# coding=utf-8

import random
import time

import playing_types

# 玩家类
class Actor:
  def __init__(self, id, now):
    self.id_ = id
    self.start_time_ = now
    self.finish_ = 0
    self.dead_count_ = 0
    self.draw_award_item_ = 0

  def get_finish(self):
    return self.finish_

  def set_finish(self):
    self.finish_ = 1

  def get_id(self):
    return self.id_

  def get_start_time(self):
    return self.start_time_

  def inc_dead_count(self):
    self.dead_count_ += 1

  def get_dead_count(self):
    return self.dead_count_

  def draw_award_item(self, items):
    size = len(items)
    if size >0:
      self.draw_award_item_ = items[random.randint(0, size - 1)]
    return self.draw_award_item_

  def get_draw_award_item(self):
    return self.draw_award_item_


# 玩家管理器类
class ActorManager:
  def __init__(self):
    self.actors_ = {}

  def get(self, id):
    if id in self.actors_:
      return self.actors_[id]
    else:
      return None

  def add(self, actor):
    self.actors_[actor.get_id()] = actor

  def remove(self, id):
    if id in self.actors_:
      del self.actors_[id]

  def clear(self):
    self.actors_.clear()


# 副本类
class Playing:
  def __init__(self, id, template_id, scene):
    self.actors_ = ActorManager()
    self.id_ = id
    self.template_id_ = template_id
    self.scene_ = scene

  def get_actor(self, id):
    return self.actors_.get(id)

  def add_actor(self, actor):
    self.actors_.add(actor)

  def get_id(self):
    return self.id_

  def get_template_id(self):
    return self.template_id_

  def get_scene(self):
    return self.scene_

  def clear(self):
    self.actors_.clear()


# 副本管理器类
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


# 副本配置
class PlayingConfig:
  def __init__(self, template_id, x, y):
    self.template_id_ = template_id
    self.x_ = x
    self.y_ = y

  def initialize(self):
    playing_cell = playing_types.Config.get(self.template_id_)
    if playing_cell == None:
      return False
    self.awards_ = playing_cell.awards_
    self.draw_items_ = playing_cell.draw_items_
    self.pass_kill_npc_ = playing_cell.pass_boss_
    return True

  def get_template_id(self):
    return self.template_id_

  def get_pass_kill_npc(self):
    return self.pass_kill_npc_

  def get_pass_npc_x(self):
    return self.x_;

  def get_pass_npc_y(self):
    return self.y_;

  def get_awards(self):
    return self.awards_

  def get_draw_awards(self):
    return self.draw_items_


# 配置管理器
class Config:
  PLAYINGS_ = {}

  @staticmethod
  def add(playing):
    Config.PLAYINGS_[playing.get_template_id()] = playing

  @staticmethod
  def get(id):
    if id in Config.PLAYINGS_:
      return Config.PLAYINGS_[id]
    else:
      return None

