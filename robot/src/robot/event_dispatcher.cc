#include "robot/event_dispatcher.h"

#include <list>
#include <ext/hash_map>

namespace robot {

class TokenAllocator {
public:
    TokenAllocator() : value_(0) {}
    ~TokenAllocator() {}

    int64_t getToken(int event_type)
    {   
        if (++value_ < 0) {
            value_ = 0;
        }   
        return ((uint64_t)(value_) << 32) + (uint64_t)event_type;
    }   

private:
    int32_t value_;
};

class EventDispatcher::Impl {
public:
    typedef EventDispatcher::EventHandler EventHandler;
    typedef std::list<EventHandler> EventHandlerList;
    typedef std::list<EventHandler>::iterator EventHandlerIter;
    typedef __gnu_cxx::hash_map<int, EventHandlerList> EventHandlerListMap;
    typedef __gnu_cxx::hash_map<int64_t, EventHandlerIter> EventHandlerIterMap;

    Impl();
    ~Impl();

    int64_t follow(int event_type, EventHandler handler);
    void unfollow(int64_t token);
    void dispatch(int event_type, const void *event);

private:
    EventHandlerListMap event_handler_lists_;
    EventHandlerIterMap event_handler_index_;
    TokenAllocator token_allocator_;
};

///////////////////////////////////////////////////////////////////////////////
EventDispatcher::Impl::Impl()
{
}

EventDispatcher::Impl::~Impl()
{
}

int64_t EventDispatcher::Impl::follow(int event_type, EventHandler handler)
{
    EventHandlerListMap::iterator iter = event_handler_lists_.find(event_type);
    if (event_handler_lists_.end() == iter) {
        EventHandlerList new_list;
        iter = this->event_handler_lists_.insert(
            std::make_pair(event_type, new_list)).first;
    }

    EventHandlerList &event_handler_list = iter->second;
    int64_t token = token_allocator_.getToken(event_type);

    EventHandlerIter iter2 = event_handler_list.insert(
        event_handler_list.end(), handler);
    event_handler_index_[token] = iter2;

    return token;
}

void EventDispatcher::Impl::unfollow(int64_t token)
{
    EventHandlerIterMap::iterator iter = event_handler_index_.find(token);
    if (event_handler_index_.end() == iter) {
        return;
    }

    int event_type = token & 0xFFFFFFFF;
    EventHandlerListMap::iterator iter2 = event_handler_lists_.find(event_type);
    if (event_handler_lists_.end() == iter2) {
        event_handler_index_.erase(iter);
        return;
    }
    EventHandlerList &event_handler_list = iter2->second;

    event_handler_list.erase(iter->second);
    event_handler_index_.erase(iter);
}

void EventDispatcher::Impl::dispatch(int event_type, const void *event)
{
    EventHandlerListMap::iterator iter = event_handler_lists_.find(event_type);
    if (event_handler_lists_.end() == iter) {
        return;
    }
    EventHandlerList &event_handler_list = iter->second;

    for (EventHandlerIter iter2 = event_handler_list.begin();
         iter2 != event_handler_list.end();) {
         EventHandlerIter iter3 = iter2; 
         ++iter3;

        if (*iter2) {
            (*iter2)(event);
        }

        iter2 = iter3;
    }
}

///////////////////////////////////////////////////////////////////////////////
EventDispatcher::EventDispatcher() :
    pimpl_(new Impl())
{
}

EventDispatcher::~EventDispatcher()
{
}

int64_t EventDispatcher::follow(int event_type, EventHandler handler)
{
    return pimpl_->follow(event_type, handler);
}

void EventDispatcher::unfollow(int64_t token)
{
    pimpl_->unfollow(token);
}

void EventDispatcher::dispatch(int event_type, const void *event)
{
    pimpl_->dispatch(event_type, event);
}

} // namespace robot

