#include "robot/actor_sight.h"

#include <cstdlib>
#include <brickred/unique_ptr.h>

#include "robot/logging.h"
#include "robot/thrift_codec.h"
#include "robot/actor.h"
#include "robot/actor_event.h"
#include "robot/aoi_entity.h"
#include "robot/aoi_role.h"
#include "robot/aoi_actor.h"
#include "robot/aoi_npc.h"
#include "robot/aoi_scene_item.h"
#include "protocol/entity_types.h"
#include "protocol/gateway_protocol_types.h"
#include "protocol/gateway_entity_protocol_types.h"

namespace robot {

using brickred::UniquePtr;

ActorSight::ActorSight(Actor *actor) :
    actor_(actor)
{
    login_start_ev_token_ = actor_->addEventListener(ActorEventType::LOGIN_START,
        BRICKRED_BIND_MEM_FUNC(&ActorSight::onActorEventLoginStart, this));
    logout_ev_token_ = actor_->addEventListener(ActorEventType::LOGOUT,
        BRICKRED_BIND_MEM_FUNC(&ActorSight::onActorEventLogout, this));
}

ActorSight::~ActorSight()
{
    actor_->removeEventListener(login_start_ev_token_);
    actor_->removeEventListener(logout_ev_token_);

    for (size_t i = 0; i < foward_message_ev_tokens_.size(); ++i) {
        actor_->removeEventListener(foward_message_ev_tokens_[i]);
    }

    clearSight();
}

const ActorSight::AoiActorMap &ActorSight::getAoiActors() const
{
    return aoi_actors_;
}

const ActorSight::AoiNpcMap &ActorSight::getAoiNpcs() const
{
    return aoi_npcs_;
}

const ActorSight::AoiSceneItemMap &ActorSight::getAoiSceneItems() const
{
    return aoi_scene_items_;
}

const AoiActor *ActorSight::getAoiActor(int64_t id) const
{
    AoiActorMap::const_iterator iter =
        aoi_actors_.find(id);
    if (iter != aoi_actors_.end()) {
        return iter->second;
    }

    return NULL;
}

const AoiNpc *ActorSight::getAoiNpc(int64_t id) const
{
    AoiNpcMap::const_iterator iter =
        aoi_npcs_.find(id);
    if (iter != aoi_npcs_.end()) {
        return iter->second;
    }

    return NULL;
}

const AoiSceneItem *ActorSight::getAoiSceneItem(int64_t id) const
{
    AoiSceneItemMap::const_iterator iter =
        aoi_scene_items_.find(id);
    if (iter != aoi_scene_items_.end()) {
        return iter->second;
    }

    return NULL;
}

const AoiEntity *ActorSight::getAoiEntity(int64_t id) const
{
     const AoiEntity *aoi_entity = NULL;

     aoi_entity = getAoiNpc(id);
     if (aoi_entity != NULL) {
        return aoi_entity;
     }
     
     aoi_entity = getAoiActor(id);
     if (aoi_entity != NULL) {
        return aoi_entity;
     }

     aoi_entity = getAoiSceneItem(id); 
     if (aoi_entity != NULL) {
        return aoi_entity;
     }

     return NULL;
}

const AoiRole *ActorSight::getAoiRole(int64_t id) const
{
     const AoiRole *aoi_role = NULL;

     aoi_role = getAoiNpc(id);
     if (aoi_role != NULL) {
        return aoi_role;
     }
     
     aoi_role = getAoiActor(id);
     if (aoi_role != NULL) {
        return aoi_role;
     }

     return NULL;
}

void ActorSight::clearSight()
{
    for (AoiActorMap::iterator iter = aoi_actors_.begin();
         iter != aoi_actors_.end(); ++iter) {
        delete iter->second;
    }
    aoi_actors_.clear();
    
    for (AoiNpcMap::iterator iter = aoi_npcs_.begin();
         iter != aoi_npcs_.end(); ++iter) {
        delete iter->second;
    }
    aoi_npcs_.clear();

    for (AoiSceneItemMap::iterator iter = aoi_scene_items_.begin();
         iter != aoi_scene_items_.end(); ++iter) {
        delete iter->second;
    }
    aoi_scene_items_.clear();
}

void ActorSight::onActorEventLoginStart(const void *event)
{
    actor_->setMessageHandler(
        gateway::protocol::MessageType::MESSAGE_CREATE_AOI_ROLES,
        BRICKRED_BIND_MEM_FUNC(
            &ActorSight::onMessageCreateAoiRoles, this));
    actor_->setMessageHandler(
        gateway::protocol::MessageType::MESSAGE_DESTORY_AOI_ROLES,
        BRICKRED_BIND_MEM_FUNC(
            &ActorSight::onMessageDestoryAoiRoles, this));
    actor_->setMessageHandler(
        gateway::protocol::MessageType::MESSAGE_CREATE_ACTOR_SYNCHRONIZE,
        BRICKRED_BIND_MEM_FUNC(
            &ActorSight::onMessageCreateActorSynchronize, this));
    actor_->setMessageHandler(
        gateway::protocol::MessageType::MESSAGE_CREATE_NPC_SYNCHRONIZE,
        BRICKRED_BIND_MEM_FUNC(
            &ActorSight::onMessageCreateNpcSynchronize, this));
    actor_->setMessageHandler(
        gateway::protocol::MessageType::MESSAGE_CREATE_SCENE_ITEM_SYNCHRONIZE,
        BRICKRED_BIND_MEM_FUNC(
            &ActorSight::onMessageCreateSceneItemSynchronize, this));
    actor_->setMessageHandler(
        gateway::protocol::MessageType::MESSAGE_DESTORY_ROLE_SYNCHRONIZE,
        BRICKRED_BIND_MEM_FUNC(
            &ActorSight::onMessageDestoryRoleSynchronize, this));
    actor_->setMessageHandler(
        gateway::protocol::MessageType::MESSAGE_SYNCHRONIZE_NPC,
        BRICKRED_BIND_MEM_FUNC(
            &ActorSight::onMessageSynchronizeNpc, this));
    actor_->setMessageHandler(
        gateway::protocol::MessageType::MESSAGE_SYNCHRONIZE_SCENE_ITEM,
        BRICKRED_BIND_MEM_FUNC(
            &ActorSight::onMessageSynchronizeSceneItem, this));

    // foward messages
    int64_t ev_token = -1;

    ev_token = actor_->addEventListener(
        ActorEventType::FORWARD_MESSAGE_ROLE_MOVE_SYNCHRONIZE,
        BRICKRED_BIND_MEM_FUNC(
            &ActorSight::onMessageRoleMoveSynchronize, this));
    foward_message_ev_tokens_.push_back(ev_token);

    ev_token = actor_->addEventListener(
        ActorEventType::FORWARD_MESSAGE_ACTOR_RELOCATION,
        BRICKRED_BIND_MEM_FUNC(
            &ActorSight::onMessageActorRelocation, this));
    foward_message_ev_tokens_.push_back(ev_token);

    ev_token = actor_->addEventListener(
        ActorEventType::FORWARD_MESSAGE_SYNCHRONIZE_ACTOR,
        BRICKRED_BIND_MEM_FUNC(
            &ActorSight::onMessageSynchronizeActor, this));
    foward_message_ev_tokens_.push_back(ev_token);
}

void ActorSight::onActorEventLogout(const void *event)
{
    actor_->resetMessageHandler(
        gateway::protocol::MessageType::MESSAGE_CREATE_AOI_ROLES);
    actor_->resetMessageHandler(
        gateway::protocol::MessageType::MESSAGE_DESTORY_AOI_ROLES);
    actor_->resetMessageHandler(
        gateway::protocol::MessageType::MESSAGE_CREATE_ACTOR_SYNCHRONIZE);
    actor_->resetMessageHandler(
        gateway::protocol::MessageType::MESSAGE_CREATE_NPC_SYNCHRONIZE);
    actor_->resetMessageHandler(
        gateway::protocol::MessageType::MESSAGE_CREATE_SCENE_ITEM_SYNCHRONIZE);
    actor_->resetMessageHandler(
        gateway::protocol::MessageType::MESSAGE_DESTORY_ROLE_SYNCHRONIZE);
    actor_->resetMessageHandler(
        gateway::protocol::MessageType::MESSAGE_SYNCHRONIZE_NPC);
    actor_->resetMessageHandler(
        gateway::protocol::MessageType::MESSAGE_SYNCHRONIZE_SCENE_ITEM);

    for (size_t i = 0; i < foward_message_ev_tokens_.size(); ++i) {
        actor_->removeEventListener(foward_message_ev_tokens_[i]);
    }
    foward_message_ev_tokens_.clear();

    clearSight();
}

void ActorSight::onMessageCreateAoiRoles(const char *message,
                                         size_t message_size)
{
    gateway::protocol::MessageCreateAoiRoles sync;
    if (thrift_codec::decode(sync, message, message_size) <= 0) {
        LOG_ERROR("actor(%lu) decode MESSAGE_CREATE_AOI_ROLES failed",
                  actor_->getActorNo());
        return;
    }

    for (size_t i = 0; i < sync.actors_.size(); ++i) {
        int64_t id = ::atol(sync.actors_[i].id_.c_str());

        AoiActorMap::iterator iter = aoi_actors_.find(id);
        if (iter != aoi_actors_.end()) {
            delete iter->second;
            aoi_actors_.erase(iter);
        }

        UniquePtr<AoiActor> aoi_actor(new AoiActor(sync.actors_[i]));
        aoi_actors_.insert(
            std::make_pair(id, aoi_actor.get()));
        aoi_actor.release();
    }

    for (size_t i = 0; i < sync.npcs_.size(); ++i) {
        int64_t id = ::atol(sync.npcs_[i].id_.c_str());

        AoiNpcMap::iterator iter = aoi_npcs_.find(id);
        if (iter != aoi_npcs_.end()) {
            delete iter->second;
            aoi_npcs_.erase(iter);
        }

        UniquePtr<AoiNpc> aoi_npc(new AoiNpc(sync.npcs_[i]));
        aoi_npcs_.insert(
            std::make_pair(id, aoi_npc.get()));
        aoi_npc.release();
    }

    for (size_t i = 0; i < sync.items_.size(); ++i) {
        int64_t id = ::atol(sync.items_[i].id_.c_str());

        AoiSceneItemMap::iterator iter = aoi_scene_items_.find(id);
        if (iter != aoi_scene_items_.end()) {
            delete iter->second;
            aoi_scene_items_.erase(iter);
        }

        UniquePtr<AoiSceneItem> aoi_scene_item(
            new AoiSceneItem(sync.items_[i]));
        aoi_scene_items_.insert(
            std::make_pair(id, aoi_scene_item.get()));
        aoi_scene_item.release();
    }
}

void ActorSight::onMessageDestoryAoiRoles(const char *message,
                                          size_t message_size)
{
    gateway::protocol::MessageDestoryAoiRoles sync;
    if (thrift_codec::decode(sync, message, message_size) <= 0) {
        LOG_ERROR("actor(%lu) decode MESSAGE_DESTORY_AOI_ROLES failed",
                  actor_->getActorNo());
        return;
    }

    for (size_t i = 0; i < sync.actors_.size(); ++i) {
        int64_t id = ::atol(sync.actors_[i].c_str());

        AoiActorMap::iterator iter = aoi_actors_.find(id);
        if (iter != aoi_actors_.end()) {
            delete iter->second;
            aoi_actors_.erase(iter);
        }
    }

    for (size_t i = 0; i < sync.npcs_.size(); ++i) {
        int64_t id = ::atol(sync.npcs_[i].c_str());

        AoiNpcMap::iterator iter = aoi_npcs_.find(id);
        if (iter != aoi_npcs_.end()) {
            delete iter->second;
            aoi_npcs_.erase(iter);
        }
    }

    for (size_t i = 0; i < sync.items_.size(); ++i) {
        int64_t id = ::atol(sync.items_[i].c_str());

        AoiSceneItemMap::iterator iter = aoi_scene_items_.find(id);
        if (iter != aoi_scene_items_.end()) {
            delete iter->second;
            aoi_scene_items_.erase(iter);
        }
    }
}

void ActorSight::onMessageCreateActorSynchronize(const char *message,
                                                 size_t message_size)
{
    gateway::protocol::MessageCreateActorSynchronize sync;
    if (thrift_codec::decode(sync, message, message_size) <= 0) {
        LOG_ERROR("actor(%lu) decode MESSAGE_CREATE_ACTOR_SYNCHRONIZE failed",
                  actor_->getActorNo());
        return;
    }

    int64_t id = ::atol(sync.id_.c_str());

    // avoid to add self
    if (actor_->getActorId() == id) {
        return;
    }

    AoiActorMap::iterator iter = aoi_actors_.find(id);
    if (iter != aoi_actors_.end()) {
        delete iter->second;
        aoi_actors_.erase(iter);
    }

    UniquePtr<AoiActor> aoi_actor(new AoiActor(sync));
    aoi_actors_.insert(
        std::make_pair(id, aoi_actor.get()));
    aoi_actor.release();
}

void ActorSight::onMessageCreateNpcSynchronize(const char *message,
                                               size_t message_size)
{
    gateway::protocol::MessageCreateNpcSynchronize sync;
    if (thrift_codec::decode(sync, message, message_size) <= 0) {
        LOG_ERROR("actor(%lu) decode MESSAGE_CREATE_NPC_SYNCHRONIZE failed",
                  actor_->getActorNo());
        return;
    }

    int64_t id = ::atol(sync.id_.c_str());

    AoiNpcMap::iterator iter = aoi_npcs_.find(id);
    if (iter != aoi_npcs_.end()) {
        delete iter->second;
        aoi_npcs_.erase(iter);
    }

    UniquePtr<AoiNpc> aoi_npc(new AoiNpc(sync));
    aoi_npcs_.insert(
        std::make_pair(id, aoi_npc.get()));
    aoi_npc.release();
}

void ActorSight::onMessageCreateSceneItemSynchronize(const char *message,
                                                     size_t message_size)
{
    gateway::protocol::MessageCreateSceneItemSynchronize sync;
    if (thrift_codec::decode(sync, message, message_size) <= 0) {
        LOG_ERROR("actor(%lu) decode MESSAGE_CREATE_SCENE_ITEM_SYNCHRONIZE failed",
                  actor_->getActorNo());
        return;
    }

    int64_t id = ::atol(sync.id_.c_str());

    AoiSceneItemMap::iterator iter = aoi_scene_items_.find(id);
    if (iter != aoi_scene_items_.end()) {
        delete iter->second;
        aoi_scene_items_.erase(iter);
    }

    UniquePtr<AoiSceneItem> aoi_scene_item(
        new AoiSceneItem(sync));
    aoi_scene_items_.insert(
        std::make_pair(id, aoi_scene_item.get()));
    aoi_scene_item.release();
}

void ActorSight::onMessageDestoryRoleSynchronize(const char *message,
                                                 size_t message_size)
{
    gateway::protocol::MessageDestoryRoleSynchronize sync;
    if (thrift_codec::decode(sync, message, message_size) <= 0) {
        LOG_ERROR("actor(%lu) decode MESSAGE_DESTORY_ROLE_SYNCHRONIZE failed",
                  actor_->getActorNo());
        return;
    }

    int64_t id = ::atol(sync.id_.c_str());

    if (entity::EntityType::TYPE_ACTOR == sync.type_) {
        AoiActorMap::iterator iter = aoi_actors_.find(id);
        if (iter != aoi_actors_.end()) {
            delete iter->second;
            aoi_actors_.erase(iter);
        }

    } else if (entity::EntityType::TYPE_NPC == sync.type_) {
        AoiNpcMap::iterator iter = aoi_npcs_.find(id);
        if (iter != aoi_npcs_.end()) {
            delete iter->second;
            aoi_npcs_.erase(iter);
        }

    } else if (entity::EntityType::TYPE_SCENE_ITEM == sync.type_) {
        AoiSceneItemMap::iterator iter = aoi_scene_items_.find(id);
        if (iter != aoi_scene_items_.end()) {
            delete iter->second;
            aoi_scene_items_.erase(iter);
        }
    }
}

void ActorSight::onMessageRoleMoveSynchronize(const void *message)
{
    const gateway::protocol::MessageRoleMoveSynchronize &sync =
        *(const gateway::protocol::MessageRoleMoveSynchronize *)message;

    int64_t id = ::atol(sync.id_.c_str());

    if (entity::EntityType::TYPE_ACTOR == sync.type_) {
        AoiActorMap::iterator iter = aoi_actors_.find(id);
        if (iter != aoi_actors_.end()) {
            iter->second->update(sync);
        }
    } else if (entity::EntityType::TYPE_NPC == sync.type_) {
        AoiNpcMap::iterator iter = aoi_npcs_.find(id);
        if (iter != aoi_npcs_.end()) {
            iter->second->update(sync);
        }
    }
}

void ActorSight::onMessageActorRelocation(const void *message)
{
    const gateway::protocol::MessageActorRelocation &sync =
        *(const gateway::protocol::MessageActorRelocation *)message;

    int64_t id = ::atol(sync.id_.c_str());

    AoiActorMap::iterator iter = aoi_actors_.find(id);
    if (iter != aoi_actors_.end()) {
        iter->second->update(sync);
    }
}

void ActorSight::onMessageSynchronizeActor(const void *message)
{
    const gateway::protocol::MessageSynchronizeActor &sync =
        *(const gateway::protocol::MessageSynchronizeActor *)message;

    int64_t id = ::atol(sync.actor_id_.c_str());

    AoiActorMap::iterator iter = aoi_actors_.find(id);
    if (iter != aoi_actors_.end()) {
        iter->second->update(sync);
    }
}

void ActorSight::onMessageSynchronizeNpc(const char *message,
                                         size_t message_size)
{
    gateway::protocol::MessageSynchronizeNpc sync;
    if (thrift_codec::decode(sync, message, message_size) <= 0) {
        LOG_ERROR("actor(%lu) decode MESSAGE_SYNCHRONIZE_NPC failed",
                  actor_->getActorNo());
        return;
    }

    int64_t id = ::atol(sync.npc_id_.c_str());

    AoiNpcMap::iterator iter = aoi_npcs_.find(id);
    if (iter != aoi_npcs_.end()) {
        iter->second->update(sync);
    }
}

void ActorSight::onMessageSynchronizeSceneItem(const char *message,
                                               size_t message_size)
{
    gateway::protocol::MessageSynchronizeSceneItem sync;
    if (thrift_codec::decode(sync, message, message_size) <= 0) {
        LOG_ERROR("actor(%lu) decode MESSAGE_SYNCHRONIZE_SCENE_ITEM failed",
                  actor_->getActorNo());
        return;
    }

    int64_t id = ::atol(sync.id_.c_str());

    AoiSceneItemMap::iterator iter = aoi_scene_items_.find(id);
    if (iter != aoi_scene_items_.end()) {
        iter->second->update(sync);
    }
}

} // namespace robot

