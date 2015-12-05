#ifndef __SESSION__SERVER__WORLD__DATA__MANAGER__H
#define __SESSION__SERVER__WORLD__DATA__MANAGER__H

#include <ext/hash_map>
#include <vector>
#include <string>
#include <mysql++/mysql++.h>

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace session {

namespace server {

struct WorldDataType {
  enum type {
    MIN = 0,

    // 地宫格子分布
    MAZE_MAPS,

    // 寻宝数据保存
    ITEM_TREASURE_HUNT,

    //商城、藏宝阁数据
    MALL_AND_TREASURE,

    MAX,
  };
};

class WorldDataManager : public global::SingletonFactory<WorldDataManager> {
  friend class global::SingletonFactory<WorldDataManager>;

 public:
  typedef __gnu_cxx::hash_map<core::int32, std::string> WorldDataHashmap;

  bool Initialize();
  void Finalize();

  std::string GetWorldData(WorldDataType::type id);
  // 异步保存(服务器运行中使用)
  void SaveWorldData(WorldDataType::type id, const std::string &data);
  // 同步保存(服务器启动关闭过程中使用)
  void SyncSaveWorldData(WorldDataType::type id, const std::string &data);

 private:
  WorldDataManager() {}
  ~WorldDataManager() {}

  bool LoadWorldData(mysqlpp::Connection &connection);
  bool SaveWorldData(mysqlpp::Connection &connection,
      WorldDataType::type id, const std::string &data);

  WorldDataHashmap data_map_;

};

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__WORLD__DATA__MANAGER__H

