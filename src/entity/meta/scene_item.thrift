namespace cpp entity
namespace py ccentity.scene_item
namespace as3 com.dreamfire.sanguo.data.definition.entity

enum SceneItemAoiFields {
  AOI_BEGIN = 0,

  TYPE = 0,                               // 类型
  TEMPLATE,                               // ID
  BIND,                                   // 是否绑定(0:非绑定)
  NUMBER,                                 // 数量

  AOI_END,
}

struct SceneItemAoiField {
  1 : required SceneItemAoiFields field_;
  2 : required i32 value_;
}

