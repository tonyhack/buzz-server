#include "robot/actor.h"

#include <cstdio>
#include <cstring>
#include <vector>
#include <ext/hash_map>
#include <brickred/string_util.h>

#include "robot/logging.h"
#include "robot/config.h"
#include "robot/client_app.h"
#include "robot/event_dispatcher.h"
#include "robot/aoi_actor.h"
#include "robot/aoi_npc.h"
#include "robot/actor_sight.h"
#include "robot/actor_event.h"
#include "robot/action/action_move.h"
#include "robot/action/action_automata.h"
#include "robot/position.h"
#include "robot/map.h"
#include "robot/map_manager.h"
#include "protocol/login_protocol_types.h"
#include "protocol/gateway_protocol_types.h"
#include "protocol/gateway_entity_protocol_types.h"
#include "protocol/role_types.h"
#include "protocol/actor_types.h"

namespace robot {

using brickred::Function;
using brickred::UniquePtr;

#define SET_LS_CB(_message_type, _handler) \
    setLoginMessageHandler( \
        login::protocol::MessageType::_message_type, \
        BRICKRED_BIND_MEM_FUNC( \
            &Actor::Impl::_handler, this));

#define RESET_LS_CB(_message_type) \
    resetLoginMessageHandler( \
        login::protocol::MessageType::_message_type);

#define SET_GS_CB(_message_type, _handler) \
    setGatewayMessageHandler( \
        gateway::protocol::MessageType::_message_type, \
        BRICKRED_BIND_MEM_FUNC( \
            &Actor::Impl::_handler, this));

#define RESET_GS_CB(_message_type) \
    resetGatewayMessageHandler( \
        gateway::protocol::MessageType::_message_type);

class Actor::Impl {
public:
    typedef Actor::MessageHandler MessageHandler;
    typedef Actor::EventHandler EventHandler;
    typedef __gnu_cxx::hash_map<int, MessageHandler> MessageHandlerMap;
    typedef __gnu_cxx::hash_map<std::string, std::string,
                brickred::string_util::Hash> AutomataVariableMap;

    Impl(Actor *thiz, int64_t actor_no,
         const std::string &open_id, int16_t zone_id);
    // use init to avoid invalid Actor *
    void init();
    ~Impl();


    bool logined() const;
    void login(int delay_ms);
    void logout();

    int64_t addEventListener(int event_type, EventHandler handler);
    void removeEventListener(int64_t token);
    void dispatchEvent(int event_type, const void *event);

    int64_t getActorNo() const { return actor_no_; }
    int64_t getActorId() const { return actor_id_; }
    const std::string &getActorName() const { return actor_name_; }
    int getMap() const { return map_; }
    const Position &getPos() const { return pos_; }

    int getMoveSpeed() const {
        return role_aoi_attrs_[entity::RoleAoiFields::MOVE_SPEED];
    }
    int getGold() const {
        return actor_client_attrs_[entity::ActorClientFields::GOLD];
    }
    int getVocation() const {
        return role_aoi_attrs_[entity::RoleAoiFields::VOCATION];
    }
    int getCurrentHp() const {
        return role_aoi_attrs_[entity::RoleAoiFields::CURRENT_HP];
    }

    const ActorSight &getSight() const { return *sight_; }
    void printSight() const;

    bool runningAutomata() const;
    void runAutomata(ActionAutomata *automata);
    void abortAutomata();
    const std::string &getAutomataVariable(const std::string &name);
    void setAutomataVariable(const std::string &name,
                             const std::string &value);
    void move(const Position &dest_pos);
    void abortMove();

    void setLoginMessageHandler(int message_type, MessageHandler handler);
    void resetLoginMessageHandler(int message_type);
    void setGatewayMessageHandler(int message_type, MessageHandler handler);
    void resetGatewayMessageHandler(int message_type);

public:
    void onNetNewConnection(int64_t socket_id);
    void onNetRecvMessage(int64_t socket_id,
                          const char *buffer, size_t size);
    void onNetPeerClose(int64_t socket_id);
    void onNetError(int64_t socket_id);
    void sendToLogin(thrift_codec::ThriftCodecFunc codec_func,
                     int login_message_type);
    void sendToGateway(thrift_codec::ThriftCodecFunc codec_func,
                       int gateway_message_type);

public:
    void addLoginDelayTimer(int delay_ms);
    void removeLoginDelayTimer();
    void onLoginDelayTimeout(int64_t timer_id);

    void onAutomataComplete(bool success);
    void onMoveComplete(bool success);

    void connectLoginServer();
    void disconnectLoginServer();
    void onLoginServerConnected();

    void onMessageActorLoginResponse(const char *message,
                                     size_t message_size);

    void connectGatewayServer();
    void disconnectGatewayServer();
    void onGatewayServerConnected();

    void onMessageVerifyQuestion(const char *message,
                                 size_t message_size);
    void onMessageVerifyQuestionResponse(const char *message,
                                         size_t message_size);
    void onMessageLoginResponse(const char *message,
                                size_t message_size);
    void onMessageCreateActor(const char *message,
                              size_t message_size);
    void onMessageSynchronizeActor(const char *message,
                                   size_t message_size);
    void onMessageActorEnterMap(const char *message,
                                size_t message_size);
    void onMessageRoleMoveSynchronize(const char *message,
                                      size_t message_size);
    void onMessageActorRelocation(const char *message,
                                  size_t message_size);

private:
    Actor *thiz_;

    // attribute
    int64_t actor_no_;
    std::string open_id_;
    int16_t zone_id_;
    int64_t actor_id_;
    std::string actor_name_;
    int map_;
    Position pos_;
    std::vector<int32_t> role_aoi_attrs_;
    std::vector<int32_t> actor_aoi_attrs_;
    std::vector<int32_t> actor_client_attrs_; 

    // login
    bool login_started_;
    bool logined_;
    int64_t login_delay_timer_id_;
    int64_t login_server_socket_id_;
    bool login_server_conn_;
    int64_t gateway_server_socket_id_;
    bool gateway_server_conn_;
    std::string game_session_;

    // message handlers
    MessageHandlerMap login_message_handlers_;
    MessageHandlerMap gateway_message_handlers_;
    // dispather
    EventDispatcher event_dispatcher_;

    // sight
    ActorSight *sight_;
    // action
    ActionAutomata *automata_;
    AutomataVariableMap automata_vars_;
    ActionMove *action_move_;
};

///////////////////////////////////////////////////////////////////////////////
Actor::Impl::Impl(Actor *thiz, int64_t actor_no,
                  const std::string &open_id, int16_t zone_id) :
    thiz_(thiz), actor_no_(actor_no), open_id_(open_id), zone_id_(zone_id),
    map_(0), login_started_(false), logined_(false),
    login_delay_timer_id_(-1),
    login_server_socket_id_(-1), login_server_conn_(false),
    gateway_server_socket_id_(-1), gateway_server_conn_(false),
    sight_(NULL),
    automata_(NULL), action_move_(NULL)
{
}

void Actor::Impl::init()
{
    UniquePtr<ActorSight> sight(new ActorSight(thiz_));

    sight_ = sight.get();
    sight.release();
}

Actor::Impl::~Impl()
{
    removeLoginDelayTimer();
    abortAutomata();
    abortMove();
    disconnectLoginServer();
    disconnectGatewayServer();

    delete sight_;
}

bool Actor::Impl::logined() const
{
    return logined_;
}

void Actor::Impl::login(int delay_ms)
{
    if (logined_) {
        return;
    }
    removeLoginDelayTimer();
    disconnectLoginServer();
    disconnectGatewayServer();

    if (delay_ms > 0) {
        addLoginDelayTimer(delay_ms);
        return;
    }
    connectLoginServer();
}

void Actor::Impl::logout()
{
    removeLoginDelayTimer();
    abortAutomata();
    abortMove();
    disconnectLoginServer();
    disconnectGatewayServer();
    LOG_INFO("actor(%lu) logout successfully", actor_no_);
}

int64_t Actor::Impl::addEventListener(int event_type, EventHandler handler)
{
    if (event_type < ActorEventType::MIN ||
        event_type >= ActorEventType::MAX) {
        LOG_ERROR("invalid event type[%d]");
        return -1;
    }

    return event_dispatcher_.follow(event_type, handler);
}

void Actor::Impl::removeEventListener(int64_t token)
{
    event_dispatcher_.unfollow(token);
}

void Actor::Impl::dispatchEvent(int event_type, const void *event)
{
    event_dispatcher_.dispatch(event_type, event);
}

void Actor::Impl::printSight() const
{
    if (logined_ == false) {
        return;
    }

    Map *map = MapManager::getInstance()->getMap(map_);
    if (NULL == map) {
        LOG_ERROR("actor(%lu) current map(%d) is invalid.",
                  actor_no_, map_);
        return;
    }

    int height = map->getHeight();
    int width = map->getWidth();
    std::string map_string = map->getMapString();


    // actors
    for (ActorSight::AoiActorMap::const_iterator iter = 
            sight_->getAoiActors().begin();
         iter != sight_->getAoiActors().end(); ++iter) {
        const AoiActor *aoi_actor = iter->second;
        map_string[aoi_actor->getPos().y_ * width +
                   aoi_actor->getPos().x_] = 'A';
    }

    // npcs
    for (ActorSight::AoiNpcMap::const_iterator iter =
             sight_->getAoiNpcs().begin();
         iter != sight_->getAoiNpcs().end(); ++iter) {
        const AoiNpc *aoi_npc = iter->second;
        map_string[aoi_npc->getPos().y_ * width +
                   aoi_npc->getPos().x_] = 'N';
    }

    // self
    map_string[pos_.y_ * width + pos_.x_] = 'X';

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            char c = map_string[y * width + x];

            if ('A' == c) {
                ::printf("\033[44;37;5m");
                ::printf("A");
                ::printf("\033[0m");
            } else if ('N' == c) {
                ::printf("\033[41;37;5m");
                ::printf("N");
                ::printf("\033[0m");
            } else if ('X' == c) {
                ::printf("\033[42;37;5m");
                ::printf("X");
                ::printf("\033[0m");
            } else {
                ::printf("%c", c);
            }
        }
        ::printf("\n");
    }
}

bool Actor::Impl::runningAutomata() const
{
    return automata_ != NULL;
}

void Actor::Impl::runAutomata(ActionAutomata *automata)
{
    if (logined_ == false) {
        delete automata;
        return;
    }

    abortAutomata();
    abortMove();

    automata_ = automata;
    automata_->setActor(thiz_);
    automata_->setCompleteCallback(BRICKRED_BIND_MEM_FUNC(
        &Actor::Impl::onAutomataComplete, this));
    automata_->start();
}

void Actor::Impl::onAutomataComplete(bool success)
{
    abortAutomata();
    LOG_INFO("actor(%lu) action automata complete[%d]", actor_no_, success);
}

void Actor::Impl::abortAutomata()
{
    if (automata_ != NULL) {
        delete automata_;
        automata_ = NULL;
    }
    automata_vars_.clear();
}

const std::string &Actor::Impl::getAutomataVariable(const std::string &name)
{
    return automata_vars_[name];
}

void Actor::Impl::setAutomataVariable(const std::string &name,
                                      const std::string &value)
{
    automata_vars_[name] = value;
}

void Actor::Impl::move(const Position &dest_pos)
{
    if (logined_ == false) {
        return;
    }

    abortAutomata();
    abortMove();

    UniquePtr<ActionMove> action_move(new ActionMove(dest_pos));
    action_move->setActor(thiz_);
    action_move->setCompleteCallback(BRICKRED_BIND_MEM_FUNC(
        &Actor::Impl::onMoveComplete, this));
    action_move_ = action_move.release();
    action_move_->start();
}

void Actor::Impl::abortMove()
{
    if (action_move_ != NULL) {
        delete action_move_;
        action_move_ = NULL;
    }
}

void Actor::Impl::onMoveComplete(bool success)
{
    abortMove();
    LOG_INFO("actor (%lu) move complete[%d]", actor_no_, success);
}

void Actor::Impl::setLoginMessageHandler(int message_type,
                                         MessageHandler handler)
{
    login_message_handlers_[message_type] = handler;
}

void Actor::Impl::resetLoginMessageHandler(int message_type)
{
    login_message_handlers_.erase(message_type);
}

void Actor::Impl::setGatewayMessageHandler(int message_type,
                                           MessageHandler handler)
{
    gateway_message_handlers_[message_type] = handler;
}

void Actor::Impl::resetGatewayMessageHandler(int message_type)
{
    gateway_message_handlers_.erase(message_type);
}

void Actor::Impl::onNetNewConnection(int64_t socket_id)
{
    if (socket_id == login_server_socket_id_) {
        onLoginServerConnected();
    } else if (socket_id == gateway_server_socket_id_) {
        onGatewayServerConnected();
    }
}

void Actor::Impl::onNetRecvMessage(int64_t socket_id,
                                   const char *buffer, size_t size)
{
    if (socket_id == login_server_socket_id_) {
        // process login server message

        login::protocol::Message message;
        if (thrift_codec::decode(message, buffer, size) <= 0) {
            LOG_ERROR("actor(%lu) decode login server message failed",
                      actor_no_);
            return;
        }

        if (message.type_ <= login::protocol::MessageType::MESSAGE_MIN ||
            message.type_ >= login::protocol::MessageType::MESSAGE_MAX) {
            LOG_ERROR("actor(%lu) recv login server invalid message[%d]",
                      actor_no_, message.type_);
            return;
        }

        const std::map<int, const char*> &msg_to_name =
            login::protocol::_MessageType_VALUES_TO_NAMES;
        std::map<int, const char*>::const_iterator iter =
            msg_to_name.find(message.type_);
        if (iter == msg_to_name.end()) {
            LOG_ERROR("actor(%lu) recv login server invalid message[%d]",
                      actor_no_, message.type_);
            return;
        }

        LOG_TRACE("actor(%lu) recv login server message %s(%d)",
                  actor_no_, iter->second, message.type_);

        MessageHandlerMap::iterator iter2 =
            login_message_handlers_.find(message.type_);
        if (login_message_handlers_.end() == iter2) {
            return;
        }
        iter2->second(message.message_.c_str(), message.message_.size());

    } else if (socket_id == gateway_server_socket_id_) {
        // process gateway server message

        gateway::protocol::Message message;
        if (thrift_codec::decode(message, buffer, size) <= 0) {
            LOG_ERROR("actor(%lu) decode gateway server message failed",
                      actor_no_);
            return;
        }

        if (message.type_ < gateway::protocol::MessageType::MESSAGE_MIN ||
            message.type_ >= gateway::protocol::MessageType::MESSAGE_MAX) {
            LOG_ERROR("actor(%lu) recv gateway server invalid message[%d]",
                      actor_no_, message.type_);
            return;
        }

        const std::map<int, const char*> &msg_to_name =
            gateway::protocol::_MessageType_VALUES_TO_NAMES;
        std::map<int, const char*>::const_iterator iter =
            msg_to_name.find(message.type_);
        if (iter == msg_to_name.end()) {
            LOG_ERROR("actor(%lu) recv gateway server invalid message[%d]",
                      actor_no_, message.type_);
            return;
        }

        LOG_TRACE("actor(%lu) recv gateway server message %s(%d)",
                  actor_no_, iter->second, message.type_);

        MessageHandlerMap::iterator iter2 =
            gateway_message_handlers_.find(message.type_);
        if (gateway_message_handlers_.end() == iter2) {
            return;
        }
        iter2->second(message.message_.c_str(), message.message_.size());
    }
}

void Actor::Impl::onNetPeerClose(int64_t socket_id)
{
    if (socket_id == login_server_socket_id_) {
        LOG_TRACE("actor(%lu) login server connection peer closed",
                  actor_no_);
        disconnectLoginServer();
    } else if(socket_id == gateway_server_socket_id_) {
        LOG_TRACE("actor(%lu) gateway server connection peer closed",
                  actor_no_);
        disconnectGatewayServer();
    }
}

void Actor::Impl::onNetError(int64_t socket_id)
{
    if (socket_id == login_server_socket_id_) {
        LOG_ERROR("actor(%lu) login server connection error", actor_no_);
        disconnectLoginServer();
    } else if (socket_id == gateway_server_socket_id_) {
        LOG_ERROR("actor(%lu) gateway server connection error", actor_no_);
        disconnectGatewayServer();
    }
}

void Actor::Impl::sendToLogin(thrift_codec::ThriftCodecFunc codec_func,
                              int login_message_type)
{
    if (!login_server_conn_) {
        LOG_ERROR("actor(%lu) not connect to login server", actor_no_);
        return;
    }

    char buffer[10240];
    uint32_t buffer_size = 0;

    // encode inner
    buffer_size = thrift_codec::encode(codec_func, buffer, sizeof(buffer));
    if (buffer_size <= 0) {
        LOG_ERROR("actor(%lu) encode inner message failed", actor_no_);
        return;
    }

    login::protocol::Message message;
    message.__set_type_((login::protocol::MessageType::type)
                         login_message_type);
    message.message_.assign(buffer, buffer_size);

    // encode wrapper
    buffer_size = thrift_codec::encode(message, buffer, sizeof(buffer));
    if (buffer_size <= 0) {
        LOG_ERROR("actor(%lu) encode wrapper message failed", actor_no_);
        return;
    }

    // send http request 
    char http_request[10240];
    int http_header_size = snprintf(http_request, sizeof(http_request),
        "POST / HTTP/1.1\r\n"
        "Content-Length: %d\r\n"
        "Content-Type: application/x-thrift\r\n\r\n", buffer_size);
    if (http_header_size + buffer_size > (int)sizeof(http_request)) {
        LOG_ERROR("actor(%lu) http content is too large", actor_no_);
        return;
    }
    ::memcpy(http_request + http_header_size, buffer, buffer_size);

    ClientApp::getInstance()->sendMessage(login_server_socket_id_,
                                      http_request, http_header_size + buffer_size);
}

void Actor::Impl::sendToGateway(thrift_codec::ThriftCodecFunc codec_func,
                                int gateway_message_type)
{
    if (!gateway_server_conn_) {
        LOG_ERROR("actor(%lu) not connnect to gateway server", actor_no_);
        return;
    }

    char buffer[10240];
    uint32_t buffer_size = 0;

    // encode inner
    buffer_size = thrift_codec::encode(codec_func, buffer, sizeof(buffer));
    if (buffer_size <= 0) {
        LOG_ERROR("actor(%lu) encode inner message failed", actor_no_);
        return;
    }

    gateway::protocol::Message message;
    message.__set_type_((gateway::protocol::MessageType::type)
                         gateway_message_type);
    message.message_.assign(buffer, buffer_size);

    // encode wrapper
    buffer_size = thrift_codec::encode(message, buffer, sizeof(buffer));
    if (buffer_size <= 0) {
        LOG_ERROR("actor(%lu) encode wrapper message failed", actor_no_);
        return;
    }

    ClientApp::getInstance()->sendMessage(gateway_server_socket_id_,
                                      (const char *)&buffer_size,
                                      sizeof(buffer_size));
    ClientApp::getInstance()->sendMessage(gateway_server_socket_id_,
                                      buffer, buffer_size);
}

void Actor::Impl::addLoginDelayTimer(int delay_ms)
{
    ClientApp::getInstance()->startTimer(delay_ms, BRICKRED_BIND_MEM_FUNC(
        &Actor::Impl::onLoginDelayTimeout, this), 1);
}

void Actor::Impl::removeLoginDelayTimer()
{
    if (login_delay_timer_id_ != -1) {
        ClientApp::getInstance()->stopTimer(login_delay_timer_id_);
        login_delay_timer_id_ = -1;
    }
}

void Actor::Impl::onLoginDelayTimeout(int64_t timer_id)
{
    login_delay_timer_id_ = -1;
    login(0);
}

void Actor::Impl::connectLoginServer()
{
    LOG_TRACE("actor(%lu) start to connect login server", actor_no_);

    bool complete = false;
    login_server_socket_id_ =
        ClientApp::getInstance()->connect(actor_no_,
            Config::getInstance()->login_server_addr_,
            &complete, ClientApp::Protocol::HTTP);
    if (-1 == login_server_socket_id_) {
        LOG_ERROR("actor(%lu) connect to login server failed", actor_no_);
        return;
    }

    if (complete) {
        onLoginServerConnected();
    }
}

void Actor::Impl::disconnectLoginServer()
{
    if (login_server_socket_id_ != -1) {
        ClientApp::getInstance()->disconnect(login_server_socket_id_);
        login_server_socket_id_ = -1;
    }
    login_server_conn_ = false;
    login_message_handlers_.clear();
}

void Actor::Impl::onLoginServerConnected()
{
    LOG_TRACE("actor(%lu) login server connected", actor_no_);

    login_server_conn_ = true;

    login::protocol::MessageActorLoginRequest request;
    request.__set_open_id_(open_id_);
    request.__set_zone_id_(zone_id_);
    thiz_->sendToLogin(request,
        login::protocol::MessageType::MESSAGE_ACTOR_LOGIN_REQUEST);

    SET_LS_CB(MESSAGE_ACTOR_LOGIN_RESPONSE, onMessageActorLoginResponse);
}

void Actor::Impl::onMessageActorLoginResponse(const char *message,
                                              size_t message_size)
{
    RESET_LS_CB(MESSAGE_ACTOR_LOGIN_RESPONSE);

    login::protocol::MessageActorLoginResponse response;
    if (thrift_codec::decode(response, message, message_size) <= 0) {
        LOG_ERROR("actor(%lu) decode MESSAGE_ACTOR_LOGIN_RESPONSE failed",
                  actor_no_);
        return;
    }

    LOG_TRACE("success_=[%d], actor_id_=[%s], game_session_=[%s], "
              "actor_name_=[%s]",
              response.success_, response.actor_id_.c_str(),
              response.game_session_.c_str(), response.actor_name_.c_str());

    if (response.success_ == false) {
        LOG_ERROR("actor(%lu) login refused", actor_no_);
        return;
    }

    actor_id_ = ::atol(response.actor_id_.c_str());
    actor_name_ = response.actor_name_;
    game_session_ = response.game_session_;

    connectGatewayServer();
}

void Actor::Impl::connectGatewayServer()
{
    LOG_TRACE("actor(%lu) start to connect gateway server", actor_no_);

    bool complete = false;
    gateway_server_socket_id_ =
        ClientApp::getInstance()->connect(actor_no_,
            Config::getInstance()->gateway_server_addr_,
            &complete, ClientApp::Protocol::BUZZ);
    if (-1 == gateway_server_socket_id_) {
        LOG_ERROR("actor(%lu) connect to gateway server failed", actor_no_);
        return;
    }

    if (complete) {
        onGatewayServerConnected();
    }
}

void Actor::Impl::disconnectGatewayServer()
{
    if (gateway_server_socket_id_ != -1) {
        ClientApp::getInstance()->disconnect(gateway_server_socket_id_);
        gateway_server_socket_id_ = -1;
    }
    gateway_server_conn_ = false;

    if (login_started_) {
        dispatchEvent(ActorEventType::LOGOUT, NULL);
    }

    logined_ = false;
    login_started_ = false;
    gateway_message_handlers_.clear();
}

void Actor::Impl::onGatewayServerConnected()
{
    LOG_TRACE("actor(%lu) gatway server connected", actor_no_);

    gateway_server_conn_ = true;

    SET_GS_CB(MESSAGE_VERIFY_QUESTION, onMessageVerifyQuestion);
}

static std::string strToBin(const char *str)
{
#define GET_HIGH_BYTE(value) ((value) >> 4)
#define GET_LOW_BYTE(value) ((value) & 0x0F)

    char temp[512] = {0};
    int str_len = strlen(str);
    const unsigned char *ustr = (const unsigned char *)str;

    for (int i = 0, j = 0; i < str_len; ++i) {
        unsigned char high = GET_HIGH_BYTE(ustr[i]);
        unsigned char low = GET_LOW_BYTE(ustr[i]);

        if (high <= 9) {
            temp[j++] = high + 48; 
        } else {
            temp[j++] = high + 55; 
        }   

        if (low <= 9) {
            temp[j++] = low + 48; 
        } else {
            temp[j++] = low + 55; 
        }   
    }

    return std::string(temp);

#undef GET_HIGH_BYTE
#undef GET_LOW_BYTE
}

static std::string simpleEncrypt(const char *in,
                                 const char *key_buffer, int code)
{
    char temp[512] = {0};
    strncpy(temp, in, sizeof(temp));

    int in_len = strlen(temp);

    char *key = (char *)key_buffer + ((code + 7) % 19);
    int key_len = strlen(key);

    int rand[9] = { 22, 12, 8, 33, 1, 9, 7, 15, 18 };

    int rand_pos = 0;
    int key_pos = 0;
    for (int i = 0; i < in_len; ++i, ++key_pos, ++rand_pos) {
        key_pos = key_pos % key_len;
        rand_pos = rand_pos % (sizeof(rand) / sizeof(int));

        temp[i] += key[key_pos] % 23;
        temp[i] += rand[rand_pos];
    }

    return strToBin(temp);
}

void Actor::Impl::onMessageVerifyQuestion(const char *message,
                                          size_t message_size)
{
    RESET_GS_CB(MESSAGE_VERIFY_QUESTION);

    gateway::protocol::MessageVerifyQuestion question;
    if (thrift_codec::decode(question, message, message_size) <= 0) {
        LOG_ERROR("actor(%lu) decode MESSAGE_VERIFY_QUESTION failed",
                  actor_no_);
        return;
    }

    LOG_TRACE("code_=[%d], key_=[x]", question.code_);

    gateway::protocol::MessageVerifyQuestionRequest request;
    request.__set_value_(simpleEncrypt("dreamfire", question.key_.c_str(),
                                       question.code_));
    thiz_->sendToGateway(request,
        gateway::protocol::MessageType::MESSAGE_VERIFY_QUESTION_REQUEST);

    SET_GS_CB(MESSAGE_VERIFY_QUESTION_RESPONSE,
              onMessageVerifyQuestionResponse);
}

void Actor::Impl::onMessageVerifyQuestionResponse(const char *message,
                                                  size_t message_size)
{
    RESET_GS_CB(MESSAGE_VERIFY_QUESTION_RESPONSE);

    gateway::protocol::MessageVerifyQuestionResponse response;
    if (thrift_codec::decode(response, message, message_size) <= 0) {
        LOG_ERROR("actor(%lu) decode MESSAGE_VERIFY_QUESTION_RESPONSE failed",
                  actor_no_);
        return;
    }

    LOG_TRACE("pass_=[%d]", response.pass_);

    if (response.pass_ == false) {
        LOG_ERROR("actor(%lu) login refused", actor_no_);
        disconnectGatewayServer();
        return;
    }

    gateway::protocol::MessageLoginRequest request;
    request.__set_actor_id_(brickred::string_util::toString(actor_id_));
    request.__set_session_key_(game_session_);
    thiz_->sendToGateway(request,
        gateway::protocol::MessageType::MESSAGE_LOGIN_REQUEST);

    login_started_ = true;
    dispatchEvent(ActorEventType::LOGIN_START, NULL);

    SET_GS_CB(MESSAGE_LOGIN_RESPONSE, onMessageLoginResponse);
    SET_GS_CB(MESSAGE_CREATE_ACTOR, onMessageCreateActor);
    SET_GS_CB(MESSAGE_SYNCHRONIZE_ACTOR, onMessageSynchronizeActor);
    SET_GS_CB(MESSAGE_ACTOR_ENTER_MAP, onMessageActorEnterMap);
    SET_GS_CB(MESSAGE_ROLE_MOVE_SYNCHRONIZE, onMessageRoleMoveSynchronize);
    SET_GS_CB(MESSAGE_ACTOR_RELOCATION, onMessageActorRelocation);
}

void Actor::Impl::onMessageLoginResponse(const char *message,
                                         size_t message_size)
{
    RESET_GS_CB(MESSAGE_LOGIN_RESPONSE);

    gateway::protocol::MessageLoginResponse response;
    if (thrift_codec::decode(response, message, message_size) <= 0) {
        LOG_ERROR("actor(%lu) decode MESSAGE_LOGIN_RESPONSE failed",
                  actor_no_);
        return;
    }

    LOG_TRACE("logined_=[%d]", response.logined_);

    if (response.logined_ == false) {
        LOG_ERROR("actor(%lu) login refused", actor_no_);
        disconnectGatewayServer();
        return;
    }

    logined_ = true;
    LOG_INFO("actor(%lu) login successfully", actor_no_);
    dispatchEvent(ActorEventType::LOGIN_FINISH, NULL);
}

void Actor::Impl::onMessageCreateActor(const char *message,
                                       size_t message_size)
{
    gateway::protocol::MessageCreateActor sync;
    if (thrift_codec::decode(sync, message, message_size) <= 0) {
        LOG_ERROR("actor(%lu) decode MESSAGE_CREATE_ACTOR failed",
                  actor_no_);
        return;
    }

    role_aoi_attrs_ = sync.role_aoi_fields_;
    actor_aoi_attrs_ = sync.actor_aoi_fields_;
    actor_client_attrs_ = sync.actor_client_fields_;
}

void Actor::Impl::onMessageSynchronizeActor(const char *message,
                                            size_t message_size)
{
    gateway::protocol::MessageSynchronizeActor sync;
    if (thrift_codec::decode(sync, message, message_size) <= 0) {
        LOG_ERROR("actor(%lu) decode MESSAGE_SYNCHRONIZE_ACTOR failed",
                  actor_no_);
        return;
    }

    if (::atol(sync.actor_id_.c_str()) != actor_id_) {
        dispatchEvent(ActorEventType::FORWARD_MESSAGE_SYNCHRONIZE_ACTOR,
                      &sync);
        return;
    }

    if (sync.__isset.role_aoi_fields_) {
        for (size_t i = 0; i < sync.role_aoi_fields_.size(); ++i) {
            int field = sync.role_aoi_fields_[i].field_;
            int value = sync.role_aoi_fields_[i].value_;

            if (field >= 0 && field < (int)role_aoi_attrs_.size()) {
                role_aoi_attrs_[field] = value;
            }
        }
    }

    if (sync.__isset.actor_aoi_fields_) {
        for (size_t i = 0; i < sync.actor_aoi_fields_.size(); ++i) {
            int field = sync.actor_aoi_fields_[i].field_;
            int value = sync.actor_aoi_fields_[i].value_;

            if (field >= 0 && field < (int)actor_aoi_attrs_.size()) {
                actor_aoi_attrs_[field] = value;
            }
        }
    }

    if (sync.__isset.actor_client_fields_) {
        for (size_t i = 0; i < sync.actor_client_fields_.size(); ++i) {
            int field = sync.actor_client_fields_[i].field_;
            int value = sync.actor_client_fields_[i].value_;

            if (field >= 0 && field < (int)actor_client_attrs_.size()) {
                actor_client_attrs_[field] = value;
            }
        }
    }
}

void Actor::Impl::onMessageActorEnterMap(const char *message,
                                         size_t message_size)
{
    gateway::protocol::MessageActorEnterMap sync;
    if (thrift_codec::decode(sync, message, message_size) <= 0) {
        LOG_ERROR("actor(%lu) decode MESSAGE_ACTOR_ENTER_MAP failed",
                  actor_no_);
        return;
    }

    LOG_TRACE("map_=[%d], x_=[%d], y_=[%d]", sync.map_, sync.x_, sync.y_);

    map_ = sync.map_;
    pos_.x_ = sync.x_;
    pos_.y_ = sync.y_;
}

void Actor::Impl::onMessageRoleMoveSynchronize(const char *message,
                                               size_t message_size)
{
    gateway::protocol::MessageRoleMoveSynchronize sync;
    if (thrift_codec::decode(sync, message, message_size) <= 0) {
        LOG_ERROR("actor(%lu) decode MESSAGE_ROLE_MOVE_SYNCHRONIZE failed",
                  actor_no_);
        return;
    }

    LOG_TRACE("id_=[%s], type_=[%d], dir_=[%d]",
              sync.id_.c_str(), sync.type_, sync.dir_);

    if (::atol(sync.id_.c_str()) != actor_id_) {
        dispatchEvent(ActorEventType::FORWARD_MESSAGE_ROLE_MOVE_SYNCHRONIZE,
                      &sync);
    } else {
        pos_.moveByDirection(sync.dir_);
        dispatchEvent(ActorEventType::SELF_MOVE, NULL);
    }

}

void Actor::Impl::onMessageActorRelocation(const char *message,
                                           size_t message_size)
{
    gateway::protocol::MessageActorRelocation sync;
    if (thrift_codec::decode(sync, message, message_size) <= 0) {
        LOG_ERROR("actor(%lu) decode MESSAGE_ACTOR_RELOCATION failed",
                  actor_no_);
        return;
    }

    if (::atol(sync.id_.c_str()) != actor_id_) {
        dispatchEvent(ActorEventType::FORWARD_MESSAGE_ACTOR_RELOCATION,
                      &sync);
    } else {
        pos_.x_ = sync.x_;
        pos_.y_ = sync.y_;
        dispatchEvent(ActorEventType::SELF_RELOCATION, NULL);
    }
}

///////////////////////////////////////////////////////////////////////////////
Actor::Actor(int64_t actor_no, const std::string &open_id, int16_t zone_id) :
    pimpl_(new Impl(this, actor_no, open_id, zone_id))
{
    pimpl_->init();
}

Actor::~Actor()
{
}

bool Actor::logined() const
{
    return pimpl_->logined();
}

void Actor::login(int delay_ms)
{
    pimpl_->login(delay_ms);
}

void Actor::logout()
{
    pimpl_->logout();
}

int64_t Actor::addEventListener(int event_type, EventHandler handler)
{
    return pimpl_->addEventListener(event_type, handler);
}

void Actor::removeEventListener(int64_t token)
{
    pimpl_->removeEventListener(token);
}

void Actor::dispatchEvent(int event_type, const void *event)
{
    pimpl_->dispatchEvent(event_type, event);
}

int64_t Actor::getActorNo() const
{
    return pimpl_->getActorNo();
}

int64_t Actor::getActorId() const
{
    return pimpl_->getActorId();
}

const std::string &Actor::getActorName() const
{
    return pimpl_->getActorName();
}

int Actor::getMap() const
{
    return pimpl_->getMap();
}

const Position &Actor::getPos() const
{
    return pimpl_->getPos();
}

int Actor::getMoveSpeed() const
{
    return pimpl_->getMoveSpeed();
}

int Actor::getGold() const
{
    return pimpl_->getGold();
}

int Actor::getVocation() const
{
    return pimpl_->getVocation();
}

int Actor::getCurrentHp() const
{
    return pimpl_->getCurrentHp();
}

const ActorSight &Actor::getSight() const
{
    return pimpl_->getSight();
}

void Actor::printSight() const
{
    pimpl_->printSight();
}

bool Actor::runningAutomata() const
{
    return pimpl_->runningAutomata();
}

void Actor::runAutomata(ActionAutomata *automata)
{
    pimpl_->runAutomata(automata);
}

void Actor::abortAutomata()
{
    pimpl_->abortAutomata();
}

void Actor::setAutomataVariable(const std::string &name,
                                const std::string &value)
{
    pimpl_->setAutomataVariable(name, value);
}

const std::string &Actor::getAutomataVariable(const std::string &name)
{
    return pimpl_->getAutomataVariable(name);
}

void Actor::move(const Position &dest_pos)
{
    pimpl_->move(dest_pos);
}

void Actor::abortMove()
{
    pimpl_->abortMove();
}

void Actor::onNetNewConnection(int64_t socket_id)
{
    pimpl_->onNetNewConnection(socket_id);
}

void Actor::onNetRecvMessage(int64_t socket_id,
                             const char *buffer, size_t size)
{
    pimpl_->onNetRecvMessage(socket_id, buffer, size);
}

void Actor::onNetPeerClose(int64_t socket_id)
{
    pimpl_->onNetPeerClose(socket_id);
}

void Actor::onNetError(int64_t socket_id)
{
    pimpl_->onNetError(socket_id);
}

void Actor::sendToLogin(thrift_codec::ThriftCodecFunc codec_func,
                        int login_message_type)
{
    pimpl_->sendToLogin(codec_func, login_message_type);
}

void Actor::sendToGateway(thrift_codec::ThriftCodecFunc codec_func,
                          int gateway_message_type)
{
    pimpl_->sendToGateway(codec_func, gateway_message_type);
}

void Actor::setMessageHandler(int message_type, MessageHandler handler)
{
    pimpl_->setGatewayMessageHandler(message_type, handler);
}

void Actor::resetMessageHandler(int message_type)
{
    pimpl_->resetGatewayMessageHandler(message_type);
}


} // namespace robot

