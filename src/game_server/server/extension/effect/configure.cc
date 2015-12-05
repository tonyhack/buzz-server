//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-02 23:45:44.
// File name: configure.cc
//
// Description:
// Define class Configure.
//

#include "game_server/server/extension/effect/configure.h"

#include "core/tinyxml/tinyxml.h"
#include "game_server/server/extension/effect/effect_type.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace effect {

bool Configure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    global::LogError("%s:%d (%s) [效果配置] 加载配置文件(%s)出错",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 configure 结点标签
  TiXmlElement *configure_node = doc.FirstChildElement("data");
  if(configure_node == NULL) {
    global::LogError("%s:%d (%s) [效果配置] 找不到结点 [data]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 effect 结点标签
  TiXmlElement *effect_node = configure_node->FirstChildElement("cell");
  while(effect_node) {
    core::uint32 id = 0;
    // 效果ID
    if(effect_node->Attribute("id", (int *)(&id)) == NULL) {
      global::LogError("%s:%d (%s) [效果配置] 找不到属性 [effect-id]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    // 是否重复
    if(this->effects_.find(id) != this->effects_.end()) {
      global::LogError("%s:%d (%s) [效果配置] 效果ID(%u) 配置重复，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, id, file.c_str());
      return false;
    }
    EffectConfigure effect;
    // 效果类型
    if(effect_node->Attribute("type", (int *)(&effect.type_)) == NULL) {
      global::LogError("%s:%d (%s) [效果配置] 找不到属性 [effect-type]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    // 效果类型是否合法
    if(effect.type_ < EffectType::EFFECT_MIN || effect.type_ >= EffectType::EFFECT_MAX) {
      global::LogError("%s:%d (%s) [效果配置] 效果(%u) 类型(%d)错误，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, id, effect.type_, file.c_str());
      return false;
    }
    // 效果参数
    if(effect_node->Attribute("param1", (int *)(&effect.param1_)) == NULL) {
      global::LogError("%s:%d (%s) [效果配置] 找不到属性 [effect-param1]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(effect_node->Attribute("param2", (int *)(&effect.param2_)) == NULL) {
      global::LogError("%s:%d (%s) [效果配置] 找不到属性 [effect-param2]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    if(effect_node->Attribute("param3", (int *)(&effect.param3_)) == NULL) {
      global::LogError("%s:%d (%s) [效果配置] 找不到属性 [effect-param3]，文件(%s)",
          __FILE__, __LINE__, __FUNCTION__, file.c_str());
      return false;
    }
    // 插入表中
    this->effects_.insert(std::make_pair(id, effect));
    // 下一个结点
    effect_node = effect_node->NextSiblingElement("cell");
  }

  return true;
}

const EffectConfigure *Configure::GetEffectConfigure(core::uint32 effect_id) {
  EffectHashmap::const_iterator iterator = this->effects_.find(effect_id);
  if(iterator != this->effects_.end()) {
    return &iterator->second;
  }
  return NULL;
}

}  // namespace effect

}  // namespace server

}  // namespace game

