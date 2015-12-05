#include "robot/client_app.h"

#include <unistd.h>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <ctime>
#include <algorithm>
#include <string>
#include <brickred/dynamic_buffer.h>
#include <brickred/io_device.h>
#include <brickred/io_service.h>
#include <brickred/random.h>
#include <brickred/socket_address.h>
#include <brickred/tcp_service.h>
#include <brickred/protocol/http_protocol.h>
#include <brickred/protocol/http_response.h>

#include "robot/logging.h"
#include "robot/config.h"
#include "robot/actor.h"
#include "robot/actor_manager.h"
#include "robot/action/action_automata_manager.h"
#include "robot/map_manager.h"
#include "robot/user_command_handler.h"

namespace robot {

using brickred::DynamicBuffer;
using brickred::IODevice;
using brickred::IOService;
using brickred::Random;
using brickred::SocketAddress;
using brickred::TcpService;
using brickred::UniquePtr;
using brickred::protocol::HttpProtocol;
using brickred::protocol::HttpResponse;

class ClientApp::Impl {
public:
    typedef ClientApp::Protocol Protocol;
    typedef ClientApp::TimerCallback TimerCallback;

    class Context : public TcpService::Context {
    public:
        int64_t actor_no_;
        int protocol_;
        UniquePtr<HttpProtocol> http_protocol_;
    };

    Impl();
    ~Impl();

    bool init();
    void run();

    int64_t connect(int64_t actor_no, const SocketAddress &addr,
                    bool *complete, Protocol::type packetizer);
    void sendMessage(int64_t socket_id, const char *message, size_t size);
    void disconnect(int64_t socket_id);

    int64_t startTimer(int timeout_ms, TimerCallback timer_cb,
                       int call_times);
    void stopTimer(int64_t timer_id);

    uint32_t rand(uint32_t max);
    uint32_t uniformRand(uint32_t min, uint32_t max);

public:
    bool initStdinConsole();
    void printPrompt();
    void stdinReadCallback(IODevice *io_device);

    bool initNetwork();
    void networkNewConnectionCallback(TcpService *service,
        int64_t from_socket_id, int64_t socket_id);
    void networkRecvMessageCallback(TcpService *service,
        int64_t socket_id, DynamicBuffer *buffer);
    void networkPeerCloseCallback(TcpService *service,
        int64_t socket_id);
    void networkErrorCallback(TcpService *service,
        int64_t socket_id, int error);
    void processHttpProtocolMessage(TcpService *service,
        int64_t socket_id, DynamicBuffer *buffer, Actor *actor,
        HttpProtocol *http_protocol);
    void processBuzzProtocolMessage(TcpService *service,
        int64_t socket_id, DynamicBuffer *buffer, Actor *actor);

private:
    IOService io_service_;
    // stdin console
    IODevice stdin_;
    DynamicBuffer stdin_buffer_;
    UserCommandHandler user_command_handler_;
    // network
    TcpService tcp_service_;
    // random
    Random random_generator_;
};

///////////////////////////////////////////////////////////////////////////////
ClientApp::Impl::Impl() :
    tcp_service_(io_service_)
{
}

ClientApp::Impl::~Impl()
{
}

bool ClientApp::Impl::init()
{
    // init singleton
    ActionAutomataManager::getInstance();
    ActorManager::getInstance();
    MapManager::getInstance();

    // load action automatas
    if (ActionAutomataManager::getInstance()->loadActionAutomatas(
            Config::getInstance()->action_file_) == false) { 
        LOG_ERROR("load action automatas from config file(%s) failed",
                  Config::getInstance()->action_file_.c_str());
        return false;
    }

    // load actors
    if (ActorManager::getInstance()->loadActors(
            Config::getInstance()->actor_file_) == false) {
        LOG_ERROR("load actors from config file(%s) failed",
                  Config::getInstance()->actor_file_.c_str());
        return false;
    }

    // load maps
    if (MapManager::getInstance()->loadMaps(
            Config::getInstance()->scene_file_) == false) {
        LOG_ERROR("load maps from config file(%s) failed",
                  Config::getInstance()->scene_file_.c_str());
        return false;
    }

    // init stdin console
    if (initStdinConsole() == false) {
        LOG_ERROR("init stdin console failed");
        return false;
    }

    // init network
    if (initNetwork() == false) {
        LOG_ERROR("init network failed");
        return false;
    }

    return true;
}

void ClientApp::Impl::run()
{
    ::printf("buzz-robot console (enter q to exit)\n");
    printPrompt();
    io_service_.loop();
}

int64_t ClientApp::Impl::connect(int64_t actor_no,
    const SocketAddress &addr, bool *complete, Protocol::type protocol)
{
    if (protocol < Protocol::MIN ||
        protocol >= Protocol::MAX) {
        LOG_ERROR("invalid protocol type");
        return -1;
    }

    int64_t socket_id = tcp_service_.asyncConnect(addr, complete, 3000);
    if (-1 == socket_id) {
        LOG_ERROR("async connect failed");
        return -1;
    }

    UniquePtr<Context> context(new Context());
    context->actor_no_ = actor_no;
    context->protocol_ = protocol;
    if (Protocol::HTTP == protocol) {
        context->http_protocol_.reset(new HttpProtocol());
    }

    if (tcp_service_.setContext(socket_id, context.get()) == false) {
        LOG_ERROR("set socket context failed");
        tcp_service_.closeSocket(socket_id);
        return -1;
    }
    context.release();

    return socket_id;
}

void ClientApp::Impl::sendMessage(int64_t socket_id,
    const char *message, size_t size)
{
    if (tcp_service_.sendMessage(socket_id, message, size) == false) {
        LOG_ERROR("socket(%lx) send message failed", socket_id);
        return;
    }
}

void ClientApp::Impl::disconnect(int64_t socket_id)
{
    tcp_service_.closeSocket(socket_id);
}

int64_t ClientApp::Impl::startTimer(int timeout_ms, TimerCallback timer_cb,
                                    int call_times)
{
    return io_service_.startTimer(timeout_ms, timer_cb, call_times);
}

void ClientApp::Impl::stopTimer(int64_t timer_id)
{
    io_service_.stopTimer(timer_id);
}

uint32_t ClientApp::Impl::rand(uint32_t max)
{
    return random_generator_.nextInt(max);
}

uint32_t ClientApp::Impl::uniformRand(uint32_t min, uint32_t max)
{
    return min + random_generator_.nextInt(max - min + 1);
}

bool ClientApp::Impl::initStdinConsole()
{
    stdin_.setDescriptor(0);

    stdin_.setReadCallback(BRICKRED_BIND_MEM_FUNC(
        &ClientApp::Impl::stdinReadCallback, this));
    stdin_.attachIOService(io_service_);

    return true;
}

void ClientApp::Impl::printPrompt()
{
    ::printf("\033[;34m");
    ::printf("robot> ");
    ::printf("\033[0m");
    fflush(stdout);
}

void ClientApp::Impl::stdinReadCallback(IODevice *io_device)
{
    char buffer[2048];
    char *ret = fgets(buffer, sizeof(buffer), stdin);
    if (NULL == ret) {
        io_service_.quit();
        return;
    }

    char *new_line = ::strstr(buffer, "\n");
    if (NULL == new_line) {
        io_service_.quit();
        return;
    }

    std::string user_command(buffer, new_line - buffer);

    if (user_command == "q" ||
        user_command == "quit" ||
        user_command == "exit") {
        io_service_.quit();
        return;
    } else if (user_command == "daemon") {
        stdin_.detachIOService();
        ::daemon(1, 0);
    } else {
        if (user_command_handler_.parse(user_command) == false) {
            ::printf("invalid command\n");
        }
    }

    printPrompt();
}

bool ClientApp::Impl::initNetwork()
{
    tcp_service_.setNewConnectionCallback(BRICKRED_BIND_MEM_FUNC(
        &ClientApp::Impl::networkNewConnectionCallback, this));
    tcp_service_.setRecvMessageCallback(BRICKRED_BIND_MEM_FUNC(
        &ClientApp::Impl::networkRecvMessageCallback, this));
    tcp_service_.setPeerCloseCallback(BRICKRED_BIND_MEM_FUNC(
        &ClientApp::Impl::networkPeerCloseCallback, this));
    tcp_service_.setErrorCallback(BRICKRED_BIND_MEM_FUNC(
        &ClientApp::Impl::networkErrorCallback, this));

    return true;
}

void ClientApp::Impl::networkNewConnectionCallback(TcpService *service,
    int64_t from_socket_id, int64_t socket_id)
{
    Context *context = (Context *)service->getContext(socket_id);
    if (NULL == context) {
        LOG_ERROR("get context for socket(%lx) failed", socket_id);
        service->closeSocket(socket_id);
        return;
    }
    Actor *actor = ActorManager::getInstance()->getActor(context->actor_no_);
    if (NULL == actor) {
        LOG_ERROR("actor(%lu) not found in actor manager", context->actor_no_);
        service->closeSocket(socket_id);
        return;
    }

    actor->onNetNewConnection(socket_id);
}

void ClientApp::Impl::processHttpProtocolMessage(TcpService *service,
    int64_t socket_id, DynamicBuffer *buffer, Actor *actor,
    HttpProtocol *http_protocol)
{
    for (;;) {
        HttpProtocol::RetCode::type ret =
            http_protocol->recvMessage(buffer);
        if (HttpProtocol::RetCode::WAITING_MORE_DATA == ret) {
            return;

        } else if (HttpProtocol::RetCode::ERROR == ret) {
            LOG_ERROR("actor(%lu) with socket(%lx) "
                      "recv http message failed",
                      actor->getActorNo(), socket_id);
            actor->onNetError(socket_id);
            return;

        } else if (HttpProtocol::RetCode::MESSAGE_READY == ret) {
            HttpResponse response;
            if (http_protocol->retrieveResponse(
                    &response) == false) {
                LOG_ERROR("actor(%lu) with socket(%lx) "
                          "retrieve http response failed",
                          actor->getActorNo(), socket_id);
                actor->onNetError(socket_id);
                return;
            }

            if (response.getStatusCode() != 200) {
                LOG_ERROR("actor(%lu) with socket(%lx) "
                          "http response status code is not 200",
                          actor->getActorNo(), socket_id);
                actor->onNetError(socket_id);
            }
            actor->onNetRecvMessage(socket_id,
                response.getBody().c_str(), response.getBody().size());

            // check connection exists
            if (service->getContext(socket_id) == NULL) {
                return;
            }

        } else {
            break;
        }
    }
}

void ClientApp::Impl::processBuzzProtocolMessage(TcpService *service,
    int64_t socket_id, DynamicBuffer *buffer, Actor *actor)
{
    for (;;) {
        if (buffer->readableBytes() <= 4) {
            return;
        }

        uint32_t packet_size = *(uint32_t *)buffer->readBegin();
        if (buffer->readableBytes() < (size_t)packet_size + 4) {
            return;
        }

        actor->onNetRecvMessage(socket_id,
            buffer->readBegin() + 4, packet_size);

        buffer->read(4 + packet_size);

        // check connection exists
        if (service->getContext(socket_id) == NULL) {
            return;
        }
    }
}

void ClientApp::Impl::networkRecvMessageCallback(TcpService *service,
    int64_t socket_id, DynamicBuffer *buffer)
{
    Context *context = (Context *)service->getContext(socket_id);
    if (NULL == context) {
        LOG_ERROR("get context for socket(%lx) failed", socket_id);
        service->closeSocket(socket_id);
        return;
    }
    Actor *actor = ActorManager::getInstance()->getActor(context->actor_no_);
    if (NULL == actor) {
        LOG_ERROR("actor(%lu) not found in actor manager", context->actor_no_);
        service->closeSocket(socket_id);
        return;
    }

    if (Protocol::HTTP == context->protocol_) {
        processHttpProtocolMessage(service, socket_id, buffer, actor,
                                   context->http_protocol_.get());
    } else if (Protocol::BUZZ == context->protocol_) {
        processBuzzProtocolMessage(service, socket_id, buffer, actor);
    }
}

void ClientApp::Impl::networkPeerCloseCallback(TcpService *service,
    int64_t socket_id)
{
    Context *context = (Context *)service->getContext(socket_id);
    if (NULL == context) {
        LOG_ERROR("get context for socket(%lx) failed", socket_id);
        service->closeSocket(socket_id);
        return;
    }
    Actor *actor = ActorManager::getInstance()->getActor(context->actor_no_);
    if (NULL == actor) {
        LOG_ERROR("actor(%lu) not found in actor manager", context->actor_no_);
        service->closeSocket(socket_id);
        return;
    }

    actor->onNetPeerClose(socket_id);
}

void ClientApp::Impl::networkErrorCallback(TcpService *service,
    int64_t socket_id, int error)
{
    Context *context = (Context *)service->getContext(socket_id);
    if (NULL == context) {
        LOG_ERROR("get context for socket(%lx) failed", socket_id);
        service->closeSocket(socket_id);
        return;
    }
    Actor *actor = ActorManager::getInstance()->getActor(context->actor_no_);
    if (NULL == actor) {
        LOG_ERROR("actor(%lu) not found in actor manager", context->actor_no_);
        service->closeSocket(socket_id);
        return;
    }

    actor->onNetError(socket_id);
}

///////////////////////////////////////////////////////////////////////////////
BRICKRED_SINGLETON2_IMPL(ClientApp)

ClientApp::ClientApp() :
    pimpl_(new Impl())
{
}

ClientApp::~ClientApp()
{
}

bool ClientApp::init()
{
    return pimpl_->init();
}

void ClientApp::run()
{
    pimpl_->run();
}

int64_t ClientApp::connect(int64_t actor_no,
                           const brickred::SocketAddress &addr, bool *complete,
                           Protocol::type protocol)
{
    return pimpl_->connect(actor_no, addr, complete, protocol);
}

void ClientApp::sendMessage(int64_t socket_id,
                            const char *message, size_t size)
{
    pimpl_->sendMessage(socket_id, message, size);
}

void ClientApp::disconnect(int64_t socket_id)
{
    pimpl_->disconnect(socket_id);
}

int64_t ClientApp::startTimer(int timeout_ms, TimerCallback timer_cb,
                              int call_times)
{
    return pimpl_->startTimer(timeout_ms, timer_cb, call_times);
}

void ClientApp::stopTimer(int64_t timer_id)
{
    return pimpl_->stopTimer(timer_id);
}

uint32_t ClientApp::rand(uint32_t max)
{
    return pimpl_->rand(max);
}

uint32_t ClientApp::uniformRand(uint32_t min, uint32_t max)
{
    return pimpl_->uniformRand(min, max);
}

} // namespace robot

