
# coding=utf-8

import sys
sys.path.append('ccentity')
sys.path.append('ccrequest')
sys.path.append('ccvariable')

import __main__

import ccentity
import ccentity.entity
import ccentity.entity.ttypes

import ccrequest
import ccrequest.ttypes
import ccrequest.scene.ttypes

import ccvariable
import ccvariable.ttypes

import variable_table
import proxy
import log

from thrift import TSerialization

# 召唤NPC
# 参数(场景ID, NPC模板ID, 坐标x, 坐标y, 宽, 高)
def summon_npc(scene_id, npc_template, x, y, width = 0, heigth = 0, delay_secs = 0,
    dir = ccentity.direction.ttypes.DirectionType.NONE):
  # 构造消息
  message = ccrequest.scene.ttypes.RequestSceneSummonNpc()
  message.scene_ = scene_id
  message.template_id_ = npc_template
  message.x_ = x
  message.y_ = y
  message.dir_ = dir
  message.width_ = width
  message.heigth_ = heigth
  message.npc_id_ = 0
  message.delay_secs_ = delay_secs
  # 发送请求
  if proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_SCENE_SUMMON_NPC,\
      message) == False:
    return 0
  # 得到结果
  result = ccrequest.scene.ttypes.RequestSceneSummonNpc()
  TSerialization.deserialize(result, proxy.Request.get_result())
  return result.npc_id_


def summon_npcs(scene_id, npc_template, num, x, y, width = 0, heigth = 0, delay_secs = 0,
    dir = ccentity.direction.ttypes.DirectionType.NONE):
  for i in range(0, num):
    summon_npc(scene_id, npc_template, x, y, width, heigth, delay_secs, dir)


# 删除NPC
# 参数(NPC唯一ID)
def remove_npc(npc, delay_secs = 0):
  # 构造消息
  message = ccrequest.scene.ttypes.RequestSceneClearNpc()
  message.id_ = npc
  message.delay_secs_ = delay_secs
  # 发送请求
  return proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_SCENE_CLEAR_NPC,\
      message)


# 清除场景内的NPC
def clear_scene_npcs(scene_id, delay_secs = 0, force_clear_reborn = False):
  message = ccrequest.scene.ttypes.RequestSceneClearAllNpc()
  message.scene_ = scene_id
  message.delay_secs_ = delay_secs
  message.force_clear_reborn_ = force_clear_reborn
  if proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_SCENE_CLEAR_ALL_NPC,\
        message) == False:
    return False
  return True


# 增加临时技能
# 参数(实体类型, 实体ID, 技能ID, 技能等级)
def add_temp_skill(entity_type, entity_id, skill_id, skill_level):
  # 构造消息
  message = ccrequest.skill.ttypes.RequestSkillAddTempSkill()
  message.type_ = entity_type
  message.id_ = entity_id
  message.skill_id_ = skill_id
  message.skill_level_ = skill_level
  message.result_ = False
  # 发送消息
  if proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_SKILL_ADD_TEMP_SKILL,
      message) == False:
    return False
  # 得到结果
  result = ccrequest.skill.ttypes.RequestSkillAddTempSkill()
  TSerialization.deserialize(result, proxy.Request.get_result())
  return result.result_


# 删除临时技能
# 参数(实体类型, 实体ID, 技能ID)
def remove_temp_skill(entity_type, entity_id, skill_id):
  # 构造消息
  message = ccrequest.skill.ttypes.RequestSkillRemoveTempSkill()
  message.type_ = entity_type
  message.id_ = entity_id
  message.skill_id_ = skill_id
  message.result_ = False
  # 发送消息
  if proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_SKILL_REMOVE_TEMP_SKILL,
      message) == False:
    return False
  # 得到结果
  result = ccrequest.skill.ttypes.RequestSkillRemoveTempSkill()
  TSerialization.deserialize(result, proxy.Request.get_result())
  return result.result_


# 改变技能形态
# 参数(实体类型, 实体ID, 技能形态)
def change_skill_form(entity_type, entity_id, skill_form):
  # 构造消息
  message = ccrequest.skill.ttypes.RequestSkillChangeSkillForm()
  message.type_ = entity_type
  message.id_ = entity_id
  message.form_  = skill_form
  message.result_ = False
  # 发送消息
  if proxy.Request.request(ccrequest.ttypes.RequestType.REQUEST_SKILL_CHANGE_SKILL_FORM,
      message) == False:
    return False
  # 得到结果
  result = ccrequest.skill.ttypes.RequestSkillChangeSkillForm()
  TSerialization.deserialize(result, proxy.Request.get_result())
  return result.result_

