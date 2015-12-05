//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-16 10:00:17.
// File name: playing_page_pool.h
//
// Description:
// Define class PlayingPagePool.
//

#include "game_server/server/extension/playing/playing_page_pool.h"

#include "game_server/server/extension/playing/playing_page.h"

namespace game {

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

}  // namespace game

