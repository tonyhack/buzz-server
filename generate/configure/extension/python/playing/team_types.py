
import random
import time

import facade_request
import playing_types
import proxy


class Actor:
  def __init__(self, id, name):
    self.id_ = id
    self.name_ = name
    self.total_damage_ = 0
    self.leave_ = False

  def get_id(self):
    return self.id_

  def get_name(self):
    return self.name_

  def get_damage(self):
    return self.total_damage_

  def add_damage(self, damage):
    self.total_damage_ = self.total_damage_ + damage

  def get_leave(self):
    return self.leave_

  def set_leave(self, leave):
    self.leave_ = leave;


class ActorRanking:
  def __init__(self, id):
    self.id_ = id
    self.damage_ = 0

  def get_id(self):
    return self.id_

  def get_damage(self):
    return self.damage_

  def add_damage(self, damage):
    self.damage_ = self.damage_ + damage

def ranking_key(a):
  return a.get_damage()

class DamageRanking:
  def __init__(self):
    self.ranking_list_ = [];

  def add_damage(self, id, damage):
    for actor in self.ranking_list_:
      if actor.get_id() == id:
        actor.add_damage(damage)
        self.sort()
        return None
    actor = ActorRanking(id)
    actor.add_damage(damage)
    self.ranking_list_.append(actor)
    self.sort()

  def sort(self):
    self.ranking_list_.sort(key=ranking_key, reverse=True)

  def get_ranking(self):
    return self.ranking_list_


class Playing:
  def __init__(self, id, template, scene):
    self.actors_ = {}
    self.id_ = id
    self.template_ = template
    self.scene_ = scene
    self.kill_npcs_ = {}
    self.start_time_ = time.time()
    self.completed_ = False
    self.ranking_ = DamageRanking()
    self.next_pos_ = 0

  def get_actor(self, id):
    if id in self.actors_:
      return self.actors_[id]
    else:
      return None

  def get_actors(self):
    return self.actors_

  def add_actor(self, actor):
    self.actors_[actor.get_id()] = actor

  def clear_actor(self):
    self.actors_.clear()

  def get_id(self):
    return self.id_

  def get_template(self):
    return self.template_

  def get_scene(self):
    return self.scene_

  def get_kill_npcs(self):
    return self.kill_npcs_

  def get_kill_npc(self, npc):
    if npc not in self.kill_npcs_:
      return 0
    else:
      return self.kill_npcs_[npc]

  def inc_kill_npc(self, npc):
    if npc not in self.kill_npcs_:
      self.kill_npcs_[npc] = 1
    else:
      self.kill_npcs_[npc] += 1

  def get_start_time(self):
    return self.start_time_

  def set_complete(self):
    self.completed_ = True

  def get_complete(self):
    return self.completed_

  def get_ranking(self):
    return self.ranking_

  def get_next_pos(self):
    return self.next_pos_

  def next_pos(self):
    self.next_pos_ += 1


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


class PlayingConfig:
  def __init__(self, id):
    self.id_ = id
    self.playing_time_ = 0
    self.pass_boss_ = 0
    self.npcs_pos_ = []

  def get_id(self):
    return self.id_

  def set_playing_time(self, time):
    self.playing_time_ = time

  def get_playing_time(self):
    return self.playing_time_

  def set_pass_boss(self, id):
    self.pass_boss_ = id

  def get_pass_boss(self):
    return self.pass_boss_

  def push_next_npc(self, npc_id, x, y):
    self.npcs_pos_.append([npc_id, x, y])

  def summon_next_npc(self, scene, pos):
    if pos >= len(self.npcs_pos_):
      return False
    npc_info = self.npcs_pos_[pos]
    facade_request.summon_npc(scene, npc_info[0], npc_info[1], npc_info[2])
    return True


class Config:
  PLAYINGS_ = {}

  @staticmethod
  def add(playing):
    Config.PLAYINGS_[playing.get_id()] = playing

  @staticmethod
  def get(id):
    if id in Config.PLAYINGS_:
      return Config.PLAYINGS_[id]
    else:
      return None

