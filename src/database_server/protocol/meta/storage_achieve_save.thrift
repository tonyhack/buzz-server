include "storage_achieve_login.thrift"

namespace cpp database.protocol


// type: STORAGE_ACHIEVE_SAVE
struct StorageAchieveSave {
  1 : required i64 actor_;
  2 : required list<storage_achieve_login.StorageAchieveFinishField> finish_achieves_;
  3 : required list<storage_achieve_login.StorageAchieveField>       achieves_;
}
