#include "robot/actor_manager.h"

#include <cstdlib>
#include <ext/hash_map>
#include <tinyxml2/tinyxml2.h>

#include "robot/logging.h"
#include "robot/actor.h"
#include "robot/client_app.h"

namespace robot {

using tinyxml2::XML_NO_ERROR;
using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;
using brickred::UniquePtr;

class ActorManager::Impl {
public:
    typedef __gnu_cxx::hash_map<int64_t, Actor *> ActorMap;

    Impl();
    ~Impl();

    bool loadActors(const std::string &file);
    Actor *getActor(int64_t actor_no);

private:
    ActorMap actors_;
};

///////////////////////////////////////////////////////////////////////////////
ActorManager::Impl::Impl()
{
}

ActorManager::Impl::~Impl()
{
    for (ActorMap::iterator iter = actors_.begin();
         iter != actors_.end(); ++iter) {
        delete iter->second;
    }
}

bool ActorManager::Impl::loadActors(const std::string &file)
{
    XMLDocument doc;
    if (doc.LoadFile(file.c_str()) != XML_NO_ERROR) {
        LOG_ERROR("load config file(%s) failed", file.c_str());
        return false;
    }

    XMLElement *config_node = doc.FirstChildElement("config");
    if (NULL == config_node) {
        LOG_ERROR("/config not found");
        return false;
    }

    const char *attr = NULL;
    size_t count = 0;

    for (XMLElement *actor_node = config_node->FirstChildElement("actor");
         actor_node != NULL;
         actor_node = actor_node->NextSiblingElement("actor")) {
        ++count;

        attr = actor_node->Attribute("no");
        if (NULL == attr) {
            LOG_ERROR("count[%zd] /config/actor[no] not found", count);
            return false;
        }
        int64_t actor_no = ::atol(attr);

        attr = actor_node->Attribute("open_id");
        if (NULL == attr) {
            LOG_ERROR("count[%zd] /config/actor[open_id] not found", count);
            return false;
        }
        std::string open_id = attr;

        attr = actor_node->Attribute("zone_id");
        if (NULL == attr) {
            LOG_ERROR("count[%zd] /config/actor[zone_id] not found", count);
            return false;
        }
        int zone_id = ::atoi(attr);

        // check actor_no dumplicate
        if (getActor(actor_no) != NULL) {
            LOG_ERROR("count[%zd] /config/actor[no] dumplicate", count);
            return false;
        }

        UniquePtr<Actor> actor(new Actor(actor_no, open_id, zone_id));
        actors_.insert(std::make_pair(actor_no, actor.get()));
        actor.release();
    }

    return true;
}

Actor *ActorManager::Impl::getActor(int64_t actor_no)
{
    ActorMap::iterator iter = actors_.find(actor_no);
    if (actors_.end() == iter) {
        return NULL;
    }
    return iter->second;
}

///////////////////////////////////////////////////////////////////////////////
ActorManager::ActorManager() :
    pimpl_(new Impl())
{
}

ActorManager::~ActorManager()
{
}

bool ActorManager::loadActors(const std::string &file)
{
    return pimpl_->loadActors(file);
}

Actor *ActorManager::getActor(int64_t actor_no)
{
    return pimpl_->getActor(actor_no);
}

} // namespace robot

