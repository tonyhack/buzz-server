//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-03-12 21:31:27.
// File name: attr_event_handler.cc
//
// Description:
// Define class AttrEventHandler.
//

#include "game_server/server/extension/soul/attr_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_attr_event_types.h"
#include "game_server/server/extension/soul/configure.h"
#include "game_server/server/extension/soul/soul.h"
#include "game_server/server/extension/soul/soul_actor.h"
#include "game_server/server/extension/soul/soul_actor_manager.h"
#include "game_server/server/extension/soul/soul_configure.h"

namespace game {

namespace server {

namespace soul {

AttrEventHandler::AttrEventHandler() {}
AttrEventHandler::~AttrEventHandler() {}

bool AttrEventHandler::Initialize() {
  this->conn_spirit_upgrade_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ATTR_SPIRIT_UPGRADE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&AttrEventHandler::OnEventAttrSpiritUpgrade,
        this, _1, _2, _3, _4));

  this->conn_sword_upgrade_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ATTR_SPIRIT_UPGRADE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&AttrEventHandler::OnEventAttrSwordUpgrade,
        this, _1, _2, _3, _4));

  this->conn_noble_upgrade_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ATTR_SPIRIT_UPGRADE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&AttrEventHandler::OnEventAttrNobleUpgrade,
        this, _1, _2, _3, _4));

  return true;
}

void AttrEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_spirit_upgrade_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_sword_upgrade_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_noble_upgrade_);
}

void AttrEventHandler::OnEventAttrSpiritUpgrade(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if (message == NULL || size != sizeof(event::EventAttrSpiritUpgrade)) {
    LOG_ERROR("参数错误");
    return;
  }

  event::EventAttrSpiritUpgrade *event = (event::EventAttrSpiritUpgrade *)message;

  SoulActor *soul_actor = SoulActorManager::GetInstance()->Get(event->actor_id_);
  if (soul_actor == NULL) {
    LOG_ERROR("获取 SoulActor(%lu) 失败", event->actor_id_);
    return;
  }

  const SoulSetting *soul_setting = soul_actor->GetCurrentSetting();
  if (soul_setting == NULL) {
    LOG_ERROR("获取 SoulActor(%lu) 当前的 SoulSetting 失败", event->actor_id_);
    return;
  }

  const SoulSetting::SettingVector &settings = soul_setting->GetSettings();
  SoulSetting::SettingVector::const_iterator iterator = settings.begin();
  for (; iterator != settings.end(); ++iterator) {
    if (*iterator <= 0) {
      continue;
    }
    Soul *soul = soul_actor->GetSoul(*iterator);
    if (soul == NULL) {
      LOG_ERROR("获取 SoulActor(%lu) 的 Soul(%lu) 失败", event->actor_id_, *iterator);
      return;
    }
    const SoulCell *cell =
      Configure::GetInstance()->GetSoulCell(soul->GetID());
    if (cell == NULL) {
      LOG_ERROR("获取 SoulCell(%d,%d) 失败", soul->GetID(), soul->GetLevel());
      return;
    }
    // 开启精灵效果
    for (SoulCell::EffectMap::const_iterator iterator = cell->spirit_effects_.begin();
        iterator != cell->spirit_effects_.end(); ++iterator) {
      if (iterator->first > event->old_level_ && iterator->first <= event->new_level_) {
        for (size_t pos = 0; pos < iterator->second.size(); ++pos) {
          soul->StartStandEffect(iterator->second[pos]);
        }
      }
    }
  }
}

void AttrEventHandler::OnEventAttrSwordUpgrade(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if (message == NULL || size != sizeof(event::EventAttrSwordUpgrade)) {
    LOG_ERROR("参数错误");
    return;
  }

  event::EventAttrSwordUpgrade *event = (event::EventAttrSwordUpgrade *)message;

  SoulActor *soul_actor = SoulActorManager::GetInstance()->Get(event->actor_id_);
  if (soul_actor == NULL) {
    LOG_ERROR("获取 SoulActor(%lu) 失败", event->actor_id_);
    return;
  }

  const SoulSetting *soul_setting = soul_actor->GetCurrentSetting();
  if (soul_setting == NULL) {
    LOG_ERROR("获取 SoulActor(%lu) 当前的 SoulSetting 失败", event->actor_id_);
    return;
  }

  const SoulSetting::SettingVector &settings = soul_setting->GetSettings();
  SoulSetting::SettingVector::const_iterator iterator = settings.begin();
  for (; iterator != settings.end(); ++iterator) {
    if (*iterator <= 0) {
      continue;
    }
    Soul *soul = soul_actor->GetSoul(*iterator);
    if (soul == NULL) {
      LOG_ERROR("获取 SoulActor(%lu) 的 Soul(%lu) 失败", event->actor_id_, *iterator);
      return;
    }
    const SoulCell *cell =
      Configure::GetInstance()->GetSoulCell(soul->GetID());
    if (cell == NULL) {
      LOG_ERROR("获取 SoulCell(%d,%d) 失败", soul->GetID(), soul->GetLevel());
      return;
    }
    // 开启魔剑效果
    for (SoulCell::EffectMap::const_iterator iterator = cell->sword_effects_.begin();
        iterator != cell->sword_effects_.end(); ++iterator) {
      if (iterator->first > event->old_step_ && iterator->first <= event->new_step_) {
        for (size_t pos = 0; pos < iterator->second.size(); ++pos) {
          soul->StartStandEffect(iterator->second[pos]);
        }
      }
    }
  }
}

void AttrEventHandler::OnEventAttrNobleUpgrade(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if (message == NULL || size != sizeof(event::EventAttrNobleUpgrade)) {
    LOG_ERROR("参数错误");
    return;
  }

  event::EventAttrNobleUpgrade *event = (event::EventAttrNobleUpgrade *)message;

  SoulActor *soul_actor = SoulActorManager::GetInstance()->Get(event->actor_id_);
  if (soul_actor == NULL) {
    LOG_ERROR("获取 SoulActor(%lu) 失败", event->actor_id_);
    return;
  }

  const SoulSetting *soul_setting = soul_actor->GetCurrentSetting();
  if (soul_setting == NULL) {
    LOG_ERROR("获取 SoulActor(%lu) 当前的 SoulSetting 失败", event->actor_id_);
    return;
  }

  const SoulSetting::SettingVector &settings = soul_setting->GetSettings();
  SoulSetting::SettingVector::const_iterator iterator = settings.begin();
  for (; iterator != settings.end(); ++iterator) {
    if (*iterator <= 0) {
      continue;
    }
    Soul *soul = soul_actor->GetSoul(*iterator);
    if (soul == NULL) {
      LOG_ERROR("获取 SoulActor(%lu) 的 Soul(%lu) 失败", event->actor_id_, *iterator);
      return;
    }
    const SoulCell *cell =
      Configure::GetInstance()->GetSoulCell(soul->GetID());
    if (cell == NULL) {
      LOG_ERROR("获取 SoulCell(%d,%d) 失败", soul->GetID(), soul->GetLevel());
      return;
    }
    // 开启贵族效果
    for (SoulCell::EffectMap::const_iterator iterator = cell->sword_effects_.begin();
        iterator != cell->sword_effects_.end(); ++iterator) {
      if (iterator->first > event->old_level_ && iterator->first <= event->new_level_) {
        for (size_t pos = 0; pos < iterator->second.size(); ++pos) {
          soul->StartStandEffect(iterator->second[pos]);
        }
      }
    }
  }
}

}  // namespace soul

}  // namespace server

}  // namespace game

