//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-22 13:50:39.
// File name: configure.cc
//
// Description:
// Define class Configure.
//

#include "game_server/server/extension/item/configure.h"

#include "core/tinyxml/tinyxml.h"
#include "global/common_functions.h"
#include "global/logging.h"
#include "global/server_configure.h"

namespace game {

namespace server {

namespace item {

const float Configure::kIntensifyVar1_ = 0.12f;
const float Configure::kIntensifyVar2_ = 0.06f;

bool Configure::LoadConfigure(const std::string &file) {
  TiXmlDocument doc;
  if(doc.LoadFile(file.c_str()) == false) {
    global::LogError("%s:%d (%s) 加载 配置文件(%s)出错",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 configure 结点
  TiXmlElement *configure_node = doc.FirstChildElement("configure");
  if(configure_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [configure]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  // 加载 item_pool 结点
  TiXmlElement *item_pool_node = configure_node->FirstChildElement("item_pool");
  if(item_pool_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [configure-item_pool]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  if(item_pool_node->Attribute("initial_number",
        (int *)(&this->item_initial_number_)) == false) {
    global::LogError("%s:%d (%s) 找不到属性 [item_pool-initial_number]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  if(item_pool_node->Attribute("extend_number",
        (int *)(&this->item_extend_number_)) == false) {
    global::LogError("%s:%d (%s) 找不到属性 [item_pool-extend_number]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }

  global::LogDebug("道具对象池 initial_number=%d extend_number=%d",
      this->item_initial_number_, this->item_extend_number_);

  // 加载 random_attribute 结点
  TiXmlElement *random_attribute_node = configure_node->FirstChildElement("random_attribute");
  if(random_attribute_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [configure-random_attribute]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  const char *str = random_attribute_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 找不到属性[random_attribute-file]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  if(this->load_random_attribute_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false) {
    global::LogError("%s:%d (%s) 加载 random_attribute 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 加载 intensify 结点
  TiXmlElement *intensify_node = configure_node->FirstChildElement("intensify");
  if(intensify_node == NULL) {
    LOG_ERROR("找不到结点 [configure-intensify]，文件(%s)", file.c_str());
    return false;
  }
  str = intensify_node->Attribute("file");
  if(str == NULL) {
    LOG_ERROR("找不到属性 [intensify-file]，文件(%s)", file.c_str());
    return false;
  }
  if(this->load_intensify_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false) {
    LOG_ERROR("加载 intensify 配置失败");
    return false;
  }
  // 加载 refine 节点
  TiXmlElement *refine_node = configure_node->FirstChildElement("refine");
  if(refine_node == NULL) {
    LOG_ERROR("找不到结点 [configure-refine]，文件(%s)", file.c_str());
    return false;
  }
  str = refine_node->Attribute("file");
  if(str == NULL) {
    LOG_ERROR("找不到属性 [refine-file]，文件(%s)", file.c_str());
    return false;
  }
  if(this->load_refine_.LoadRefineConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false) {
    LOG_ERROR("加载 intensify 配置失败");
    return false;
  }
  // 加载洗练加锁消耗
  TiXmlElement *refine_lock_node = configure_node->FirstChildElement("refinelock");
  if(refine_lock_node == NULL) {
    LOG_ERROR("找不到结点 [configure-refinelock]，文件(%s)", file.c_str());
    return false;
  }
  str = refine_lock_node->Attribute("file");
  if(str == NULL) {
    LOG_ERROR("找不到属性 [refinelock-file]，文件(%s)", file.c_str());
    return false;
  }
  if(this->load_refine_.LoadEquipLockConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false) {
    LOG_ERROR("加载 refinelock 配置失败");
    return false;
  }
 
  // 加载 medicine 结点
  TiXmlElement *medicine_node = configure_node->FirstChildElement("medicine");
  if(medicine_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [configure-medicine]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  str = medicine_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 找不到属性[medicine-file]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  if(this->load_medicine_configure_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false) {
    global::LogError("%s:%d (%s) 加载 medicine 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 加载 equipment 结点
  TiXmlElement *equip_node = configure_node->FirstChildElement("equipment");
  if(equip_node == NULL) {
    global::LogError("%s:%d (%s) 找不到结点 [configure-equipment]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  str = equip_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 找不到属性[equipment-file]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  if(this->load_equipment_configure_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false) {
    global::LogError("%s:%d (%s) 加载 equipment 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 加载 material 结点
  TiXmlElement *material_node = configure_node->FirstChildElement("material");
  if(material_node == NULL) {
    global::LogError("%s:%d (%s) 找不到属性 [configure-material]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  str = material_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 加载 material 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->load_material_configure_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false) {
    global::LogError("%s:%d (%s) 加载 material 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 加载 gem 结点
  // 加载 gem_gathering 结点
  TiXmlElement *gem_node = configure_node->FirstChildElement("gem");
  if(gem_node == NULL) {
    global::LogError("%s:%d (%s) 找不到属性 [configure-gem]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  const char *gem_str = gem_node->Attribute("file");
  if(gem_str == NULL) {
    global::LogError("%s:%d (%s) 加载 gem 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  TiXmlElement *gem_gathering_node = configure_node->FirstChildElement("gem_gathering");
  if(gem_gathering_node == NULL) {
    global::LogError("%s:%d (%s) 找不到属性 [configure-gem_gathering]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  const char *gem_gathering_str = gem_gathering_node->Attribute("file");
  if(gem_gathering_str == NULL) {
    global::LogError("%s:%d (%s) 加载 gem_gathering 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->load_gem_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        gem_str,
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        gem_gathering_str) == false) {
    global::LogError("%s:%d (%s) 加载 gem 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 加载 shop 结点
  TiXmlElement *shop_node = configure_node->FirstChildElement("shop");
  if(shop_node == NULL) {
    global::LogError("%s:%d (%s) 找不到属性 [configure-shop]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  str = shop_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 加载 shop 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->load_shop_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false) {
    global::LogError("%s:%d (%s) 加载 mall 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 加载 vip_gifts 结点
  TiXmlElement *vip_gifts_node = configure_node->FirstChildElement("vip_gifts");
  if(vip_gifts_node == NULL) {
    global::LogError("%s:%d (%s) 找不到属性 [configure-vip_gifts]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  str = vip_gifts_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 加载 vip_gifts 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->load_vip_gifts_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false) {
    global::LogError("%s:%d (%s) 加载 vip_gifts 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 加载 compose_recipe 节点
  TiXmlElement *compose_recipe_node = configure_node->FirstChildElement("compose_recipe");
  if(compose_recipe_node == NULL) {
    global::LogError("%s:%d (%s) 找不到属性 [configure-compose_recipe]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  str = compose_recipe_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 加载 compose_recipe 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->load_item_compose_recipe_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false) {
    global::LogError("%s:%d (%s) 加载 compose_recipe 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 加载 suit 节点
  TiXmlElement *suit_node = configure_node->FirstChildElement("suit");
  if(suit_node == NULL) {
    global::LogError("%s:%d (%s) 找不到属性 [configure-suit]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  str = suit_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 加载 suit 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->load_suit_configure_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false) {
    global::LogError("%s:%d (%s) 加载 suit 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 加载 treasure_spend 节点
  TiXmlElement *treasure_spend_node = configure_node->FirstChildElement("treasure_spend");
  if(treasure_spend_node == NULL) {
    global::LogError("%s:%d (%s) 找不到属性 [configure-treasure_spend]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  str = treasure_spend_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 加载 treasure_spend 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->load_treasure_spend_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false) {
    global::LogError("%s:%d (%s) 加载 treasure_spend 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 加载 hunt_treasure 节点
  TiXmlElement *hunt_treasure_node = configure_node->FirstChildElement("hunt_treasure");
  if(hunt_treasure_node == NULL) {
    global::LogError("%s:%d (%s) 找不到属性 [configure-hunt_treasure]，文件(%s)",
        __FILE__, __LINE__, __FUNCTION__, file.c_str());
    return false;
  }
  str = hunt_treasure_node->Attribute("file");
  if(str == NULL) {
    global::LogError("%s:%d (%s) 加载 hunt_treasure 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->load_hunt_treasure_.LoadConfigure(
        global::ServerConfigureSingleton::GetInstance().GetConfigueDir() +
        str) == false) {
    global::LogError("%s:%d (%s) 加载 hunt_treasure 配置失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  return true;
}

const MaterialCell *Configure::GetMaterialConfigure(core::uint32 id) const {
  return this->load_material_configure_.GetMaterial(id);
}

const MedicineCell *Configure::GetMedicineConfigure(core::uint32 id) const {
  return this->load_medicine_configure_.GetMedicine(id);
}

const EquipmentCell *Configure::GetEquipmentConfigure(core::uint32 id) const {
  return this->load_equipment_configure_.GetEquipment(id);
}

const ItemCell *Configure::GetItem(core::uint32 id) const {
  const ItemCell *cell = this->GetMedicineConfigure(id);
  if(cell == NULL) {
    cell = this->GetEquipmentConfigure(id);
  }
  if(cell == NULL) {
    cell = this->GetMaterialConfigure(id);
  }
  return cell;
}

const RandomAttributeCell *Configure::GetRandomAttribute(core::uint32 random_id) const {
  return this->load_random_attribute_.RandomAttribute(random_id);
}

const IntensifyCell *Configure::GetIntensifyConfigure(core::int32 equip_level,
    core::int32 intensify) const {
  return this->load_intensify_.GetIntensify(equip_level, intensify);
}

const EquipRefineSpendCell *Configure::GetRefineConfigure(core::int32 lv, core::int32 quality) const {
  return this->load_refine_.GetEquipRefineSpend((core::uint32)lv, quality);
}

const EquipRefineSpendLockCell *Configure::GetRefineLockConfigure(core::int32 lock_num) const {
  return this->load_refine_.GetEquiplockSpend(lock_num);
}

const GemCell *Configure::GetGem(core::int32 id, core::int32 level) const {
  return this->load_gem_.GetGem(id, level);
}

core::int32 Configure::GetRandomGemIDByQuality(GemCell::QualityType::type type) const {
  return this->load_gem_.GetRandomGemIDByQuality(type);
}

const GemGatheringPointCell *Configure::GetGemGatheringPoint(core::int32 id) const {
  return this->load_gem_.GetGemGatheringPoint(id);
}

const GemGatheringPointCell *Configure::GetNextGemGatheringPoint(core::int32 id) const {
  return this->load_gem_.GetNextGemGatheringPoint(id);
}

const GemGatheringPointCell *Configure::GetPrevGemGatheringPoint(core::int32 id) const {
  return this->load_gem_.GetPrevGemGatheringPoint(id);
}

const GemGatheringPointCell *Configure::GetLastGemGatheringPoint() const {
  return this->load_gem_.GetLastGemGatheringPoint();
}

bool Configure::CheckShopTemplateID(core::int32 id) {
  return this->load_shop_.GetTemplateID(id);
}

const LoadVipGiftsConfigure::Awards *Configure::GetAwards(entity::VipType::type type) const {
  return this->load_vip_gifts_.GetAwards(type);
}

const ItemComposeRecipeCell *Configure::GetItemComposeRecipe(core::int32 id) const {
  return this->load_item_compose_recipe_.GetItemComposeRecipe(id);
}

const LoadSuitConfigure::EffectVecs *Configure::GetSuit(core::int32 suit_id) const {
  return this->load_suit_configure_.GetCell(suit_id);
}

HuntTreasureRandCell *Configure::GetTreasureRandDrop(core::int32 tab_id) {
  return this->load_hunt_treasure_.GetRand(tab_id);
}

const TreasureSpendCell *Configure::GetSpendCell(core::int32 id, core::int32 num) const {
  return this->load_treasure_spend_.GetCell(id, num);
}

IntegralType::type Configure::GetItegralType(core::int32 tab_id) {
  return this->load_hunt_treasure_.GetItegralType(tab_id);
}

}  // namespace item

}  // namespace server

}  // namespace game

