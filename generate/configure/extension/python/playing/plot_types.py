
import random
import time

import playing_types

class Actor:
  def __init__(self, id, now):
    self.id_ = id
    self.finish_ = 0
    self.kill_npcs_ = {}
    self.draw_award_item_ = 0
    self.score_ = 0
    self.award_gold_ = 0
    self.award_exp_ = 0
    self.award_nimbus_ = 0
    self.start_time_ = now
    self.dead_count_ = 0
    self.hide_npc_ = 0

  def get_finish(self):
    return self.finish_

  def set_finish(self):
    self.finish_ = 1

  def get_id(self):
    return self.id_

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

  def get_score(self):
    return self.score_

  def inc_score(self, increament):
    self.score_ += increament

  def draw_award_item(self, items):
    size = len(items)
    if size > 0:
      self.draw_award_item_ = items[random.randint(0, size - 1)]
    return self.draw_award_item_

  def get_draw_award_item(self):
    return self.draw_award_item_

  def get_award_gold(self):
    return self.award_gold_

  def set_award_gold(self, value):
    self.award_gold_ = value

  def get_award_exp(self):
    return self.award_exp_

  def set_award_exp(self, value):
    self.award_exp_ = value

  def get_award_nimbus(self):
    return self.award_nimbus_

  def set_award_nimbus(self, value):
    self.award_nimbus_ = value

  def get_start_time(self):
    return self.start_time_

  def set_start_time(self, value):
    self.start_time_ = value

  def get_dead_count(self):
    return self.dead_count_

  def set_start_time(self, value):
    self.dead_count_ = value

  def inc_dead_count(self, increament):
    self.dead_count_ += increament

  def get_hide_npc(self):
    return self.hide_npc_

  def inc_hide_npc(self, increament):
    self.hide_npc_ += increament


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


class PlayingConfig:
  def __init__(self, id):
    self.id_ = id

  def initialize(self):
    playing_cell = playing_types.Config.get(self.id_)
    if playing_cell == None:
      return False
    self.draw_items_ = playing_cell.draw_items_
    self.awards_ = playing_cell.awards_
    self.pass_kill_npc_ = playing_cell.pass_boss_
    self.dead_count_ = playing_cell.dead_count_
    self.spend_time_ = playing_cell.finish_time_
    self.npc_number_ = 0
    for (k,v) in playing_cell.monsters_.items():
      self.npc_number_ += v
    return True

  def get_id(self):
    return self.id_

  def get_draw_items(self):
    return self.draw_items_

  def get_awards(self):
    return self.awards_

  def get_pass_kill_npc(self):
    return self.pass_kill_npc_

  def get_dead_count(self):
    return self.dead_count_

  def get_spend_time(self):
    return self.spend_time_

  def get_npc_number(self):
    return self.npc_number_

  def set_hide_npc(self, hide_npc):
    self.hide_npc_ = hide_npc

  def get_hide_npc(self):
    return self.hide_npc_


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


class Playing:
  def __init__(self, id, template, scene):
    self.actors_ = ActorManager()
    self.id_ = id
    self.template_ = template
    self.scene_ = scene

  def get_actor(self, id):
    return self.actors_.get(id)

  def add_actor(self, actor):
    self.actors_.add(actor)

  def get_id(self):
    return self.id_

  def get_template(self):
    return self.template_

  def get_scene(self):
    return self.scene_

  def clear(self):
    self.actors_.clear()


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

