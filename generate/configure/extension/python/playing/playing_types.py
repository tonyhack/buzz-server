
# coding=utf-8

import random
import time
import xml.dom.minidom

import ccentity
import ccentity.entity
import ccentity.entity.ttypes

import proxy
import facade_request


# 配置
class PlayingCell:
  def __init__(self):
    self.id_ = 0
    self.group_ = 0
    self.depend_id_ = 0
    self.camp_type_ = ccentity.playing.ttypes.PlayingCampType.MAX
    self.map_ = 0
    self.min_level_ = 0
    self.max_level_ = 0
    self.period_type_ = ccentity.playing.ttypes.PlayingPeriodType.MAX
    self.period_count_ = 0
    self.camp_number_ = 0
    self.min_actor_ = 0
    self.max_actor_ = 0
    self.position_enter_x_ = 0
    self.position_enter_y_ = 0
    self.position_exit_map_ = 0
    self.position_exit_x_ = 0
    self.position_exit_y_ = 0
    self.auto_type_ = ccentity.playing.ttypes.PlayingAutoType.MAX
    self.spend_freedom_dollars_ = 0
    self.revive_ = 0
    self.finish_time_ = 0
    self.dead_count_ = 0
    self.gold_ = 0
    self.exp_ = 0
    self.nimbus_ = 0

    self.awards_ = []
    self.draw_items_ = []
    self.first_pass_awards_ = []
    self.paid_award_period_count_ = 0
    self.paid_awards_ = []
    self.paid_award_spend_freedom_dollars_ = 0

    self.monsters_ = {}
    self.monster_order_ = []
    self.monster_number_order_ = []
    self.pass_boss_ = 0;


class Config:
  PLAYINGS_ = {}

  @staticmethod
  def get(id):
    if id in Config.PLAYINGS_:
      return Config.PLAYINGS_[id]
    proxy.Logging.error("PLAYINGS_.size()=%d id=%d" % (len(Config.PLAYINGS_), id))
    return None

  @staticmethod
  def parse(file_string):
    f = file(file_string, 'r')
    input_xml_string = f.read()
    f.close()
    doc = xml.dom.minidom.parseString(input_xml_string)
    if doc == None:
      proxy.Logging.error("[playing_config] parse xml(%s) error" % file)
      return False
    root = doc.documentElement
    if root == None:
      proxy.Logging.error("[playing_config] root == None")
      return False
    for cell_node in root.getElementsByTagName("cell"):
      playing_cell = PlayingCell()
      playing_cell.id_ = int(cell_node.getAttribute("id"))
      if playing_cell.id_ in Config.PLAYINGS_:
        proxy.Logging.error("[playing_config] id(%d) reduplicate error" % playing_cell.id_)
        return False
      playing_cell.group_ = int(cell_node.getAttribute("group"))
      playing_cell.depend_id_ = int(cell_node.getAttribute("depend"))
      playing_cell.camp_type_ = int(cell_node.getAttribute("camp_type"))
      playing_cell.map_ = int(cell_node.getAttribute("map"))
      playing_cell.min_level_ = int(cell_node.getAttribute("min_level"))
      playing_cell.max_level_ = int(cell_node.getAttribute("max_level"))
      playing_cell.period_type_ = int(cell_node.getAttribute("period_type"))
      playing_cell.period_count_ = int(cell_node.getAttribute("period_count"))
      playing_cell.camp_number_ = int(cell_node.getAttribute("camp_number"))
      playing_cell.min_actor_ = int(cell_node.getAttribute("min_actor"))
      playing_cell.max_actor_ = int(cell_node.getAttribute("max_actor"))
      # 进入坐标
      position_enter_str = cell_node.getAttribute("position_enter")
      position_enter_list = position_enter_str.split(":")
      if len(position_enter_list) != 2:
        proxy.Logging.error("[playing_config] position_enter error")
        return False
      playing_cell.position_enter_x_ = int(position_enter_list[0])
      playing_cell.position_enter_y_ = int(position_enter_list[1])
      # 退出坐标
      position_exit_str = cell_node.getAttribute("position_exit")
      position_exit_list = position_exit_str.split(":")
      if len(position_exit_list) != 3:
        proxy.Logging.error("[playing_config] position_exit error")
        return False
      playing_cell.position_exit_map_ = int(position_exit_list[0])
      playing_cell.position_exit_x_ = int(position_exit_list[1])
      playing_cell.position_exit_y_ = int(position_exit_list[2])

      playing_cell.auto_type_ = int(cell_node.getAttribute("auto_type"))
      playing_cell.spend_freedom_dollars_ = int(cell_node.getAttribute("spend_freedom_dollars"))
      playing_cell.revive_ = int(cell_node.getAttribute("revive"))
      playing_cell.finish_time_ = int(cell_node.getAttribute("finish_time"))
      playing_cell.dead_count_ = int(cell_node.getAttribute("dead_count"))

      # 奖励
      awards_str = cell_node.getAttribute("awards")
      awards_list = awards_str.split("+")
      for i in range(0, len(awards_list)):
        item_str = awards_list[i]
        item_info = item_str.split(":")
        if len(item_info) == 3:
          playing_cell.awards_.append(ccentity.playing.ttypes.PlayingAwardField(\
                int(item_info[0]), int((item_info[1])), int(item_info[2])))

      # 抽奖奖励
      draw_items_str = cell_node.getAttribute("draw_items")
      draw_items_list = draw_items_str.split("+")
      for i in range(0, len(draw_items_list)):
        item_str = draw_items_list[i]
        item_info = item_str.split(":")
        if len(item_info) == 3:
          playing_cell.draw_items_.append(ccentity.playing.ttypes.PlayingAwardField(\
                int(item_info[0]), int((item_info[1])), int(item_info[2])))
          # proxy.Logging.info("draw_items(%d,%d,%d)" % (int(item_info[0]), int(item_info[1]), int(item_info[2])))

      # 首通奖励
      first_pass_awards_str = cell_node.getAttribute("first_pass_awards")
      first_pass_awards_list = first_pass_awards_str.split("+")
      for i in range(0, len(first_pass_awards_list)):
        item_str = first_pass_awards_list[i]
        item_info = item_str.split(":")
        if len(item_info) == 3:
          playing_cell.first_pass_awards_.append(ccentity.playing.ttypes.PlayingAwardField(\
                int(item_info[0]), int((item_info[1])), int(item_info[2])))

      # 购买奖励
      playing_cell.paid_award_period_count_ = int(cell_node.getAttribute("buy_award_period_count"))
      playing_cell.paid_award_spend_freedom_dollars_ =\
          int(cell_node.getAttribute("buy_award_spend_freedom_dollars"))
      paid_awards_str = cell_node.getAttribute("buy_award")
      paid_awards_list = paid_awards_str.split("+")
      for i in range(0, len(paid_awards_list)):
        item_str = paid_awards_list[i]
        item_info = item_str.split(":")
        if len(item_info) == 3:
          playing_cell.paid_awards_.append(ccentity.playing.ttypes.PlayingAwardField(\
                int(item_info[0]), int((item_info[1])), int(item_info[2])))

      # 怪物列表
      monsters_str = str(cell_node.getAttribute("monsters"))
      monsters_list = monsters_str.split("+")
      if len(monsters_list) > 0:
        for i in range(0, len(monsters_list)):
          monster_str = monsters_list[i]
          monster_pair = monster_str.split(":")
          if len(monster_pair) == 2:
            playing_cell.monster_order_.append(int(monster_pair[0]))
            playing_cell.monster_number_order_.append([int(monster_pair[0]), int(monster_pair[1])])
            playing_cell.monsters_[int(monster_pair[0])] = int(monster_pair[1])
            if i == (len(monsters_list) - 1):
              playing_cell.pass_boss_ = int(monster_pair[0])
              # proxy.Logging.info("pass_boss_=%d" % playing_cell.pass_boss_)

      Config.PLAYINGS_[playing_cell.id_] = playing_cell
      # proxy.Logging.info("playing=%d" % playing_cell.id_)

