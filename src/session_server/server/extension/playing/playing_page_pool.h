//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-03-26 18:29:23.
// File name: playing_page_pool.h
//
// Description:
// Define class PlayingPagePool.
//

#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace session {

namespace server {

namespace playing {

class PlayingPage;

class PlayingPagePool : public global::SingletonFactory<PlayingPagePool> {
  friend class global::SingletonFactory<PlayingPagePool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  PlayingPage *Allocate();
  void Deallocate(PlayingPage *page);

 private:
  PlayingPagePool() {}
  ~PlayingPagePool() {}

  global::PoolTemplate<PlayingPage> pages_;
};

}  // namespace playing

}  // namespace server

}  // namespace session

