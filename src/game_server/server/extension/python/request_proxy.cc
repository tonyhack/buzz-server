//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-02 14:34:27.
// File name: request_proxy.cc
//
// Description:
// Define class RequestProxy.
//

#include "game_server/server/extension/python/request_proxy.h"

#include "game_server/server/extension_manager.h"
#include "game_server/server/request/game_attr_request_types.h"
#include "game_server/server/request/game_ai_request_types.h"
#include "game_server/server/request/game_buff_request_types.h"
#include "game_server/server/request/game_cooling_request_types.h"
#include "game_server/server/request/game_effect_request_types.h"
#include "game_server/server/request/game_guild_request_types.h"
#include "game_server/server/request/game_item_request_types.h"
#include "game_server/server/request/game_mail_request_types.h"
#include "game_server/server/request/game_playing_request_types.h"
#include "game_server/server/request/game_scene_request_types.h"
#include "game_server/server/request/game_skill_request_types.h"
#include "game_server/server/request/game_soul_request_types.h"
#include "game_server/server/request/game_team_request_types.h"
#include "global/global_packet.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace python {

RequestProxy::RequestProxy() {}
RequestProxy::~RequestProxy() {}

bool RequestProxy::Initialize() {
  return true;
}

void RequestProxy::Finalize() {}

//int RequestProxy::Request(request::RequestType::type type,
int RequestProxy::Request(int type,
    const char *message, size_t size) {
  switch(type) {
    case request::RequestType::REQUEST_CHECK_IN_COOLING:
      return this->RequestHandler<request::RequestCheckInCooling>(type, message, size);
    case request::RequestType::REQUEST_START_COOLING:
      return this->RequestHandler<request::RequestStartCooling>(type, message, size);
    case request::RequestType::REQUEST_SCENE_GET_AROUND_ACTORS:
      return this->RequestHandler<request::RequestSceneGetAroundActors>(type, message, size);
    case request::RequestType::REQUEST_SCENE_SUMMON_NPC:
      return this->RequestHandler<request::RequestSceneSummonNpc>(type, message, size);
    case request::RequestType::REQUEST_SCENE_CLEAR_NPC:
      return this->RequestHandler<request::RequestSceneClearNpc>(type, message, size);
    case request::RequestType::REQUEST_SCENE_ROLE_MOVE:
      return this->RequestHandler<request::RequestSceneRoleMove>(type, message, size);
    case request::RequestType::REQUEST_SCENE_JUMP:
      return this->RequestHandler<request::RequestSceneJump>(type, message, size);
    case request::RequestType::REQUEST_SCENE_DAYNIGHT_INFO:
      return this->RequestHandler<request::RequestSceneDayNightInfo>(type, message, size);
    case request::RequestType::REQUEST_SCENE_ROUTER:
      return this->RequestHandler<request::RequestSceneRouter>(type, message, size);
    case request::RequestType::REQUEST_SCENE_PK_INFO:
      return this->RequestHandler<request::RequestScenePKInfo>(type, message, size);
    case request::RequestType::REQUEST_SCENE_IS_SAFE:
      return this->RequestHandler<request::RequestSceneIsSafe>(type, message, size);
    case request::RequestType::REQUEST_SCENE_ACTOR_BROADCAST:
      return this->RequestHandler<request::RequestSceneActorBroadcast>(type, message, size);
    case request::RequestType::REQUEST_SCENE_CREATE_MAP:
      return this->RequestHandler<request::RequestSceneCreateMap>(type, message, size);
    case request::RequestType::REQUEST_SCENE_DESTORY_MAP:
      return this->RequestHandler<request::RequestSceneDestoryMap>(type, message, size);
    case request::RequestType::REQUEST_SCENE_CLEAR_ALL_NPC:
      return this->RequestHandler<request::RequestSceneClearAllNpc>(type, message, size);
    case request::RequestType::REQUEST_SCENE_REVIVE:
      return this->RequestHandler<request::RequestSceneRevive>(type, message, size);
    case request::RequestType::REQUEST_EFFECT_START:
      return this->RequestHandler<request::RequestEffectStart>(type, message, size);
    case request::RequestType::REQUEST_EFFECT_END:
      return this->RequestHandler<request::RequestEffectEnd>(type, message, size);
    case request::RequestType::REQUEST_BUFF_START:
      return this->RequestHandler<request::RequestBuffStart>(type, message, size);
    case request::RequestType::REQUEST_BUFF_END:
      return this->RequestHandler<request::RequestBuffEnd>(type, message, size);
    case request::RequestType::REQUEST_ADD_ITEM:
      return this->RequestHandler<request::RequestAddItem>(type, message, size);
    case request::RequestType::REQUEST_ADD_ITEMS:
      return this->RequestHandler<request::RequestAddItems>(type, message, size);
    case request::RequestType::REQUEST_GET_PAKCET_ITEM_NUMBER:
      return this->RequestHandler<request::RequestGetPacketItemNumber>(type, message, size);
    case request::RequestType::REQUEST_REMOVE_ITEM:
      return this->RequestHandler<request::RequestRemoveItem>(type, message, size);
    case request::RequestType::REQUEST_REMOVE_ITEM_LOCATION_ID:
      return this->RequestHandler<request::RequestRemoveItemLocationID>(type, message, size);
    case request::RequestType::REQUEST_USE_SKILL:
      return this->RequestHandler<request::RequestUseSkill>(type, message, size);
    case request::RequestType::REQUEST_ADD_SKILL:
      return this->RequestHandler<request::RequestAddSkill>(type, message, size);
    case request::RequestType::REQUEST_REMOVE_SKILL:
      return this->RequestHandler<request::RequestRemoveSkill>(type, message, size);
    case request::RequestType::REQUEST_UPGRADE_SKILL:
      return this->RequestHandler<request::RequestUpgradeSkill>(type, message, size);
    case request::RequestType::REQUEST_SKILL_STAND:
      return this->RequestHandler<request::RequestSkillStand>(type, message, size);
    case request::RequestType::REQUEST_SKILL_SIT:
      return this->RequestHandler<request::RequestSkillSit>(type, message, size);
    case request::RequestType::REQUEST_ADD_SKILL_GLOBAL_BUFF:
      return this->RequestHandler<request::RequestAddSkillGlobalBuff>(type, message, size);
    case request::RequestType::REQUEST_REMOVE_SKILL_GLOBAL_BUFF:
      return this->RequestHandler<request::RequestRemoveSkillGlobalBuff>(type, message, size);
    case request::RequestType::REQUEST_SKILL_ADD_TEMP_SKILL:
      return this->RequestHandler<request::RequestSkillAddTempSkill>(type, message, size);
    case request::RequestType::REQUEST_SKILL_REMOVE_TEMP_SKILL:
      return this->RequestHandler<request::RequestSkillRemoveTempSkill>(type, message, size);
    case request::RequestType::REQUEST_SKILL_CHANGE_SKILL_FORM:
      return this->RequestHandler<request::RequestSkillChangeSkillForm>(type, message, size);
    case request::RequestType::REQUEST_CHECK_SOUL_EXIST:
      return this->RequestHandler<request::RequestCheckSoulExit>(type, message, size);
    case request::RequestType::REQUEST_ADD_SOUL:
      return this->RequestHandler<request::RequestAddSoul>(type, message, size);
    case request::RequestType::REQUEST_ATTR_CHECK_SWORD_OPEN:
      return this->RequestHandler<request::RequestAttrCheckSwordOpen>(type, message, size);
    case request::RequestType::REQUEST_ATTR_OPEN_SWORD:
      return this->RequestHandler<request::RequestAttrOpenSword>(type, message, size);
    case request::RequestType::REQUEST_ATTR_ADD_SPIRIT_FACADE:
      return this->RequestHandler<request::RequestAttrAddSpiritFacade>(type, message, size);
    case request::RequestType::REQUEST_MAIL_SEND:
      return this->RequestHandler<request::RequestMailSend>(type, message, size);
    case request::RequestType::REQUEST_PLAYING_SYNCHRONIZE_SCORE:
      return this->RequestHandler<request::RequestPlayingSynchronizeScore>(type, message, size);
    case request::RequestType::REQUEST_PLAYING_SYNCHRONIZE_RANKING:
      return this->RequestHandler<request::RequestPlayingSynchronizeRanking>(type, message, size);
    case request::RequestType::REQUEST_PLAYING_INITIALIZE_ACTOR:
      return this->RequestHandler<request::RequestPlayingInitializeActor>(type, message, size);
    case request::RequestType::REQUEST_PLAYING_COMPLETE:
      return this->RequestHandler<request::RequestPlayingComplete>(type, message, size);
    case request::RequestType::REQUEST_PLAYING_ACTOR_COMPLETE:
      return this->RequestHandler<request::RequestPlayingActorComplete>(type, message, size);
    case request::RequestType::REQUEST_PLAYING_ADD_AWARD:
      return this->RequestHandler<request::RequestPlayingAddAward>(type, message, size);
    case request::RequestType::REQUEST_PLAYING_INCREASE_COMPLETE:
      return this->RequestHandler<request::RequestPlayingIncreaseComplete>(type, message, size);
    case request::RequestType::REQUEST_PLAYING_ACTOR_INCREASE_COMPLETE:
      return this->RequestHandler<request::RequestPlayingActorIncreaseComplete>(type, message, size);
    case request::RequestType::REQUEST_PLAYING_FAILURE:
      return this->RequestHandler<request::RequestPlayingFailure>(type, message, size);
    case request::RequestType::REQUEST_PLAYING_UPDATE_RECORD:
      return this->RequestHandler<request::RequestPlayingUpdateRecord>(type, message, size);
    case request::RequestType::REQUEST_PLAYING_UPDATE_RESULT:
      return this->RequestHandler<request::RequestPlayingUpdateResult>(type, message, size);
    case request::RequestType::REQUEST_PLAYING_AWARD_ACTOR:
      return this->RequestHandler<request::RequestPlayingAwardActor>(type, message, size);
    case request::RequestType::REQUEST_AI_ADD_THREAT:
      return this->RequestHandler<request::RequestAiAddThreat>(type, message, size);
    case request::RequestType::REQUEST_AI_SET_TARGET:
      return this->RequestHandler<request::RequestAiSetTarget>(type, message, size);
    case request::RequestType::REQUEST_AI_SET_AI:
      return this->RequestHandler<request::RequestAiSetAi>(type, message, size);
    case request::RequestType::REQUEST_GUILD_GET_ACTOR_GUILD:
      return this->RequestHandler<request::RequestGuildGetActorGuild>(type, message, size);
    case request::RequestType::REQUEST_TEAM_GET_ACTOR_TEAM:
      return this->RequestHandler<request::RequestTeamGetActorTeam>(type, message, size);

    default:
      return -1;
  }
}

}  // namespace python

}  // namespace server

}  // namespace game

