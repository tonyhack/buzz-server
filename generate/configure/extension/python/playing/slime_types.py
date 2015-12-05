
# coding=utf-8

import random

import ccentity
import ccentity.entity
import ccentity.entity.ttypes

import proxy
import facade_request

# 棋子
class Chess:
  def __init__(self, template, id):
    self.template_ = template
    self.id_ = id

  def get_template(self):
    return self.template_
  def set_template(self, template):
    self.template_ = template

  def get_id(self):
    return self.id_
  def set_id(self, id):
    self.id_ = id


# 棋盘
class Chessboard:
  WIDTH = 6
  HEIGHT = 5

  def __init__(self, playing):
    self.board_ = []
    self.playing_ = playing
    for x in range(Chessboard.WIDTH): 
      self.board_.append([])
      for y in range(Chessboard.HEIGHT):
        chess = Chess(0, 0)
        self.board_[x].append(chess)

  # 获取棋盘中某个指定id棋子的坐标
  def get_pos(self, id):
    if id <= 0:
      return -1, -1
    for x in range(Chessboard.WIDTH):
      for y in range(Chessboard.HEIGHT):
        if self.board_[x][y].get_id() == id:
          return x, y
    return -1, -1

  # 刷新棋盘
  def refresh_all(self):
    proxy.Logging.debug("[slime] refresh_all")
    for x in range(Chessboard.WIDTH):
      for y in range(Chessboard.HEIGHT):
        if self.board_[x][y].get_id() == 0:
          template = Config.random_get()
          id = facade_request.summon_npc(self.playing_.get_scene(),\
              template, x*5+16, y*7+17, 0, 0, 0, ccentity.direction.ttypes.DirectionType.SOUTH)
          self.board_[x][y].set_template(template)
          self.board_[x][y].set_id(id)
          proxy.Logging.debug("[slime] summon npc(%d, %d)" % (template, id))

  # 刷新棋盘中的某个坐标
  def refresh_by_pos(self, x, y):
    if x < 0 or x >= Chessboard.WIDTH:
      return False
    if y < 0 or y >= Chessboard.HEIGHT:
      return False
    template = Config.random_get()
    id = facade_request.summon_npc(self.playing_.get_scene(),\
        template, x*5+16, y*7+17, 0, 0, 0, ccentity.direction.ttypes.DirectionType.SOUTH)
    self.board_[x][y].set_template(template)
    self.board_[x][y].set_id(id)
    proxy.Logging.debug("[slime] summon npc(%d, %d)" % (template, id))

  # 删除棋盘中所有棋子
  def erase_all(self):
    for x in range(Chessboard.WIDTH):
      for y in range(Chessboard.HEIGHT):
        facade_request.remove_npc(self.board_[x][y].get_id(), 0)
        self.board_[x][y].set_template(0);
        self.board_[x][y].set_id(0);

  # 删除棋盘中某个坐标上的棋子
  def erase_by_pos(self, x, y):
    if x < 0 or x >= Chessboard.WIDTH:
      return False
    if y < 0 or y >= Chessboard.HEIGHT:
      return False
    id = self.board_[x][y].get_id()
    facade_request.remove_npc(id, 0)
    self.board_[x][y].set_template(0);
    self.board_[x][y].set_id(0);

  # 删除棋盘中某个指定id的棋子
  def erase_by_id(self, id):
    if id <= 0:
      return False
    for x in range(Chessboard.WIDTH):
      for y in range(Chessboard.HEIGHT):
        if self.board_[x][y].get_id() == id:
          facade_request.remove_npc(id, 0)
          self.board_[x][y].set_template(0);
          self.board_[x][y].set_id(0);

  # 删除棋盘中某坐标与某棋子相关的棋子
  def erase_associated_by_pos(self, npc_template, x, y):
    if x < 0 or x >= Chessboard.WIDTH:
      proxy.Logging.error("[slime] erase_associated_by_pos(%d,%d,%d)"\
          % (npc_template, x, y))
      return 0
    if y < 0 or y >= Chessboard.HEIGHT:
      proxy.Logging.error("[slime] erase_associated_by_pos(%d,%d,%d)"\
          % (npc_template, x, y))
      return 0
    score = 0
    for x1 in range(x-1, -1, -1):
      proxy.Logging.debug("[slime] x1(%d) in range(x, -1, -1)" % x1)
      if self.board_[x1][y].get_template() == npc_template:
        facade_request.remove_npc(self.board_[x1][y].get_id(), 0)
        score = score + 1
        self.board_[x1][y].set_template(0)
        self.board_[x1][y].set_id(0)
        self.refresh_by_pos(x1, y)
      else:
        proxy.Logging.debug("[slime] associated1: %d %d" % (self.board_[x1][y].get_template(), npc_template))
        break
    for x1 in range(x+1, Chessboard.WIDTH, 1):
      proxy.Logging.debug("[slime] x1(%d) in range(x, 1, Chessboard.WIDTH)" % x1)
      if self.board_[x1][y].get_template() == npc_template:
        facade_request.remove_npc(self.board_[x1][y].get_id(), 0)
        score = score + 1
        self.board_[x1][y].set_template(0)
        self.board_[x1][y].set_id(0)
        self.refresh_by_pos(x1, y)
        proxy.Logging.debug("[slime] associated2: %d %d" % (self.board_[x1][y].get_template(), npc_template))
      else:
        break
    for y1 in range(y-1, -1, -1):
      proxy.Logging.debug("[slime] y1(%d) in range(y, -1, -1)" % y1)
      if self.board_[x][y1].get_template() == npc_template:
        facade_request.remove_npc(self.board_[x][y1].get_id(), 0)
        score = score + 1
        self.board_[x][y1].set_template(0)
        self.board_[x][y1].set_id(0)
        self.refresh_by_pos(x, y1)
        proxy.Logging.debug("[slime] associated3: %d %d" % (self.board_[x][y1].get_template(), npc_template))
      else:
        break
    for y1 in range(y+1, Chessboard.HEIGHT, 1):
      proxy.Logging.debug("[slime] y1(%d) in range(y, 1, Chessboard.WIDTH)" % y1)
      if self.board_[x][y1].get_template() == npc_template:
        facade_request.remove_npc(self.board_[x][y1].get_id(), 0)
        score = score + 1
        self.board_[x][y1].set_template(0)
        self.board_[x][y1].set_id(0)
        self.refresh_by_pos(x, y1)
        proxy.Logging.debug("[slime] associated4: %d %d" % (self.board_[x][y1].get_template(), npc_template))
      else:
        break
    return score


# 副本对象
class Playing:
  def __init__(self, id, template, scene):
    self.actor_ = None
    self.id_ = id
    self.template_ = template
    self.scene_ = scene
    self.start_time_ = 0
    self.score_ = 0
    self.actor_ = 0
    self.exp_ = 0
    self.spend_power_ = 0
    self.section_boss_ = False
    self.kill_boss_ = False
    self.boss_summon_time_ = 0
    self.chessboard_ = Chessboard(self)
    self.chessboard_.refresh_all();
    self.draw_award_ = None

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

  def get_spend_power(self):
    return self.spend_power_

  def add_spend_power(self, value):
    self.spend_power_ = self.spend_power_ + 1

  def get_score(self):
    return self.score_

  def add_score(self, score):
    self.score_ = self.score_ + score

  def get_exp(self):
    return self.exp_

  def add_exp(self, value):
    self.exp_ = self.exp_ + value

  def get_section_boss(self):
    return self.section_boss_

  def set_section_boss(self):
    self.section_boss_ = True

  def get_kill_boss(self):
    return self.kill_boss_

  def set_kill_boss(self):
    self.kill_boss_ = True

  def get_boss_summon_time(self):
    return self.boss_summon_time_

  def set_boss_summon_time(self, time):
    self.boss_summon_time_ = time

  def get_chessboard(self):
    return self.chessboard_

  def set_draw_award(self, award):
    self.draw_award_ = award

  def get_draw_award(self):
    return self.draw_award_


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


# 经验、灵气、金币、魔晶、爵位进阶石及抽奖奖励
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
  NPCS_ = []
  BOSS_ = 0
  BOSS_X_ = 0
  BOSS_Y_ = 0
  MAX_POWER_ = 0
  KILL_EXP_ = 0
  MULTI_KILL_NUM_ = 0
  MULTI_KILL_EXP_ = 0
  SLIME_TIME_ = 0
  BOSS_TIME_ = 0
  AWARD_ = None

  @staticmethod
  def add(npc_template):
    Config.NPCS_.append(npc_template)

  @staticmethod
  def set_boss(npc_template):
    Config.BOSS_ = npc_template

  @staticmethod
  def get_boss():
    return Config.BOSS_

  @staticmethod
  def set_boss_position(x, y):
    Config.BOSS_X_ = x
    Config.BOSS_Y_ = y

  @staticmethod
  def get_boss_position():
    return Config.BOSS_X_, Config.BOSS_Y_

  @staticmethod
  def set_max_power(power):
    Config.MAX_POWER_ = power

  @staticmethod
  def get_max_power():
    return Config.MAX_POWER_

  @staticmethod
  def set_kill_exp(exp):
    Config.KILL_EXP_ = exp

  @staticmethod
  def get_kill_exp():
    return Config.KILL_EXP_

  @staticmethod
  def set_multi_kill_num(num):
    Config.MULTI_KILL_NUM_ = num

  @staticmethod
  def get_multi_kill_num():
    return Config.MULTI_KILL_NUM_

  @staticmethod
  def set_multi_kill_exp(exp):
    Config.MULTI_KILL_EXP_ = exp

  @staticmethod
  def get_multi_kill_exp():
    return Config.MULTI_KILL_EXP_

  @staticmethod
  def set_award(award):
    Config.AWARD_ = award

  @staticmethod
  def get_award():
    return Config.AWARD_

  @staticmethod
  def set_slime_time(time):
    Config.SLIME_TIME_ = time

  @staticmethod
  def get_slime_time():
    return Config.SLIME_TIME_

  @staticmethod
  def set_boss_time(time):
    Config.BOSS_TIME_ = time

  @staticmethod
  def get_boss_time():
    return Config.BOSS_TIME_

  @staticmethod
  def random_get():
    npc_len = len(Config.NPCS_)
    if npc_len <= 0:
      return 0
    return Config.NPCS_[random.randint(0, npc_len - 1)]

