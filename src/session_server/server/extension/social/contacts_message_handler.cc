#include "session_server/server/extension/social/contacts_message_handler.h"

#include <algorithm>
#include <set>
#include <vector>
#include <boost/bind.hpp>

#include "global/logging.h"
#include "global/common_functions.h"
#include "global/global_packet.h"
#include "global/configure/configure.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_social_protocol_types.h"
#include "session_server/server/brief_actor.h"
#include "session_server/server/session_server.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/extension/social/social_actor.h"
#include "session_server/server/extension/social/social_actor_manager.h"

namespace session {

namespace server {

namespace social {

ContactsMessageHandler::ContactsMessageHandler() {}
ContactsMessageHandler::~ContactsMessageHandler() {}

bool ContactsMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_EDIT_SIGNATURE_REQUEST,
      boost::bind(&ContactsMessageHandler::OnMessageSocialContactsEditSignatureRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_ADD_REQUEST,
      boost::bind(&ContactsMessageHandler::OnMessageSocialContactsAddRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_REMOVE_REQUEST,
      boost::bind(&ContactsMessageHandler::OnMessageSocialContactsRemoveRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_MOVE_REQUEST,
      boost::bind(&ContactsMessageHandler::OnMessageSocialContactsMoveRequest,
          this, _1, _2, _3));
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_RECOMMEND_REQUEST,
      boost::bind(&ContactsMessageHandler::OnMessageSocialContactsRecommendRequest,
          this, _1, _2, _3));

  return true;
}

void ContactsMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_EDIT_SIGNATURE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_ADD_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_REMOVE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_MOVE_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_RECOMMEND_REQUEST);
}

static void SendMessageSocialContactsEditSignatureResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageSocialContactsEditSignatureResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_EDIT_SIGNATURE_RESPONSE);
}

static void SendMessageSocialContactsAddResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageSocialContactsAddResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_ADD_RESPONSE);
}

static void SendMessageSocialContactsRemoveResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageSocialContactsRemoveResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_REMOVE_RESPONSE);
}

static void SendMessageSocialContactsMoveResponseFailed(SocialActor *social_actor) {
  gateway::protocol::MessageSocialContactsMoveResponse response;
  response.__set_result_(false);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_MOVE_RESPONSE);
}

static void GetRandomSet(std::set<core::uint32> *random_set, core::uint32 range, size_t count) {
  if (range <= (core::uint32)count * 2) {
    // 如果需要随机的数非常密集, 使用random_shuffle算法
    std::vector<core::uint32> shuffle_vector;

    for (core::uint32 i = 0; i < range; ++i) {
      shuffle_vector.push_back(i);
    }
    std::random_shuffle(shuffle_vector.begin(), shuffle_vector.end());
    for (size_t i = 0; i < count; ++i) {
      random_set->insert(shuffle_vector[i]);
    }
  } else {
    // 否则反复生成随机数, 直到满足需要的个数
    while (random_set->size() < count) {
      core::uint32 random = ExtensionManager::GetInstance()->GetRandom().Random(range);
      random_set->insert(random);
    }
  }
}

void ContactsMessageHandler::OnMessageSocialContactsEditSignatureRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageSocialContactsEditSignatureRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageSocialContactsEditSignatureRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_SOCIAL_CONTACTS_EDIT_SIGNATURE_REQUEST from actor(%lu, %s), "
           "signature_=[%s].",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.signature_.c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::CONTACTS_MODULE) == false) {
    LOG_WARNING("CONTACTS_MODULE is disable.");
    SendMessageSocialContactsEditSignatureResponseFailed(social_actor);
    return;
  }

  // 检查长度
  ssize_t visual_length = global::GetStrVisualLengthUTF8(request.signature_.c_str());
  if (visual_length < 0 || visual_length > MISC_CONF()->contacts_max_signature_length_) {
    LOG_WARNING("Signature is invalid.");
    SendMessageSocialContactsEditSignatureResponseFailed(social_actor);
    return;
  }

  // 关键字过滤
  SessionServerSingleton::GetInstance().GetWordFilter()->Block(&request.signature_);

  social_actor->SetSignature(request.signature_);

  gateway::protocol::MessageSocialContactsEditSignatureResponse response;
  response.__set_result_(true);
  response.__set_signature_(request.signature_);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_EDIT_SIGNATURE_RESPONSE);
}

void ContactsMessageHandler::OnMessageSocialContactsAddRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageSocialContactsAddRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageSocialContactsAddRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_SOCIAL_CONTACTS_ADD_REQUEST from actor(%lu, %s), "
           "actor_name_=[%s], contacts_type_=[%d]",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.actor_name_.c_str(), request.contacts_type_);

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::CONTACTS_MODULE) == false) {
    LOG_WARNING("CONTACTS_MODULE is disable.");
    SendMessageSocialContactsAddResponseFailed(social_actor);
    return;
  }

  bool result = social_actor->AddContacts(request.actor_name_, request.contacts_type_);

  if (false == result) {
    LOG_WARNING("AddContacts failed.");
  }

  gateway::protocol::MessageSocialContactsAddResponse response;
  response.__set_result_(result);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_ADD_RESPONSE);
}

void ContactsMessageHandler::OnMessageSocialContactsRemoveRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageSocialContactsRemoveRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageSocialContactsRemoveRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_SOCIAL_CONTACTS_REMOVE_REQUEST from actor(%lu, %s), "
           "actor_id_=[%s], contacts_type_=[%d]",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.actor_id_.c_str(), request.contacts_type_);

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::CONTACTS_MODULE) == false) {
    LOG_WARNING("CONTACTS_MODULE is disable.");
    SendMessageSocialContactsRemoveResponseFailed(social_actor);
    return;
  }

  bool result = social_actor->RemoveContacts(atol(request.actor_id_.c_str()), request.contacts_type_);

  if (false == result) {
    LOG_WARNING("RemoveContacts failed.");
  }

  gateway::protocol::MessageSocialContactsRemoveResponse response;
  response.__set_result_(result);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_REMOVE_RESPONSE);
}

void ContactsMessageHandler::OnMessageSocialContactsMoveRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageSocialContactsMoveRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageSocialContactsMoveRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_SOCIAL_CONTACTS_MOVE_REQUEST from actor(%lu, %s), "
           "actor_id_=[%s], src_contacts_type_=[%d], dst_contacts_type_=[%d]",
           session_actor->GetActorID(), session_actor->GetName().c_str(),
           request.actor_id_.c_str(), request.src_contacts_type_, request.dst_contacts_type_);

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::CONTACTS_MODULE) == false) {
    LOG_WARNING("CONTACTS_MODULE is disable.");
    SendMessageSocialContactsMoveResponseFailed(social_actor);
    return;
  }

  if (request.src_contacts_type_ == request.dst_contacts_type_) {
    LOG_WARNING("src_contacts_type_ == dst_contacts_type_");
    SendMessageSocialContactsMoveResponseFailed(social_actor);
    return;
  }

  core::uint64 actor_id = atol(request.actor_id_.c_str());

  bool result = social_actor->RemoveContacts(actor_id, request.src_contacts_type_) &&
      social_actor->AddContacts(actor_id, request.dst_contacts_type_, true, false);

  if (false == result) {
    LOG_WARNING("MoveContacts failed.");
  }

  gateway::protocol::MessageSocialContactsMoveResponse response;
  response.__set_result_(result);
  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_MOVE_RESPONSE);
}

void ContactsMessageHandler::OnMessageSocialContactsRecommendRequest(core::uint64 id,
    const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取社交玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(id);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", id);
    return;
  }
  SessionActor *session_actor = social_actor->GetActor();
  if (NULL == session_actor) {
    LOG_ERROR("SessionActor(%lu) is null.", id);
    return;
  }
  BriefActor *brief_actor = SessionActorManager::GetInstance()->GetBriefActor(id);
  if (NULL == brief_actor) {
    LOG_ERROR("actor_id(%lu) does not exist.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageSocialContactsRecommendRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageSocialContactsRecommendRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_SOCIAL_CONTACTS_RECOMMEND_REQUEST from actor(%lu, %s)",
           session_actor->GetActorID(), session_actor->GetName().c_str());

  // 检查功能开启
  if (session_actor->CheckFunctionalityState(entity::FunctionalityType::CONTACTS_MODULE) == false) {
    LOG_WARNING("CONTACTS_MODULE is disable.");
    return;
  }

  bool positive_search_finished = false;
  bool negative_search_finished = false;
  core::int32 positive_search_factor = -1;
  core::int32 negative_search_factor = 0;
  std::set<core::uint64> recommend_actors;
  size_t left_count = (size_t)MISC_CONF()->contacts_recommend_count_;

  while (!(positive_search_finished && negative_search_finished)) {
    const SocialActorManager::ActorIDSet *level_actors = NULL;

    if (positive_search_factor <= negative_search_factor) {
      // 正向查找
      ++positive_search_factor;
      if (positive_search_finished) {
        continue;
      }
      core::int32 level = brief_actor->GetLevel() +
          positive_search_factor * MISC_CONF()->contacts_recommend_level_range_;
      level_actors = SocialActorManager::GetInstance()->GetActorsNearLevel(level);
      if (NULL == level_actors) {
        positive_search_finished = true;
        continue;
      }
    } else {
      // 反向查找
      ++negative_search_factor;
      if (negative_search_finished) {
        continue;
      }
      core::int32 level = brief_actor->GetLevel() -
          positive_search_factor * MISC_CONF()->contacts_recommend_level_range_;
      if (level < 0 ||
          (level_actors = SocialActorManager::GetInstance()->GetActorsNearLevel(level)) == NULL) {
        negative_search_finished = true;
        continue;
      }
    }

    // 随机取玩家ID
    std::set<core::uint64> recommend_actors_to_add;

    if (!level_actors->empty()) {
      if (level_actors->size() <= left_count) {
        recommend_actors_to_add.insert(level_actors->begin(), level_actors->end());
      } else{
        std::set<core::uint32> random_set;
        GetRandomSet(&random_set, (core::uint32)level_actors->size(), left_count);

        for (std::set<core::uint32>::const_iterator iter = random_set.begin();
             iter != random_set.end(); ++iter) {
          SocialActorManager::ActorIDSet::const_iterator iter2 = level_actors->begin();
          std::advance(iter2, *iter);
          recommend_actors_to_add.insert(*iter2);
        }
      }
    }

    // 过滤掉自己和已经是联系人的玩家
    for (std::set<core::uint64>::const_iterator iter = recommend_actors_to_add.begin();
         iter != recommend_actors_to_add.end();) {
      core::uint64 actor_id = *iter;

      if (actor_id == social_actor->GetActorID() ||
          social_actor->IsContacts(actor_id)) {
        recommend_actors_to_add.erase(iter++);
      } else {
        ++iter;
      }
    }

    // 已经满足数量
    recommend_actors.insert(recommend_actors_to_add.begin(), recommend_actors_to_add.end());
    left_count = (size_t)MISC_CONF()->contacts_recommend_count_ - recommend_actors.size();
    if (0 == left_count) {
      break;
    }
  }

  // 发送回复消息
  gateway::protocol::MessageSocialContactsRecommendResponse response;

  for (std::set<core::uint64>::const_iterator iter = recommend_actors.begin();
       iter != recommend_actors.end(); ++iter) {
    core::uint64 actor_id = *iter;

    BriefActor *target_brief_actor = SessionActorManager::GetInstance()->GetBriefActor(actor_id);
    if (NULL == target_brief_actor) {
      LOG_ERROR("actor_id(%lu) does not exist.", actor_id);
      continue;
    }

    gateway::protocol::RecommendContactsData data;
    data.__set_actor_name_(target_brief_actor->GetName());
    data.__set_level_(target_brief_actor->GetLevel());
    response.contacts_.push_back(data);
  }

  social_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SOCIAL_CONTACTS_RECOMMEND_RESPONSE);
}

}  // namespace social

}  // namespace server

}  // namespace session

