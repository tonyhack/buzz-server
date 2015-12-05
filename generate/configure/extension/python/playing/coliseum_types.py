
# coding=utf-8

import random
import time

import ccentity.playing.ttypes
import ccentity.resource.ttypes

import ccrequest.ttypes
import ccrequest.playing.ttypes

import proxy
import facade_request


# 副本对象
class Playing:
  def __init__(self, id, template, scene):
    self.actor_ = 0
    self.id_ = id
    self.template_ = template
    self.scene_ = scene
    self.start_time_ = 0
    self.circle_seq_ = list(range(Config.get_max_circle()))
    self.current_circle_ = -1
    self.current_wave_ = -1
    self.required_kill_npc_number_ = 0
    self.kill_npc_number_ = 0
    self.runes_ = {}
    self.boss_summoned_ = False
    self.finished_ = False
    self.draw_award_item_ = None

    # 随机法阵顺序
    random.shuffle(self.circle_seq_)

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

  def get_start_time(self):
    return self.start_time_

  def set_start_time(self, time):
    self.start_time_ = time

  def get_circle_seq(self):
    return self.circle_seq_

  def get_current_stage(self):
    if self.finished_:
      return Config.MAX_CIRCLE_ + 1
    elif self.boss_summoned_:
      return Config.MAX_CIRCLE_
    else:
      return self.current_circle_

  def is_end_circle(self):
    return self.current_circle_ >= Config.MAX_CIRCLE_ - 1

  def get_current_wave(self):
    return self.current_wave_

  def is_end_wave(self):
    return self.current_wave_ >= Config.MAX_WAVE_ - 1

  def is_in_boss_stage(self):
    return self.boss_summoned_

  def check_finished(self):
    return self.finished_

  def set_finished(self):
    self.finished_ = True
    # 同步当前阶段
    self.synchronize_current_stage()

  def activate_next_circle(self):
    if self.is_end_circle():
      return False

    self.current_circle_ += 1
    self.current_wave_ = -1
    self.required_kill_npc_number_ = 0
    self.kill_npc_number_ = 0
    # 同步当前阶段
    self.synchronize_current_stage()

    return True

  def summon_next_wave(self):
    if self.current_circle_ < 0:
      return False
    if self.is_end_wave():
      return False

    self.current_wave_ += 1

    # 刷新怪物
    cell = Config.get_circle(self.circle_seq_[self.current_circle_])
    for i in range(cell.monster_number_):
      facade_request.summon_npc(self.scene_, cell.monster_template_id_,
          cell.x_, cell.y_, cell.width_, cell.height_)
    self.required_kill_npc_number_ += cell.monster_number_

    return True

  def inc_kill_npc_number(self):
    self.kill_npc_number_ += 1

  def check_circle_finished(self):
    return self.is_end_wave() and \
        self.kill_npc_number_ >= self.required_kill_npc_number_

  def summon_rune(self):
    if self.current_circle_ < 0:
      return

    cell = Config.get_circle(self.circle_seq_[self.current_circle_])
    npc_id = facade_request.summon_npc(self.scene_, cell.rune_template_id_,
        cell.x_, cell.y_)
    self.runes_[npc_id] = time.time() + Config.get_rune_exist_time()

  def remove_rune(self):
    now = time.time()
    for k, v in self.runes_.items():
      if now >= v:
        facade_request.remove_npc(k)
        del self.runes_[k]

  def summon_boss(self):
    if self.boss_summoned_:
      return

    cell = Config.get_boss()
    facade_request.summon_npc(self.scene_, cell.template_id_, cell.x_, cell.y_)
    self.boss_summoned_ = True
    # 同步当前阶段
    self.synchronize_current_stage()

  def synchronize_current_stage(self):
    request = ccrequest.playing.ttypes.RequestPlayingSynchronizeScore()
    request.actor_ = self.actor_
    request.score_ = ccentity.playing.ttypes.PlayingScoreField(
        ccentity.playing.ttypes.PlayingScoreType.STAGE, 0, self.get_current_stage())
    proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_PLAYING_SYNCHRONIZE_SCORE, request)

  def set_draw_award_item(self, item):
      self.draw_award_item_ = item

  def get_draw_award_item(self):
    return self.draw_award_item_


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
      return PlayingManager.get(PlayingManager.ACTORS_[id])
    else:
      return None


# 法阵配置
class CircleCell:
  def __init__(self):
    self.x_ = 0
    self.y_ = 0
    self.width_ = 0
    self.height_ = 0
    self.monster_template_id_ = 0
    self.monster_number_ = 0
    self.rune_template_id_ = 0


# BOSS配置
class BossCell:
  def __init__(self):
    self.x_ = 0
    self.y_ = 0
    self.template_id_ = 0


# 配置
class Config:
  # 开始倒计时
  START_COUNTDOWN_ = 0
  # 法阵数量
  MAX_CIRCLE_ = 0
  # 最大波数
  MAX_WAVE_ = 0
  # 每波间隔时间
  WAVE_INTERVAL_ = 0
  # 符文存在时间
  RUNE_EXIST_TIME_ = 0
  # 法阵信息
  CIRCLES_ = []
  # BOSS信息
  BOSS_ = None
  # 固定奖励
  AWARDS_ = []
  # 抽奖奖励
  DRAW_ITEMS_ = []
  # 快速完成额外经验奖励
  AUTO_FINISH_EXTRA_EXP_ = 0
  # 时间限制
  TIME_LIMIT_ = 0

  @staticmethod
  def initialize(playing_cell):
    Config.START_COUNTDOWN_ = 10
    Config.MAX_CIRCLE_ = 6
    Config.MAX_WAVE_ = 5
    Config.WAVE_INTERVAL_ = 15
    Config.RUNE_EXIST_TIME_ = 20
    Config.AUTO_FINISH_EXTRA_EXP_ = 3050000

    if len(playing_cell.monster_order_) != 2 * Config.MAX_CIRCLE_ + 1:
      proxy.Logging.error("[coliseum] playing_cell monsters is invalid")
      return False

    for i in range(Config.MAX_CIRCLE_):
      cell = CircleCell()
      cell.width_ = 10
      cell.height_ = 10
      cell.monster_template_id_ = playing_cell.monster_order_[i]
      cell.monster_number_ = playing_cell.monsters_[cell.monster_template_id_] / Config.MAX_WAVE_
      cell.rune_template_id_ = playing_cell.monster_order_[Config.MAX_CIRCLE_ + i]
      Config.CIRCLES_.append(cell)

    Config.CIRCLES_[0].x_ = 42
    Config.CIRCLES_[0].y_ = 14
    Config.CIRCLES_[1].x_ = 65
    Config.CIRCLES_[1].y_ = 20
    Config.CIRCLES_[2].x_ = 67
    Config.CIRCLES_[2].y_ = 34
    Config.CIRCLES_[3].x_ = 42
    Config.CIRCLES_[3].y_ = 41
    Config.CIRCLES_[4].x_ = 18
    Config.CIRCLES_[4].y_ = 34
    Config.CIRCLES_[5].x_ = 19
    Config.CIRCLES_[5].y_ = 20

    Config.BOSS_ = BossCell()
    Config.BOSS_.x_ = 42
    Config.BOSS_.y_ = 27
    Config.BOSS_.template_id_ = playing_cell.monster_order_[2 * Config.MAX_CIRCLE_]

    Config.AWARDS_ = playing_cell.awards_
    Config.DRAW_ITEMS_ = playing_cell.draw_items_
    Config.TIME_LIMIT_ = playing_cell.finish_time_

    return True

  @staticmethod
  def get_start_countdown():
    return Config.START_COUNTDOWN_

  @staticmethod
  def get_max_circle():
    return Config.MAX_CIRCLE_

  @staticmethod
  def get_max_mave():
    return Config.MAX_WAVE_

  @staticmethod
  def get_wave_interval():
    return Config.WAVE_INTERVAL_

  @staticmethod
  def get_rune_exist_time():
    return Config.RUNE_EXIST_TIME_

  @staticmethod
  def get_circle(circle_index):
    return Config.CIRCLES_[circle_index]

  @staticmethod
  def get_boss():
    return Config.BOSS_

  @staticmethod
  def get_awards():
    return Config.AWARDS_

  @staticmethod
  def get_rand_draw_item():
    if len(Config.DRAW_ITEMS_) > 0:
      return random.choice(Config.DRAW_ITEMS_)
    else:
      return ccentity.playing.ttypes.PlayingAwardField(
          ccentity.resource.ttypes.ResourceType.MIN, 0, 0)

  @staticmethod
  def get_auto_finish_extra_exp():
    return Config.AUTO_FINISH_EXTRA_EXP_

  @staticmethod
  def get_time_limit():
    return Config.TIME_LIMIT_

