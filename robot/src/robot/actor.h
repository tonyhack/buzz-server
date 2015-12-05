#ifndef BUZZ_ROBOT_ROBOT_ACTOR_H
#define BUZZ_ROBOT_ROBOT_ACTOR_H

#include <stdint.h>
#include <cstddef>
#include <string>
#include <brickred/class_util.h>
#include <brickred/unique_ptr.h>
#include <brickred/function.h>

#include "robot/thrift_codec.h"

namespace robot {

class ActionAutomata;
class ActorSight;
class ClientApp;
class Position;

class Actor {
public:
    typedef brickred::Function<void (const char *, size_t)>
            MessageHandler;
    typedef brickred::Function<void (const void *)>
            EventHandler;

    Actor(int64_t actor_no, const std::string &open_id, int16_t zone_id);
    ~Actor();

    bool logined() const;
    void login(int delay_ms = 0);
    void logout();

    int64_t addEventListener(int event_type, EventHandler handler);
    void removeEventListener(int64_t token);
    void dispatchEvent(int event_type, const void *event);

    int64_t getActorNo() const;
    int64_t getActorId() const;
    const std::string &getActorName() const;
    int getMap() const;
    const Position &getPos() const;
    int getMoveSpeed() const;
    int getGold() const;
    int getVocation() const;
    int getCurrentHp() const;

    const ActorSight &getSight() const;
    void printSight() const;

    bool runningAutomata() const;
    void runAutomata(ActionAutomata *automata);
    void abortAutomata();
    const std::string &getAutomataVariable(const std::string &name);
    void setAutomataVariable(const std::string &name,
                             const std::string &value);
    void move(const Position &dest_pos);
    void abortMove();

    template <class T>
    void sendToLogin(const T &obj, int login_message_type);
    template <class T>
    void sendToGateway(const T &obj, int gateway_message_type);

    void setMessageHandler(int message_type, MessageHandler handler);
    void resetMessageHandler(int message_type);

private:
    BRICKRED_NONCOPYABLE(Actor)

    friend class ClientApp;
    void onNetNewConnection(int64_t socket_id);
    void onNetRecvMessage(int64_t socket_id,
                          const char *buffer, size_t size);
    void onNetPeerClose(int64_t socket_id);
    void onNetError(int64_t socket_id);

    void sendToLogin(thrift_codec::ThriftCodecFunc codec_func,
                     int login_message_type);
    void sendToGateway(thrift_codec::ThriftCodecFunc codec_func,
                       int gateway_message_type);

    class Impl;
    brickred::UniquePtr<Impl> pimpl_;
};

template <class T>
void Actor::sendToLogin(const T &obj, int login_message_type)
{

    thrift_codec::ThriftCodecFunc codec_func = 
        BRICKRED_BIND_TEMPLATE_MEM_FUNC(&T::write, &obj);
    sendToLogin(codec_func, login_message_type);
}

template <class T>
void Actor::sendToGateway(const T &obj, int gateway_message_type)
{
    thrift_codec::ThriftCodecFunc codec_func =
        BRICKRED_BIND_TEMPLATE_MEM_FUNC(&T::write, &obj);
    sendToGateway(codec_func, gateway_message_type);
}

} // namespace robot

#endif

