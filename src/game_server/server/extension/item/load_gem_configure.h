#ifndef __GAME__SERVER__ITEM__LOAD__GEM__CONFIGURE__H
#define __GAME__SERVER__ITEM__LOAD__GEM__CONFIGURE__H

#include <cstddef>
#include <string>
#include <vector>
#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension/item/item_configure.h"

namespace game {

namespace server {

namespace item {

class LoadGemConfigure : public core::Noncopyable {
  typedef __gnu_cxx::hash_map<core::uint64, GemCell>  GemHashmap;
  typedef std::vector< std::vector<core::int32> > GemQualityGroupVector;
  typedef __gnu_cxx::hash_map<core::int32, size_t>  GemGatheringPointIndexHashmap;
  typedef std::vector<GemGatheringPointCell> GemGatheringPointVector;

 public:
  LoadGemConfigure();
  ~LoadGemConfigure();

  bool LoadConfigure(const std::string &gem_file, const std::string &gem_gathering_file);

  const GemCell *GetGem(core::int32 id, core::int32 level) const;
  core::int32 GetRandomGemIDByQuality(GemCell::QualityType::type type) const;
  const GemGatheringPointCell *GetGemGatheringPoint(core::int32 id) const;
  const GemGatheringPointCell *GetNextGemGatheringPoint(core::int32 id) const;
  const GemGatheringPointCell *GetPrevGemGatheringPoint(core::int32 id) const;
  const GemGatheringPointCell *GetLastGemGatheringPoint() const;
  
 private:
  GemHashmap gems_;
  GemQualityGroupVector quality_group_gems_;
  GemGatheringPointIndexHashmap gem_gathering_points_index_;
  GemGatheringPointVector gem_gathering_points_store_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__LOAD__GEM__CONFIGURE__H

