//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-03-26 18:36:25.
// File name: playing_page_pool.cc
//
// Description:
// Define PlayingPagePool.
//

#include "session_server/server/extension/playing/playing_page_pool.h"

#include "session_server/server/extension/playing/playing_page.h"

namespace session {

namespace server {

namespace playing {

bool PlayingPagePool::Initialize(size_t initial_number, size_t extend_number) {
	return this->pages_.Initialize(initial_number, extend_number);
}

void PlayingPagePool::Finalize() {
	this->pages_.Finalize();
}

PlayingPage *PlayingPagePool::Allocate() {
	return this->pages_.Allocate();
}

void PlayingPagePool::Deallocate(PlayingPage *page) {
	this->pages_.Deallocate(page);
}

}  // namespace playing

}  // namespace server

}  // namespace session

