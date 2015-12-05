#ifndef BUZZ_ROBOT_ROBOT_ACTOR_SIGHT_H
#define BUZZ_ROBOT_ROBOT_ACTOR_SIGHT_H

#include <stdint.h>
#include <cstddef>
#include <map>
#include <vector>

namespace robot {

class Actor;
class AoiEntity;
class AoiRole;
class AoiActor;
class AoiNpc;
class AoiSceneItem;

class ActorSight {
public:
    typedef std::map<int64_t, AoiActor *> AoiActorMap;
    typedef std::map<int64_t, AoiNpc *> AoiNpcMap;
    typedef std::map<int64_t, AoiSceneItem *> AoiSceneItemMap;

    ActorSight(Actor *actor);
    ~ActorSight();

    const AoiActorMap &getAoiActors() const;
    const AoiNpcMap &getAoiNpcs() const;
    const AoiSceneItemMap &getAoiSceneItems() const;

    // WARNING don't keep the pointer
    const AoiActor *getAoiActor(int64_t id) const;
    const AoiNpc *getAoiNpc(int64_t id) const;
    const AoiSceneItem *getAoiSceneItem(int64_t id) const;
    const AoiEntity *getAoiEntity(int64_t id) const;
    const AoiRole *getAoiRole(int64_t id) const;

private:
    void clearSight();
    void onActorEventLoginStart(const void *event);
    void onActorEventLogout(const void *event);
    void onMessageCreateAoiRoles(const char *message,
                                 size_t message_size);
    void onMessageDestoryAoiRoles(const char *message,
                                  size_t message_size);
    void onMessageCreateActorSynchronize(const char *message,
                                         size_t message_size);
    void onMessageCreateNpcSynchronize(const char *message,
                                       size_t message_size);
    void onMessageCreateSceneItemSynchronize(const char *message,
                                             size_t message_size);
    void onMessageDestoryRoleSynchronize(const char *message,
                                         size_t message_size);
    void onMessageRoleMoveSynchronize(const void *message);
    void onMessageActorRelocation(const void *message);
    void onMessageSynchronizeActor(const void *message);
    void onMessageSynchronizeNpc(const char *message,
                                 size_t message_size);
    void onMessageSynchronizeSceneItem(const char *message,
                                       size_t message_size);

    Actor *actor_;
    int64_t login_start_ev_token_;
    int64_t logout_ev_token_;
    std::vector<int64_t> foward_message_ev_tokens_;
    AoiActorMap aoi_actors_;
    AoiNpcMap aoi_npcs_;
    AoiSceneItemMap aoi_scene_items_;
};

} // namespace robot

#endif

