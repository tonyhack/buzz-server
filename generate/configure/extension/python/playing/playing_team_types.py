
import random
import time

import playing_types

class Actor:
  def __int__(self, id, now):
    self.id_ = id
    self.total_damage_ = 0

  def get_id(self):
    return self.id_

  def get_damage(self):
    return self.total_damage_

  def add_damage(self, damage):
    self.total_damage_ = self.total_damage_ + damage


class Playing
  def __int__(self, id, template, scene):
    self.actors_ = {}
    self.id_ = id
    self.template_ = template
    self.scene_ = scene
    self.current_pass_ = 0
    self.kill_npcs_ = {}

  def get_actor(self, id):
    if id in self.actors_:
      return self.actors_[id]
    else:
      return None

  def add_actor(self, id):
    if id in self.actors_:
      del self.actors_[id]

  def clear_actor(self):
    self.actors_.clear()

  def get_id(self):
    return self.id_

  def get_template(self):
    return self.template_

  def get_scene(self):
    return self.scene_

  def get_current_pass(self):
    return self.current_pass_

  def add_current_pass(self):
    return self.current_pass_ = self.current_pass_ + 1

  def get_kill_npcs(self):
    return self.kill_npcs_

  def inc_kill_npc(self, npc):
    if npc not in self.kill_npcs_:
      self.kill_npcs_[npc] = 1
    else:
      self.kill_npcs_[npc] += 1


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


class BossConfig:
  def __int__(self, id, x, y, award):
    self.id_ = id
    self.x_ = x
    self.y_ = y
    self.award_ = award

  def get_id(self):
    return self.id_

  def get_x(self):
    return self.x_

  def get_y(self):
    return self.y_

  def get_award(self):
    return self.award_


class Config:
  BOSSES_ = []
  PLAYING_TIME_ = 0

  @staticmethod
  def add_boss_config(boss_config):
    Config.BOSSES_.append(boss_config)

  @staticmethod
  def get_bosses_config():
    return Config.BOSSES_

  @staticmethod
  def set_playing_time(time):
    Config.PLAYING_TIME_ = time

  
