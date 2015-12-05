
import time

import facade_request
import playing_types
import proxy


class Actor:
  def __init__(self, id, name):
    self.id_ = id
    self.name_ = name
    self.total_kill_npc_ = 0
    self.total_kill_actor_ = 0
    self.score_ = 0
    self.leaving = False

  def get_id(self):
    return self.id_

  def get_name(self):
    return self.name_

  def get_total_kill_npc(self):
    return self.total_kill_npc_

  def inc_kill_npc(self):
    self.total_kill_npc_ += 1

  def get_total_kill_actor(self):
    return self.total_kill_actor_

  def inc_kill_actor(self):
    self.total_kill_actor_ += 1


class ActorRanking:
  def __init__(self, id):
    self.id_ = id
    self.score_ = 0

  def add_score(self, score):
    self.score_ += score

def ranking_key(k):
  return k.score_


class ScoreRanking:
  def __init__(self):
    self.ranking_list_ = []

  def add_score(self, id, score):
    for actor in self.ranking_list_:
      if actor.id_ == id:
        actor.add_score(score)
        self.sort()
        return None
    actor = ActorRanking(id)
    actor.add_score(score)
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
    self.start_time_ = time.time()
    self.group_ = 0
    self.boss_time_ = False
    self.ranking_ = ScoreRanking()

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

  def get_start_time(self):
    return self.start_time_


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


class MonsterGroupConfig:
  def __init_(self):
    self.monster1_id_ = 0
    self.monster1_num_ = 0
    self.monster2_id_ = 0
    self.monster2_num_ = 0
    self.award_monster1_id_ = 0
    self.award_monster1_num_ = 0
    self.award_monster2_id_ = 0
    self.award_monster2_num_ = 0
    self.boss_id_ = 0
    self.boss_number_ = 0


class PlayingConfig:
  def __init__(self, id):
    self.id_ = id
    self.playing_time_ = 0
    self.pos_ = []
    self.monster_groups_ = []
    self.last_boss_id_ = 0
    self.scores_ = {}
    self.kill_actor_score_ = 0
    self.award_pos_ = []
    self.award_pos_range = 0
    self.boss_pos_ = []

  def get_id(self):
    return self.id_

  def push_next_pos(self, x, y):
    self.pos_.append([x,y])

  def get_monster_score(self, id):
    if id in self.scores_:
      return self.scores_[id]
    else:
      return 0

  def get_actor_score(self):
    return self.kill_actor_score_

  def get_ranking_award_multiple(self, ranking):
    if ranking == 1:
      return 5
    elif ranking == 2:
      return 3
    elif ranking == 3:
      return 2
    else:
      return 0


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

