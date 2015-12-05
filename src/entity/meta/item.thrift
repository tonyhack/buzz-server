namespace cpp entity
namespace py ccentity.item
namespace as3 com.dreamfire.sanguo.data.definition.entity


////////////////////////////////////////////////
// 道具类型定义
enum ItemType {
  EQUIPMENT,                          // 装备
  MEDICINE,                           // 药品
  CARD,                               // 卡牌
  MATERIAL,                           // 材料
  TASK,                               // 任务

  MAX,
}


///////////////////////////////////////////////
// 子类型定义
enum EquipmentType {
  MIN = 0,

  WEAPONS = 0,                        // 武器
  HAT,                                // 帽子
  CLOTHES,                            // 衣服
  ARM,                                // 护腕
  BELT,                               // 腰带
  TROUSERS,                           // 裤子
  SHOES,                              // 鞋子
  RING,                               // 戒指
  NECKLACE,                           // 项链
  ACCESORY,                           // 配饰

  MAX,
}
enum MedicineType {
  MIN = 0,

  MEDICINE = 0,                       // 药品
  BUFF,                               // BUFF类
  SOUL,                               // 英灵卡类

  MAX,
}
enum MaterialType {
  MIN = 0,

  EQUIP_INTENSIFY = 0,                // 装备强化
  INLAY,                              // 镶嵌
  SLOTTING,                           // 打孔
  REFINE,                             // 祭炼
  PROMOTION,                          // 进阶
  HORSE_INTENSIFY,                    // 坐骑强化
  HORSE_UNREAL,                       // 坐骑幻化
  SOUL_STONE,                         // 英灵石
  RUNE = 13,                          // 符文

  MAX,
}
enum CardType {
  MIN = 0,

  MAX,
}
enum TaskItemType {
  MIN = 0,

  MAX,
}


////////////////////////////////////////////////
// 道具容器类型定义
enum ItemContainerType {
  PACKET = 0,                        // 背包栏
  EQUIP,                             // 装备栏
  SHOP,                              // 商品回购列表
  STORAGE,                           // 仓库
  TREASURE,                          // 礼品仓库

  MAX,        
}   


////////////////////////////////////////////////
// 道具使用目标类型
enum ItemTargetType {
  SELF = 0,                          // 对自己使用
  OTHER_ONE,                         // 对别人使用

}


////////////////////////////////////////////////
// 道具基本属性字段定义
enum ItemClientFields {
  CLIENT_BEGIN = 0,

  BIND_TYPE = 0,                      // 绑定类型
  STACK_COUNT,                        // 叠加数量

  CLIENT_END,
}

enum ItemServerFields {
  SERVER_BEGIN = 0,

  SERVER_END,
}


///////////////////////////////////////////////
// 道具属性对定义
struct ItemClientField {
  1 : required ItemClientFields field_;
  2 : required i32 value_;
}
struct ItemServerField {
  1 : required ItemServerFields field_;
  2 : required i32 value_;
}


///////////////////////////////////////////////
// 绑定定义
enum BindRequireType {
  NOT_BIND = 0,                        // 非绑定
  BIND = 1,                            // 绑定
  BOTH,                                // 都可以(扣除时道具的绑定类型)
}


///////////////////////////////////////////////
// 增加道具字段定义
struct AddItemFields {
  1 : required i32 item_template_;     // 道具模板
  2 : required i32 number_;            // 数量
  3 : required bool bind_;             // 是否绑定
  4 : required i32 intensify_level_;   // 强化等级
}


///////////////////////////////////////////////
// 道具初始化字段定义
struct ItemFields {
  1 : required string id_;                                  // ID
  2 : required i32 template_id_;                            // 模板ID
  3 : required ItemType type_;                              // 类型
  4 : required i16 location_;                               // 容器中的位置
  5 : required list<i32> item_client_fields_;               // 道具基础属性
  6 : optional list<i32> equipment_client_fields_;          // 装备属性
  7 : optional string first_equip_time_;                    // 首次装备时间
}

/////////////////////////////////////////////
// 使用状态需求
enum ItemUseRoleStatus {
  LIVE,                               // 活的
  DIED,                               // 死的
  ALL,                                // 无所谓(所有)
}

