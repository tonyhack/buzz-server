
# coding=utf-8

import random
import time

import facade_request
import playing_types
import proxy

import ccentity.entity.ttypes
import ccentity.playing.ttypes
import ccentity.role.ttypes

import ccevent.ttypes
import ccevent.role.ttypes

import ccrequest.ttypes
import ccrequest.ai.ttypes
import ccrequest.playing.ttypes
import ccrequest.scene.ttypes


class Actor:
  def __init__(self, id):
    self.id_ = id
    self.leave_ = False

  def get_id(self):
    return self.id_

  def get_leave(self):
    return self.leave_

  def set_leave(self, value):
    self.leave_ = value


class Monster:
  def __init__(self, id, template):
    self.id_ = id
    self.fighting_status_ = False
    self.template_ = template


class Playing:
  def __init__(self):
    self.id_ = 0

  def initialize(self, id, template, scene):
    self.id_ = id
    self.template_ = template
    self.scene_ = scene
    self.start_time_ = 0
    self.actors_ = {}
    self.monsters_ = {}
    self.monster_numbers_ = {}
    self.unreal_monster_order_ = []
    self.current_unreal_wave_ = -1
    self.current_unreal_soul_id_ = 0
    self.draw_award_ = None
    self.kill_wrong_num_ = 0
    self.kill_right_num_ = 0
    self.boss_stage_ = False

    for monster_cell in Config.MONSTERS_:
      self.unreal_monster_order_.append(monster_cell.template_)
      self.monster_numbers_[monster_cell.template_] = 0
      for pos in range(0, monster_cell.number_):
        monster_id = facade_request.summon_npc(scene, monster_cell.template_,
            monster_cell.x_, monster_cell.y_, monster_cell.width_, monster_cell.height_)
        if monster_id != 0:
          monster = Monster(monster_id, monster_cell.template_)
          self.monsters_[monster.id_] = monster
          self.monster_numbers_[monster.template_] += 1
          PlayingManager.add_monster(monster.id_, self.id_)
          proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_ROLE_ENTER_FIGHTING_STATUS,
              monster.id_, ccevent.ttypes.ChannelType.CHANNEL_NPC,
              "playing_unreal_soul_on_event_role_enter_fighting_status")
          proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_ROLE_LEAVE_FIGHTING_STATUS,
              monster.id_, ccevent.ttypes.ChannelType.CHANNEL_NPC,
              "playing_unreal_soul_on_event_role_leave_fighting_status")

    random.shuffle(self.unreal_monster_order_)

  def finalize(self):
    for (id, monster) in self.monsters_.items():
      proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_ROLE_ENTER_FIGHTING_STATUS,
          id, ccevent.ttypes.ChannelType.CHANNEL_NPC,
          "playing_unreal_soul_on_event_role_enter_fighting_status")
      proxy.Communicator.follow(ccevent.ttypes.EventType.EVENT_ROLE_LEAVE_FIGHTING_STATUS,
          id, ccevent.ttypes.ChannelType.CHANNEL_NPC,
          "playing_unreal_soul_on_event_role_leave_fighting_status")

  def get_actor(self, id):
    if id in self.actors_:
      return self.actors_[id]
    else:
      return None

  def add_actor(self, actor):
    self.actors_[actor.get_id()] = actor

  def get_monster(self, id):
    return self.monsters_[id]

  def remove_monster(self, id):
    if id in self.monsters_:
      monster = self.monsters_[id]
      self.monster_numbers_[monster.template_] -= 1
      del self.monsters_[id]
      PlayingManager.remove_monster(id)
      proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_ROLE_ENTER_FIGHTING_STATUS,
          id, ccevent.ttypes.ChannelType.CHANNEL_NPC,
          "playing_unreal_soul_on_event_role_enter_fighting_status")
      proxy.Communicator.unfollow(ccevent.ttypes.EventType.EVENT_ROLE_LEAVE_FIGHTING_STATUS,
          id, ccevent.ttypes.ChannelType.CHANNEL_NPC,
          "playing_unreal_soul_on_event_role_leave_fighting_status")

  def get_monster_number(self, template):
    if id in self.monsters_:
      return self.monster_numbers_[template]
    else:
      return None

  def get_current_unreal(self):
    return self.current_unreal_wave_

  def get_current_unreal_template(self):
    if self.current_unreal_wave_ >= 0 and\
      self.current_unreal_wave_ < len(self.monster_numbers_):
      return self.unreal_monster_order_[self.current_unreal_wave_]
    else:
      return 0

  def check_current_unreal_empty(self):
    current_template = self.get_current_unreal_template()
    if current_template == 0:
      return True
    else:
      return self.monster_numbers_[current_template] == 0

  def kill_npc(self, template):
    if template == self.get_current_unreal_template():
      self.kill_right_num_ += 1
    else:
      self.kill_wrong_num_ += 1

    self.broadcast_score()

  def goto_next_unreal(self):
    if self.current_unreal_wave_ >= 0:
      self.freeze_current_monster_drop()
      self.reset_unreal_monster_ai()

    len_monster_numbers = len(self.monster_numbers_)
    if self.current_unreal_wave_ >= len_monster_numbers - 1:
      return False

    self.current_unreal_wave_ += 1
    while self.get_current_unreal_template() != 0 and self.check_current_unreal_empty() == True:
      self.current_unreal_wave_ += 1

    if self.get_current_unreal_template() == 0:
      return False

    self.transfer_unreal_soul()
    self.activate_current_monster_drop()
    self.set_unreal_monster_ai()
    return True

  def transfer_unreal_soul(self):
    if self.current_unreal_soul_id_ != 0:
      facade_request.remove_npc(self.current_unreal_soul_id_)
    self.current_unreal_soul_id_ = facade_request.summon_npc(self.scene_,
        self.unreal_monster_order_[self.current_unreal_wave_] * 10 + 1,
        Config.UNREAL_SOUL_X_, Config.UNREAL_SOUL_Y_)

  def freeze_current_monster_drop(self):
    request = ccrequest.scene.ttypes.RequestSceneForbidDropItem()
    request.npcs_ = []
    request.forbid_ = True
    for (id, monster) in self.monsters_.items():
      if monster.template_ == self.unreal_monster_order_[self.current_unreal_wave_]:
        request.npcs_.append(id)
    proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_SCENE_FORBID_DROP_ITEM,
        request)

  def activate_current_monster_drop(self):
    request = ccrequest.scene.ttypes.RequestSceneForbidDropItem()
    request.npcs_ = []
    request.forbid_ = False
    for (id, monster) in self.monsters_.items():
      if monster.template_ == self.unreal_monster_order_[self.current_unreal_wave_]:
        request.npcs_.append(id)
    proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_SCENE_FORBID_DROP_ITEM,
        request)

  def set_unreal_monster_ai(self):
    request = ccrequest.ai.ttypes.RequestAiSetAi()
    request.type_ = ccentity.entity.ttypes.EntityType.TYPE_NPC
    request.ai_ = 4
    for (id, monster) in self.monsters_.items():
      if monster.template_ == self.unreal_monster_order_[self.current_unreal_wave_]:
        request.id_ = id
        proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_AI_SET_AI,
            request)

  def reset_unreal_monster_ai(self):
    request = ccrequest.ai.ttypes.RequestAiSetAi()
    request.type_ = ccentity.entity.ttypes.EntityType.TYPE_NPC
    request.ai_ = 2
    for (id, monster) in self.monsters_.items():
      if monster.template_ == self.unreal_monster_order_[self.current_unreal_wave_]:
        request.id_ = id
        proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_AI_SET_AI,
            request)

  def check_summon_boss(self):
    if self.current_unreal_wave_ >= len(self.monster_numbers_):
      return True
    return False

  def summon_boss(self):
    if self.boss_stage_ == False:
      facade_request.summon_npc(self.scene_, Config.BOSS_.template_,
          Config.BOSS_.x_, Config.BOSS_.y_)
      self.boss_stage_ = True

  def set_draw_award(self, award):
    self.draw_award_ = award

  def get_draw_award(self):
    return self.draw_award_

  def strengthen_monster(self):
    for (id, monster) in self.monsters_.items():
      if monster.fighting_status_ == True:
        ccmonster = proxy.EntityManager.get_npc(id)
        if ccmonster != None:
          physique_per = ccmonster.get_attr_role_server(ccentity.role.ttypes.RoleServerFields.PHYSIQUE_ADD_PERCENT)
          ccmonster.set_attr_role_server(ccentity.role.ttypes.RoleServerFields.PHYSIQUE_ADD_PERCENT, physique_per + 10000)
          strength_per = ccmonster.get_attr_role_server(ccentity.role.ttypes.RoleServerFields.STRENGTH_ADD_PERCENT)
          ccmonster.set_attr_role_server(ccentity.role.ttypes.RoleServerFields.STRENGTH_ADD_PERCENT, strength_per + 10000)
          intelligence_ptr = ccmonster.get_attr_role_server(ccentity.role.ttypes.RoleServerFields.INTELLIGENCE_ADD_PERCENT)
          ccmonster.set_attr_role_server(ccentity.role.ttypes.RoleServerFields.INTELLIGENCE_ADD_PERCENT, intelligence_ptr + 10000)
          agile_per = ccmonster.get_attr_role_server(ccentity.role.ttypes.RoleServerFields.AGILE_ADD_PERCENT)
          ccmonster.set_attr_role_server(ccentity.role.ttypes.RoleServerFields.AGILE_ADD_PERCENT, agile_per + 10000)
          spirit_per = ccmonster.get_attr_role_server(ccentity.role.ttypes.RoleServerFields.SPIRIT_ADD_PERCENT)
          ccmonster.set_attr_role_server(ccentity.role.ttypes.RoleServerFields.SPIRIT_ADD_PERCENT, spirit_per + 10000)

  def broadcast_score(self):
    actor_ids = []
    for (actor_id, actor) in self.actors_.items():
      if actor.get_leave() == True:
        continue
      request = ccrequest.playing.ttypes.RequestPlayingSynchronizeScore()
      request.actor_ = actor_id
      request.score_ = ccentity.playing.ttypes.PlayingScoreField(
        ccentity.playing.ttypes.PlayingScoreType.KILL_NPC, 0, self.kill_wrong_num_)
      proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_SYNCHRONIZE_SCORE, request)
      request.score_ = ccentity.playing.ttypes.PlayingScoreField(
        ccentity.playing.ttypes.PlayingScoreType.KILL_NPC, 1, self.kill_right_num_)
      proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_SYNCHRONIZE_SCORE, request)


class PlayingManager:
  PLAYINGS_ = {}
  ACTORS_ = {}
  MONSTERS_ = {}

  @staticmethod
  def add(playing):
    PlayingManager.PLAYINGS_[playing.id_] = playing

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

  @staticmethod
  def add_monster(id, playing_id):
    PlayingManager.MONSTERS_[id] = playing_id

  @staticmethod
  def remove_monster(id):
    if id in PlayingManager.MONSTERS_:
      del PlayingManager.MONSTERS_[id]

  @staticmethod
  def get_monster_playing(id):
    if id in PlayingManager.MONSTERS_:
      return PlayingManager.MONSTERS_[id]
    else:
      return 0


class MonsterCell:
  def __init__(self):
    self.x_ = 0
    self.y_ = 0
    self.width_ = 0
    self.height_ = 0
    self.template_ = 0
    self.number_ = 0

class BossCell:
  def __init__(self):
    self.x_ = 0
    self.y_ = 0
    self.template_ = 0

class Config:
  UNREAL_TRANSFER_INTERVAL = 0
  AWARDS_ = []
  DRAW_AWARDS_ = []
  BOSS_ = None
  MONSTERS_ = []
  PLAYING_MAX_TIME_ = 0
  UNREAL_SOUL_X_ = 0
  UNREAL_SOUL_Y_ = 0

  @staticmethod
  def initialize(playing_cell):
    Config.UNREAL_TRANSFER_INTERVAL = 180

    Config.AWARDS_ = playing_cell.awards_
    Config.DRAW_AWARDS_ = playing_cell.draw_items_

    len_monster_order = len(playing_cell.monster_order_)
    if len_monster_order <= 0:
      proxy.Logging.error("[unreal_soul] playing_cell monster_order error")
      return False
 
    Config.BOSS_ = BossCell()
    Config.BOSS_.x_ = 50
    Config.BOSS_.y_ = 50
    Config.BOSS_.template_ = playing_cell.monster_order_[len_monster_order - 1]

    Config.UNREAL_SOUL_X_ = 38
    Config.UNREAL_SOUL_Y_ = 36
 
    for monster_template in playing_cell.monster_order_:
      if monster_template != Config.BOSS_.template_:
        cell = MonsterCell()
        cell.width_ = 38
        cell.height_ = 38
        cell.x_ = 40
        cell.y_ = 45
        cell.template_ = monster_template
        cell.number_ = playing_cell.monsters_[cell.template_]
        Config.MONSTERS_.append(cell)

    Config.PLAYING_MAX_TIME_ = playing_cell.finish_time_

    return True

  @staticmethod
  def get_rand_draw():
    if len(Config.DRAW_AWARDS_) > 0:
      return random.choice(Config.DRAW_AWARDS_)
    else:
      return ccentity.playing.ttypes.PlayingAwardField(
          ccentity.resource.ttypes.ResourceType.MIN, 0, 0)

