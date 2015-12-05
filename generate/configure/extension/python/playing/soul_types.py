
# coding=utf-8

import random
import time

import ccentity
import ccentity.entity
import ccentity.entity.ttypes

import proxy
import facade_request


# 玩家
class Actor:
  def __init__(self, id):
    self.id_ = id
    self.draw_award_ = None

  def get_id(self):
    return self.id_

  def set_draw_award(self, award):
    self.draw_award_ = award

  def get_draw_award(self):
    return self.draw_award_


# 副本对象
class Playing:
  def __init__(self, id, template, scene):
    self.id_ = id
    self.template_ = template
    self.scene_ = scene
    self.start_time_ = time.time()
    self.actors_ = {}
    self.completed_ = False

  def get_id(self):
    return self.id_

  def get_template(self):
    return self.template_

  def get_scene(self):
    return self.scene_

  def get_start_time(self):
    return self.start_time_

  def add_actor(self, actor):
    self.actors_[actor.get_id()] = actor

  def get_actor(self, actor_id):
    if actor_id in self.actors_:
      return self.actors_[actor_id]
    return None

  def get_actors(self):
    return self.actors_

  def clear_actors(self):
    self.actors_.clear()

  def set_complete(self):
    self.completed_ = True

  def get_complete(self):
    return self.completed_


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


# 配置
class Config:
  BOSS_ = 0
  AWARD_ = None
  DRAW_AWARD_ = None
  PLAYING_SECS_ = 0

  @staticmethod
  def set_boss(npc_template):
    Config.BOSS_ = npc_template

  @staticmethod
  def get_boss():
    return Config.BOSS_

  @staticmethod
  def set_playing_secs(secs):
    Config.PLAYING_SECS_ = secs

  @staticmethod
  def get_playing_secs():
    return Config.PLAYING_SECS_

  @staticmethod
  def set_awards(awards):
    Config.AWARDS_ = awards

  @staticmethod
  def get_awards():
    return Config.AWARDS_

  @staticmethod
  def set_draw_awards(awards):
    Config.DRAW_AWARDS_ = awards

  @staticmethod
  def get_draw_awards():
    return Config.DRAW_AWARDS_

