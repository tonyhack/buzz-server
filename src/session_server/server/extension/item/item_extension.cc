#include "session_server/server/extension/item/item_extension.h"

#include "global/common_functions.h"
#include "gateway_server/protocol/gateway_item_protocol_types.h"
#include "gateway_server/protocol/gateway_mall_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "session_server/protocol/game_item_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "session_server/server/extension/item/actor_login_event_handler.h"
#include "session_server/server/world_data_manager.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/session_server.h"

namespace session {

namespace server {

namespace item {

ItemExtension::ItemExtension() {}
ItemExtension::~ItemExtension() {}

bool ItemExtension::Initialize() {
  if(game_message_handler_.Initialize() == false) {
    LOG_ERROR("Initialize GameMessageHandler failed.");
    return false;
  }
  if(actor_login_.Initialize() == false) {
    LOG_ERROR("初始化 ActorLoginEventHandler 失败");
    return false;
  }
  if(system_event_.Initialize() == false) {
    LOG_ERROR("初始化 SystemEventHandler failed.");
    return false;
  }
  if(request_item_.Initialize() == false) {
    LOG_ERROR("初始化 RequestItemHandler failed.");
    return false;;
  }

  std::vector<std::string> infos;
  std::string treasure_data = 
    WorldDataManager::GetInstance()->GetWorldData(WorldDataType::ITEM_TREASURE_HUNT);
  if(treasure_data !="") {
    // 分割
    global::StringSplit(treasure_data.c_str(), ":", infos);
    for(size_t i = 0; i < infos.size(); ++i) {
      this->world_treasure_vec_.push_back(infos[i]);
    }
  }

  // 分割商城、藏宝阁数据
  infos.clear();
  std::string mall_config = 
    WorldDataManager::GetInstance()->GetWorldData(WorldDataType::MALL_AND_TREASURE);
  global::StringSplit(mall_config.c_str(), ":", infos);
  if(infos.size() == 0) {
    LOG_ERROR("无商城、藏宝阁配置");
    return false;
  }
  std::vector<core::int64> results;
  for(size_t i = 0; i < infos.size(); ++i) {
    global::StringSplit(infos[i].c_str(), ",", results);
    if(results.size() != 14) {
      LOG_ERROR("配置错误(%d)", results.size());
      return false;
    }
    entity::MallTreasureCell cell;
    cell.__set_tab_((int8_t)results[0]);
    cell.__set_shop_type_((int8_t)results[1]);
    cell.__set_location_((int8_t)results[2]);
    cell.__set_id_((core::int32)results[3]);
    cell.__set_hot_((bool)results[4]);
    cell.__set_dollars_((core::int32)results[5]);
    cell.__set_spend_type_((int8_t)results[6]);
    cell.__set_up_start_time_(global::ToString(results[7]));
    cell.__set_up_end_time_(global::ToString(results[8]));
    cell.__set_dollars_start_time_(global::ToString(results[9]));
    cell.__set_dollars_end_time_(global::ToString(results[10]));
    cell.__set_limit_buy_num_((core::int32)results[11]);
    cell.__set_sell_price_((core::int32)results[12]);
    cell.__set_bind_((int8_t)results[13]);
    mall_treasure_vec_.push_back(cell);
    results.clear();
  }
  return true;
}

void ItemExtension::Finalize() {
  this->game_message_handler_.Finalize();
  this->actor_login_.Finalize();
  this->system_event_.Finalize();
  this->world_treasure_vec_.clear();
  this->request_item_.Finalize();
}

void ItemExtension::AddTreasureInfo(std::string &info) {
  if(this->world_treasure_vec_.size() >= 5) {
    this->world_treasure_vec_.erase(this->world_treasure_vec_.begin());
  }
  this->world_treasure_vec_.push_back(info);
  // 广播所有玩家
  gateway::protocol::MessageItemTreasureHuntNotice message;
  message.notices_.push_back((this->world_treasure_vec_[this->world_treasure_vec_.size() - 1]));
  SessionServerSingleton::GetInstance().BroadcastMessage(message,
      gateway::protocol::MessageType::MESSAGE_ITEM_TREASURE_HUNT_NOTICE);

  std::string temp_save_data;
  if(this->world_treasure_vec_.size() >= 1) {
    temp_save_data.append(this->world_treasure_vec_[0]);
  }
  for(size_t i = 1; i < this->world_treasure_vec_.size(); ++i) {
    temp_save_data.append(":");
    temp_save_data.append(this->world_treasure_vec_[i]);
  }
  WorldDataManager::GetInstance()->SaveWorldData(WorldDataType::ITEM_TREASURE_HUNT, temp_save_data);
}

void ItemExtension::SendTreasureInfo(core::uint64 actor) {
  SessionActor *session_actor = SessionActorManager::GetInstance()->GetActor(actor);
  if(session_actor == NULL) {
    LOG_ERROR("玩家没有找到");
    return ;
  }
  gateway::protocol::MessageItemTreasureHuntNotice message;
  for(size_t i = 0; i < this->world_treasure_vec_.size(); ++i) {
    message.notices_.push_back(this->world_treasure_vec_[i]);
  }
  session_actor->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_ITEM_TREASURE_HUNT_NOTICE);
}

bool ItemExtension::PushMallTreasureData(const entity::MallTreasureCell &cell) {
  for(size_t i = 0; i < this->mall_treasure_vec_.size(); ++i) {
    if(cell.location_ == this->mall_treasure_vec_[i].location_ &&
        cell.shop_type_ == this->mall_treasure_vec_[i].shop_type_ &&
        cell.tab_ == this->mall_treasure_vec_[i].tab_) {
      LOG_ERROR("数据重复");
      return false;
    }
  }
  this->mall_treasure_vec_.push_back(cell);
  return true;
}

void ItemExtension::SynchronizeMallConfig(core::int32 game_id) {
  // 同步数据
  SessionTerminal *terminal = SessionServerSingleton::GetInstance().GetTerminal(
      global::protocol::ServerType::GAME_SERVER, game_id);
  session::protocol::MessageItemMallConfigSynchronize message;
  message.mall_config_ = mall_treasure_vec_;
  terminal->SendGameMessage(message,
      session::protocol::GameMessageType::MESSAGE_ITEM_MALL_CONFIG_SYNCHRONIZE);
}

void ItemExtension::SynchronizeMallTreasureData() {
  std::string temp_data;
  if(this->mall_treasure_vec_.size() >= 1) {
    temp_data.append(global::ToString(mall_treasure_vec_[0].tab_));
    temp_data.append(",");
    temp_data.append(global::ToString(mall_treasure_vec_[0].shop_type_));
    temp_data.append(",");
    temp_data.append(global::ToString(mall_treasure_vec_[0].location_));
    temp_data.append(",");
    temp_data.append(global::ToString(mall_treasure_vec_[0].id_));
    temp_data.append(",");
    temp_data.append(global::ToString(mall_treasure_vec_[0].hot_));
    temp_data.append(",");
    temp_data.append(global::ToString(mall_treasure_vec_[0].dollars_));
    temp_data.append(",");
    temp_data.append(global::ToString(mall_treasure_vec_[0].spend_type_));
    temp_data.append(",");
    temp_data.append(mall_treasure_vec_[0].up_start_time_);
    temp_data.append(",");
    temp_data.append(mall_treasure_vec_[0].up_end_time_);
    temp_data.append(",");
    temp_data.append(mall_treasure_vec_[0].dollars_start_time_);
    temp_data.append(",");
    temp_data.append(mall_treasure_vec_[0].dollars_end_time_);
    temp_data.append(",");
    temp_data.append(global::ToString(mall_treasure_vec_[0].limit_buy_num_));
    temp_data.append(",");
    temp_data.append(global::ToString(mall_treasure_vec_[0].sell_price_));
    temp_data.append(",");
    temp_data.append(global::ToString(mall_treasure_vec_[0].bind_));
  }
  for(size_t i = 1; i < this->mall_treasure_vec_.size(); ++i) {
    temp_data.append(":");
    temp_data.append(global::ToString(mall_treasure_vec_[0].tab_));
    temp_data.append(",");
    temp_data.append(global::ToString(mall_treasure_vec_[0].shop_type_));
    temp_data.append(",");
    temp_data.append(global::ToString(mall_treasure_vec_[0].location_));
    temp_data.append(",");
    temp_data.append(global::ToString(mall_treasure_vec_[0].id_));
    temp_data.append(",");
    temp_data.append(global::ToString(mall_treasure_vec_[0].hot_));
    temp_data.append(",");
    temp_data.append(global::ToString(mall_treasure_vec_[0].dollars_));
    temp_data.append(",");
    temp_data.append(global::ToString(mall_treasure_vec_[0].spend_type_));
    temp_data.append(",");
    temp_data.append(mall_treasure_vec_[0].up_start_time_);
    temp_data.append(",");
    temp_data.append(mall_treasure_vec_[0].up_end_time_);
    temp_data.append(",");
    temp_data.append(mall_treasure_vec_[0].dollars_start_time_);
    temp_data.append(",");
    temp_data.append(mall_treasure_vec_[0].dollars_end_time_);
    temp_data.append(",");
    temp_data.append(global::ToString(mall_treasure_vec_[0].limit_buy_num_));
    temp_data.append(",");
    temp_data.append(global::ToString(mall_treasure_vec_[0].sell_price_));
    temp_data.append(",");
    temp_data.append(global::ToString(mall_treasure_vec_[0].bind_));
  }
  WorldDataManager::GetInstance()->SaveWorldData(WorldDataType::MALL_AND_TREASURE, temp_data);

  // 推送数据
  session::protocol::MessageItemMallConfigSynchronize message;
  message.mall_config_ = mall_treasure_vec_;
  SessionServerSingleton::GetInstance().BroadcastGameMessage(message,
      session::protocol::GameMessageType::MESSAGE_ITEM_MALL_CONFIG_SYNCHRONIZE);

  // 推送数据给玩家
  gateway::protocol::MessageMallSynchronize message_actor;
  message_actor.cells_ = mall_treasure_vec_;
  SessionServerSingleton::GetInstance().BroadcastMessage(message_actor,
      gateway::protocol::MessageType::MESSAGE_MALL_SYNCHRONIZE);
}

void ItemExtension::SynchronizeMallTreasureDataActor(core::uint64 actor) {
  // 从玩家管理器中获取玩家对象
  SessionActor *session_actor = SessionActorManager::GetInstance()->GetActor(actor);
  if (NULL == session_actor) {
    LOG_ERROR("Get SessionActor(%lu) from SessionActorManager failed.", actor);
    return;
  }
  gateway::protocol::MessageMallSynchronize message;
  message.cells_ = mall_treasure_vec_;
  session_actor->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_MALL_SYNCHRONIZE);
}

void ItemExtension::ClearMallConfigure() {
  this->mall_treasure_vec_.clear();
}

void ItemExtension::Start() {}

void ItemExtension::Stop() {}

} // namespace item

} // namepsace server

} // namespace session

