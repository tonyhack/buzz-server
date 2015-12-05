//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-11 19:22:42.
// File name: mall_message_handler.cc
//
// Description:
//

#include "game_server/server/extension/item/mall_message_handler.h"

#include <boost/bind.hpp>
#include "entity/mall_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/item/configure.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/item_actor_manager.h"
#include "game_server/server/extension/item/item_builder.h"
#include "game_server/server/extension/item/item_configure.h"
#include "game_server/server/extension/item/item.h"
#include "game_server/server/extension/item/item_types.h"
#include "game_server/server/extension/item/load_mall_configure.h"
#include "game_server/server/global_time_tick.h"
#include "game_server/server/configure.h"
#include "game_server/server/load_functionality_configure.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_mall_protocol_types.h"
#include "global/global_packet.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace item {

MallMessageHandler::MallMessageHandler() {}
MallMessageHandler::~MallMessageHandler() {}

bool MallMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(gateway::protocol::MessageType::MESSAGE_MALL_BUY_REQUEST,
     boost::bind(&MallMessageHandler::OnMessageMallBuyRequest, this, _1, _2, _3));
  return true;
}

void MallMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
     gateway::protocol::MessageType::MESSAGE_MALL_BUY_REQUEST);
}

void MallMessageHandler::OnMessageMallBuyRequest(core::uint64 id, 
  const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  // 得到道具玩家对象
  ItemActor *actor = ItemActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    LOG_ERROR("获取玩家(%lu) 失败", id);
    return ;
  }

  // 得到 GameActor 玩家对象
  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("获取玩家(%s) GameActor 对象失败", id);
    return ;
  }

  // 解析消息
  gateway::protocol::MessageMallBuyRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息 MessageMallBuyRequest 失败");
    return ;
  }

  LOG_INFO("Recevie MESSAGE_MALL_BUY_REQUEST from actor(%lu, %s), num_(%d), pos_(%d), shop_type_(%d), tab_(%d), use_type_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.num_, message.pos_, message.shop_type_, message.tab_, message.use_type_);

  // 验证功能是否开启
  if(game_actor->CheckFunctionalityState(entity::FunctionalityType::TREASURE) == false) {
    LOG_ERROR("藏宝阁功能未开启，无法操作");
    return ;
  }

  // 得到配置
  MallConfigureKey key(message.tab_, message.shop_type_, message.pos_);
  const entity::MallTreasureCell *cell = Configure::GetInstance()->GetMallConfigure().GetMallCell(key);
  if(cell == NULL) {
    LOG_ERROR("配置没有找到tab(%d)shop_type(%d)pos(%d)", message.tab_, message.shop_type_, message.pos_);
    return ;
  }

  // 检查时间
  if(this->CheckBuyItemTime(cell) == false) {
    LOG_ERROR("商品已经下架");
    return ;
  }

  // 检查打折时间
  bool des = this->CheckDesPrice(cell);
  core::int32 price = 0; 
  
  // 检查藏宝阁限制次数
  if(message.shop_type_ == ::entity::MallTreasureType:: TREASURE) {
    if(actor->GetBuyNum(cell->id_) >= cell->limit_buy_num_) {
      LOG_ERROR("道具(%d)已经超过购买次数了，不能再购买", cell->id_);
      return ;
    }
  }

  const ItemCell *item_cell = Configure::GetInstance()->GetItem(cell->id_);
  if(item_cell == NULL) {
    LOG_ERROR("道具配置没有找到(%d)", cell->id_);
    return ;
  }

  // 检查消费
  price = cell->sell_price_ * (des == true ? cell->dollars_ : 100) * message.num_/100;
  if(cell->spend_type_ == (core::int8)entity::MallUseType::USE_FREEDOM_RESTRICT) {
    price = item_cell->buy_gold_ * (des == true ? cell->dollars_ : 100) * message.num_/100;;
    if(message.use_type_ == (core::int8)entity::MallUseType::USE_FREEDOM_DOLLARS) {
      if(game_actor->GetResource(entity::ResourceID::FREEDOM_DOLLARS) < price) {
        LOG_ERROR("价格不足(%d)", price);
        return ;
      }
    } else {
      price = item_cell->buy_lockedgold_ * (des == true ? cell->dollars_ : 100) * message.num_/100;
      if(game_actor->GetResource(entity::ResourceID::RESTRICT_DOLLARS) < price) {
        LOG_ERROR("价格不足(%d)", price);
        return ;
      }
    }
  } else if(cell->spend_type_ == (core::int8)entity::MallUseType::USE_FREEDOM_DOLLARS) {
    price = item_cell->buy_gold_ * (des == true ? cell->dollars_ : 100) * message.num_/100;
    if(message.use_type_ != (core::int8)entity::MallUseType::USE_FREEDOM_DOLLARS ||
        game_actor->GetResource(entity::ResourceID::FREEDOM_DOLLARS) < price) {
      LOG_ERROR("传入的类型不对");
      return ;
    }
  } else if(cell->spend_type_ == (core::int8)entity::MallUseType::USE_RESTRICT_DOLLARS) {
    price = item_cell->buy_lockedgold_ * (des == true ? cell->dollars_ : 100) * message.num_/100;
    if(message.use_type_ != (core::int8)entity::MallUseType::USE_RESTRICT_DOLLARS ||
        game_actor->GetResource(entity::ResourceID::RESTRICT_DOLLARS) < price) {
      LOG_ERROR("传入的类型不正确");
      return ;
    }
  } else if(cell->spend_type_ == (core::int8)entity::MallUseType::USE_ARENA_INTEGRAL) {
    if(message.use_type_ != (core::int8)entity::MallUseType::USE_ARENA_INTEGRAL ||
        game_actor->GetResource(entity::ResourceID::ARENA_INTEGRAL) < price) {
      LOG_ERROR("传入的类型不正确");
      return ;
    }
  } else if(cell->spend_type_ == (core::int8)entity::MallUseType::USE_JUSTICE) {
    if(message.use_type_ != (core::int8)entity::MallUseType::USE_JUSTICE ||
        game_actor->GetResource(entity::ResourceID::JUSTICE) < price) {
      LOG_ERROR("传入的类型不正确");
      return ;
    }
  } else if(cell->spend_type_ == (core::int8)entity::MallUseType::USE_MIRACLE_INTEGRAL) {
    if(message.use_type_ != (core::int8)entity::MallUseType::USE_MIRACLE_INTEGRAL ||
        game_actor->GetResource(entity::ResourceID::MIRACLE_INTEGRAL) < price) {
      LOG_ERROR("传入的类型不正确");
      return ;
    }
  } else if(cell->spend_type_ == (core::int8)entity::MallUseType::USE_LEGEND_INTEGRAL) {
    if(message.use_type_ != (core::int8)entity::MallUseType::USE_LEGEND_INTEGRAL ||
        game_actor->GetResource(entity::ResourceID::LEGEND_INTEGRAL) < price) {
      LOG_ERROR("传入的类型不正确");
      return ;
    }
  } else {
    LOG_ERROR("传入的类型(%d)不正切", message.use_type_);
    return ;
  }

  // 可以购买商品
  bool is_open_funcationality = false;
  const MedicineCell* medicine_cell = 
    Configure::GetInstance()->GetMedicineConfigure(cell->id_);
  if(medicine_cell != NULL && medicine_cell->func_type_ == UseFuncType::OPEN_FUNCTIONALITY) {
    // 开启功能
    entity::FunctionalityType::type func_type = game::server::Configure::GetInstance()->GetServerDefineByFunctionality(
        medicine_cell->func_params_[0].param2_);
    if(game_actor->CheckFunctionalityState(func_type) == true) {
      LOG_ERROR("功能开启失败(%d)", func_type);
      return ;
    }
    game_actor->SetFunctionalityState(func_type);
  }
  if(is_open_funcationality == false) {
    PacketContainer *pack = (PacketContainer *)actor->GetContainer(
        entity::ItemContainerType::PACKET, entity::VocationType::MAX);
    if(pack == NULL) {
      LOG_ERROR("玩家背包没有找到");
      return ;
    }
    if(pack->AddItems(cell->id_, message.num_, cell->bind_ ==
          entity::BindRequireType::NOT_BIND ? false : true, 0, NULL) == false) {
      LOG_ERROR("道具(%d)数目(%d)添加背包失败", cell->id_, cell->bind_);
      return ;
    }
  }

  // 扣除费用
  if(message.use_type_ == (core::int8)entity::MallUseType::USE_FREEDOM_DOLLARS) {
    game_actor->AddResource(entity::ResourceID::FREEDOM_DOLLARS, -price);
  } else if(message.use_type_ == (core::int8)entity::MallUseType::USE_RESTRICT_DOLLARS) {
    game_actor->AddResource(entity::ResourceID::RESTRICT_DOLLARS, -price);
  } else if(message.use_type_ == (core::int8)entity::MallUseType::USE_FREEDOM_DOLLARS) {
    game_actor->AddResource(entity::ResourceID::FREEDOM_DOLLARS, -price);
  } else if(message.use_type_ == (core::int8)entity::MallUseType::USE_RESTRICT_DOLLARS) {
    game_actor->AddResource(entity::ResourceID::RESTRICT_DOLLARS, -price);
  } else if(message.use_type_ == (core::int8)entity::MallUseType::USE_ARENA_INTEGRAL) {
    game_actor->AddResource(entity::ResourceID::ARENA_INTEGRAL, -price);
  } else if(message.use_type_ == (core::int8)entity::MallUseType::USE_JUSTICE) {
    game_actor->AddResource(entity::ResourceID::JUSTICE, -price);
  } else if(message.use_type_ == (core::int8)entity::MallUseType::USE_MIRACLE_INTEGRAL) {
    game_actor->AddResource(entity::ResourceID::MIRACLE_INTEGRAL, -price);
  } else if(message.use_type_ == (core::int8)entity::MallUseType::USE_LEGEND_INTEGRAL) {
    game_actor->AddResource(entity::ResourceID::LEGEND_INTEGRAL, -price);
  } else {
    LOG_ERROR("传入的类型(%d)不正切", message.use_type_);
    return ;
  }

  if(message.shop_type_ == ::entity::MallTreasureType:: TREASURE) {
    actor->AddBuyNum(cell->id_, message.num_);
  }

  // 消息回应
  gateway::protocol::MessageMallBuyResponse response;
  response.__set_use_type_(message.use_type_);
  response.__set_id_(cell->id_);
  response.__set_num_(message.num_);
  response.__set_shop_type_(message.shop_type_);
  game_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_MALL_BUY_RESPONSE);
}

bool MallMessageHandler::CheckBuyItemTime(const entity::MallTreasureCell *cell) {
  if(cell == NULL) {
    return false;
  }
  time_t now_time = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  time_t up_time = atol(cell->up_start_time_.c_str());
  time_t down_time = atol(cell->up_end_time_.c_str());
  if(up_time >= now_time && up_time != 0) {
    LOG_ERROR("还没有到上架时间");
    return false;
  }
  if(down_time <= now_time && down_time != 0) {
    LOG_ERROR("已经超过下架时间");
    return false;
  }
  return true;
}

bool MallMessageHandler::CheckDesPrice(const entity::MallTreasureCell *cell) {
  if(cell == NULL) {
    return false;
  }
  time_t now_time = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  time_t des_start = atol(cell->dollars_start_time_.c_str());
  time_t des_end = atol(cell->dollars_end_time_.c_str());
  if(des_start <= now_time && des_start != 0) {
    return false;
  }
  if(des_end >= now_time) {
    return false;
  }
  return true;
}

} // namespace item

} // namespace server

} // namespace game

