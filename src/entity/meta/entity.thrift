namespace cpp entity
namespace py ccentity.entity
namespace as3 com.dreamfire.sanguo.data.definition.entity

enum EntityType {
  TYPE_ACTOR = 0,
  TYPE_NPC,
  TYPE_SCENE_ITEM,

  TYPE_MAX,
}

enum EntityClientFields {
  CLIENT_BEGIN = 0,

  CLIENT_END,
}

enum EntityServerFields {
  SERVER_BEGIN = 0,

  SERVER_END,
}

struct EntityClientField {
  1 : required EntityClientFields field_;
  2 : required i32 value_;
}

