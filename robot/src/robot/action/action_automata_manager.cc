#include "robot/action/action_automata_manager.h"

#include <cstddef>
#include <cstdlib>
#include <ext/hash_map>
#include <tinyxml2/tinyxml2.h>

#include "robot/logging.h"
#include "robot/config.h"
#include "robot/action/action_automata.h"

namespace robot {

using tinyxml2::XML_NO_ERROR;
using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;
using brickred::UniquePtr;

class ActionAutomataManager::Impl {
public:
    typedef __gnu_cxx::hash_map<int, ActionAutomata *> ActionAutomataMap;

    Impl();
    ~Impl();

    bool loadActionAutomatas(const std::string &file);
    ActionAutomata *getActionAutomata(int id);

private:
    ActionAutomataMap automatas_;
};

///////////////////////////////////////////////////////////////////////////////
ActionAutomataManager::Impl::Impl()
{
}

ActionAutomataManager::Impl::~Impl()
{
    for (ActionAutomataMap::iterator iter = automatas_.begin();
         iter != automatas_.end(); ++iter) {
        delete iter->second;
    }
}

bool ActionAutomataManager::Impl::loadActionAutomatas(const std::string &file)
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

    for (XMLElement *action_node = config_node->FirstChildElement("action");
         action_node != NULL;
         action_node = action_node->NextSiblingElement("action")) {
        ++count;

        attr = action_node->Attribute("id");
        if (NULL == attr) {
            LOG_ERROR("count[%zd] /config/action[id] not found", count);
            return false;
        }
        int id = ::atoi(attr);

        attr = action_node->Attribute("file");
        if (NULL == attr) {
            LOG_ERROR("count[%zd] /config/action[file] not found", count);
            return false;
        }
        std::string file = Config::getInstance()->config_dir_ + attr;

        // check id dumplicate
        if (getActionAutomata(id) != NULL) {
            LOG_ERROR("count[%zd] /config/action[id] dumplicate", count);
            return false;
        }

        UniquePtr<ActionAutomata> automata(new ActionAutomata(id));
        if (automata->load(file) == false) {
            LOG_ERROR("count[%zd] load action file(%s) failed",
                      count, file.c_str());
            return false;
        }

        automatas_.insert(std::make_pair(id, automata.get()));
        automata.release();
    }

    return true;
}

ActionAutomata *ActionAutomataManager::Impl::getActionAutomata(int id)
{
    ActionAutomataMap::iterator iter = automatas_.find(id);
    if (automatas_.end() == iter) {
        return NULL;
    }
    return iter->second;
}

///////////////////////////////////////////////////////////////////////////////
ActionAutomataManager::ActionAutomataManager() :
    pimpl_(new Impl())
{
}

ActionAutomataManager::~ActionAutomataManager()
{
}

bool ActionAutomataManager::loadActionAutomatas(const std::string &file)
{
    return pimpl_->loadActionAutomatas(file);
}

ActionAutomata *ActionAutomataManager::getActionAutomata(int id)
{
    return pimpl_->getActionAutomata(id);
}

} // namespace robot

