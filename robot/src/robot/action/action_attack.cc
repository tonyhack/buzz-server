#include "robot/action/action_attack.h"

#include <cstddef>
#include <cstdlib>
#include <algorithm>
#include <brickred/unique_ptr.h>
#include <brickred/string_util.h>

#include "robot/logging.h"
#include "robot/client_app.h"
#include "robot/config.h"
#include "robot/thrift_codec.h"
#include "robot/actor.h"
#include "robot/actor_sight.h"
#include "robot/aoi_role.h"
#include "robot/ai/target_selector.h"
#include "robot/ai/skill_selector.h"
#include "robot/action/action_approach_target.h"
#include "protocol/gateway_protocol_types.h"
#include "protocol/gateway_skill_protocol_types.h"

namespace robot {

using brickred::UniquePtr;

ActionAttack::ActionAttack(TargetSelector *target_selector,
                           SkillSelector *skill_selector) :
    target_selector_(target_selector), skill_selector_(skill_selector),
    action_approach_target_(NULL), target_id_(-1), timer_id_(-1)
{
}

ActionAttack::~ActionAttack()
{
    reset();

    delete target_selector_;
    delete skill_selector_;
}

Action *ActionAttack::clone()
{
    UniquePtr<TargetSelector> target_selector(target_selector_->clone());
    UniquePtr<SkillSelector> skill_selector(skill_selector_->clone());
    UniquePtr<ActionAttack> action_attack(
        new ActionAttack(target_selector.get(), skill_selector.get()));
    target_selector.release();
    skill_selector.release();

    ActionAttack *raw_action_attack = action_attack.release();
    return raw_action_attack;
}

void ActionAttack::start()
{
    target_id_ = target_selector_->getTarget(actor_);
    if (-1 == target_id_) {
        LOG_DEBUG("actor(%ld) get target failed", actor_->getActorNo());
        return finish(false);
    }

    attack();
}

void ActionAttack::reset()
{
    target_id_ = -1;

    if (action_approach_target_ != NULL) {
        delete action_approach_target_;
        action_approach_target_ = NULL;
    }

    if (timer_id_ != -1) {
        ClientApp::getInstance()->stopTimer(timer_id_);
        timer_id_ = -1;
    }
}

void ActionAttack::attack()
{
    const AoiRole *target = actor_->getSight().getAoiRole(target_id_);
    if (NULL == target) {
        return finish();
    }

    if (target->getCurrentHp() <= 0) {
        return finish();
    }

    const SkillCell *skill_cell = skill_selector_->getSkill(actor_);
    if (NULL == skill_cell) {
        LOG_ERROR("actor(%ld) get skill failed", actor_->getActorNo());
        return finish(false);
    }
    if (0 == skill_cell->id_) {
        // idle, wait for next attack chance
        timer_id_ = ClientApp::getInstance()->startTimer(1000,
            BRICKRED_BIND_MEM_FUNC(&ActionAttack::onAttackTimer, this), 1);
        return;
    }

    if (actor_->getPos().getDistance(target->getPos()) >
        skill_cell->distance_) {
        // approach the target
        UniquePtr<ActionApproachTarget> action_approach_target(
            new ActionApproachTarget(target_id_, skill_cell->distance_));
        action_approach_target->setActor(actor_);
        action_approach_target->setCompleteCallback(BRICKRED_BIND_MEM_FUNC(
            &ActionAttack::onApproachComplete, this));
        action_approach_target_ = action_approach_target.release();
        action_approach_target_->start();
        return;

    } else {
        // use skill
        gateway::protocol::MessageSkillOperate request;
        request.__set_skill_id_(skill_cell->id_);
        request.__set_type_(gateway::protocol::SkillOperateType::USE);
        request.context_.__set_target_type_(target->getType());
        request.context_.__set_target_id_(
            brickred::string_util::toString(target_id_));
        actor_->sendToGateway(request,
            gateway::protocol::MessageType::MESSAGE_SKILL_OPERATE);

        // wait for next attack chance
        int ms = actor_->getPos().getDistance(target->getPos()) *
                 skill_cell->speed_;
        timer_id_ = ClientApp::getInstance()->startTimer(std::max(ms, 1000),
            BRICKRED_BIND_MEM_FUNC(&ActionAttack::onAttackTimer, this), 1);
    }
}

void ActionAttack::onAttackTimer(int64_t timer_id)
{
    timer_id_ = -1;
    attack();
}

void ActionAttack::onApproachComplete(bool success)
{
    if (action_approach_target_ != NULL) {
        delete action_approach_target_;
        action_approach_target_ = NULL;
    }

    if (success) {
        attack();
    } else {
        return finish(false);
    }
}

} // namespace robot

