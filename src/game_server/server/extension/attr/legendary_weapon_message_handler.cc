#include "game_server/server/extension/attr/legendary_weapon_message_handler.h"

#include <boost/bind.hpp>

#include "global/global_packet.h"
#include "global/logging.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_attr_protocol_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/attr/attr_actor.h"
#include "game_server/server/extension/attr/attr_actor_manager.h"
#include "game_server/server/extension/attr/configure.h"
#include "game_server/server/extension/attr/facade_request.h"
#include "game_server/server/extension/attr/legendary_weapon_actor.h"

namespace game {

namespace server {

namespace attr {

LegendaryWeaponMessageHandler::LegendaryWeaponMessageHandler() {}
LegendaryWeaponMessageHandler::~LegendaryWeaponMessageHandler() {}

bool LegendaryWeaponMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_LEGENDARY_WEAPON_REPAIR_REQUEST,
      boost::bind(&LegendaryWeaponMessageHandler::OnMessageAttrLegendaryWeaponRepairRequest,
          this, _1, _2, _3));

  return true;
}

void LegendaryWeaponMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ATTR_LEGENDARY_WEAPON_REPAIR_REQUEST);
}

static void SendMessageAttrLegendaryWeaponRepairResponseFailed(AttrActor *attr_actor) {
  gateway::protocol::MessageAttrLegendaryWeaponRepairResponse response;
  response.__set_result_(false);
  attr_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_ATTR_LEGENDARY_WEAPON_REPAIR_RESPONSE);
}

void LegendaryWeaponMessageHandler::OnMessageAttrLegendaryWeaponRepairRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取属性玩家对象
  AttrActor *attr_actor = AttrActorManager::GetInstance()->Get(id);
  if (NULL == attr_actor) {
    LOG_ERROR("Get AttrActor(%lu) from AttrActorManager failed.", id);
    return;
  }
  GameActor *game_actor = attr_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", id);
    return;
  }
  LegendaryWeaponActor *weapon_actor = attr_actor->GetLegendaryWeaponActor();

  // 消息解析
  gateway::protocol::MessageAttrLegendaryWeaponRepairRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageAttrLegendaryWeaponRepairRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_ATTR_LEGENDARY_WEAPON_REPAIR_REQUEST from actor(%lu, %s)",
           game_actor->GetID(), game_actor->GetName().c_str());

  // 检查功能开启
  if (game_actor->CheckFunctionalityState(
          entity::FunctionalityType::LEGENDARY_WEAPON_MODULE) == false) {
    LOG_WARNING("LEGENDARY_WEAPON_MODULE is disable.");
    SendMessageAttrLegendaryWeaponRepairResponseFailed(attr_actor);
    return;
  }

  for (size_t i = 0; i < request.targets_.size(); ++i) {
    const gateway::protocol::LegendaryWeaponRepairData &repair_target = request.targets_[i];

    LOG_INFO("weapon_id_=[%d], piece_index_=[%d], use_item_count_=[%d].",
             repair_target.weapon_id_, repair_target.piece_index_,
             repair_target.use_item_count_);

    // 检查名武ID
    const LegendaryWeaponCell *weapon_cell =
      Configure::GetInstance()->GetLegendaryWeapon(repair_target.weapon_id_);
    if (NULL == weapon_cell) {
      LOG_WARNING("weapon_id is invalid.");
      SendMessageAttrLegendaryWeaponRepairResponseFailed(attr_actor);
      return;
    }

    // 检查碎片索引
    if (repair_target.piece_index_ < 0 ||
        repair_target.piece_index_ >= (core::int32)weapon_cell->pieces_.size()) {
      LOG_WARNING("piece_index is invalid.");
      SendMessageAttrLegendaryWeaponRepairResponseFailed(attr_actor);
      return;
    }

    const LegendaryWeaponPieceCell *piece_cell =
      &weapon_cell->pieces_[repair_target.piece_index_];

    // 已有的碎片数量
    core::int32 old_piece_count = weapon_actor->GetWeaponPiece(
         repair_target.weapon_id_, repair_target.piece_index_);

    if (old_piece_count >= piece_cell->item_count_) {
      LOG_WARNING("piece is already repaired.");
      SendMessageAttrLegendaryWeaponRepairResponseFailed(attr_actor);
      return;
    }

    // 检查使用道具数量
    if (repair_target.use_item_count_ <= 0 ||
        old_piece_count + repair_target.use_item_count_ > (core::int32)piece_cell->item_count_) {
      LOG_WARNING("use_item_count is invalid.");
      SendMessageAttrLegendaryWeaponRepairResponseFailed(attr_actor);
      return;
    }

    // 检查道具是否足够
    if (FacadeRequest::GetInstance()->CheckPacketItemExist(id, piece_cell->item_id_,
            repair_target.use_item_count_, BindType::BOTH) == false) {
      LOG_WARNING("Item(%u) is not enough.", piece_cell->item_id_);
      SendMessageAttrLegendaryWeaponRepairResponseFailed(attr_actor);
      return;
    }

    // 扣除道具
    core::uint32 delete_number = FacadeRequest::GetInstance()->RemovePacketItem(id,
        piece_cell->item_id_, repair_target.use_item_count_, BindType::BOTH);
    if (delete_number != (core::uint32)repair_target.use_item_count_) {
      LOG_ERROR("Remove item(%d) failed, req_count=[%d], del_count=[%d].",
                piece_cell->item_id_, repair_target.use_item_count_, delete_number);
      SendMessageAttrLegendaryWeaponRepairResponseFailed(attr_actor);
      return;
    }

    // 增加碎片
    weapon_actor->AddWeaponPiece(repair_target.weapon_id_,
        repair_target.piece_index_, repair_target.use_item_count_);

    // 增加经验和灵气
    core::int32 new_piece_count = weapon_actor->GetWeaponPiece(
         repair_target.weapon_id_, repair_target.piece_index_);
    if (new_piece_count >= piece_cell->item_count_) {
      game_actor->AddResource(entity::ResourceID::EXP, piece_cell->exp_reward_);
      game_actor->AddResource(entity::ResourceID::NIMBUS, piece_cell->nimbus_reward_);
    }
  }

  gateway::protocol::MessageAttrLegendaryWeaponRepairResponse response;
  response.__set_result_(true);
  attr_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_ATTR_LEGENDARY_WEAPON_REPAIR_RESPONSE);
}

}  // namespace attr

}  // namespace server

}  // namespace game

