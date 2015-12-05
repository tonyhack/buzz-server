

namespace cpp game.server.event
namespace py ccevent.item

// 道具使用事件
// EVENT_ITEM_USE
struct EventItemUse {
  1 : required i64 id_;                   // 玩家ID
  2 : required i32 template_id_;          // 道具模板ID
}


// EVENT_ITEM_EQUIP_STRENGTH
struct EventItemEquipStrength {
  1 : required i64 id_;                   // 玩家ID
  2 : required i32 template_id_;          // 强化的道具ID
  3 : required bool result_;              // 强化结果
}


// 装备洗练
// EVENT_ITEM_EQUIP_REFINE
struct EventItemEquipRefine {
  1 : required i64 id_;                   // 玩家ID
  2 : required i32 template_id_;          // 道具模板ID
}


// 宝石镶嵌
// EVENT_ITEM_GEM_INLAY
struct EventItemEquipInlay {
  1 : required i64 id_;                   // 玩家ID
}


// 穿装备
// EVENT_ITEM_WEAR_EQUIP
struct EventItemWearEquip {
  1 : required i64 id_;                   // 玩家ID
  2 : required i32 template_id_;          // 道具模板ID
}


// 占星(购买宝石)
// EVENT_ITEM_DIVINE_GEM
struct EventItemDivineGem {
  1 : required i64 id_;                   // 玩家ID
}

