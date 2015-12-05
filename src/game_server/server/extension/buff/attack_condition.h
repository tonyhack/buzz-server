//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-21 17:34:46.
// File name: attack_condition.h
//
// Description:
// Define attack condition.
//

#ifndef __GAME__SERVER__BUFF__ATTACK__CONDITION__H
#define __GAME__SERVER__BUFF__ATTACK__CONDITION__H

#include "game_server/server/extension/buff/condition.h"

namespace game {

namespace server {

namespace buff {

class AttackHitCondition : public Condition {
 public:
  AttackHitCondition();
  virtual ~AttackHitCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::ATTACK_HIT;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

  virtual void OnExecute() {
		this->passed_ = false;
	}

 private:
  bool OnEventAttackHit(const void *data, size_t size);
};

class AttackCritCondition : public Condition {
 public:
  AttackCritCondition();
  virtual ~AttackCritCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::ATTACK_CRIT;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

  virtual void OnExecute() {
		this->passed_ = false;
	}

 private:
  bool OnEventAttackHit(const void *data, size_t size);
};

class AttackDodgeCondition : public Condition {
 public:
  AttackDodgeCondition();
  virtual ~AttackDodgeCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::ATTACK_DODGE;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

  virtual void OnExecute() {
		this->passed_ = false;
	}

 private:
  bool OnEventAttackHit(const void *data, size_t size);
};

class AttackIgnoreDefenceCondition : public Condition {
 public:
  AttackIgnoreDefenceCondition();
  virtual ~AttackIgnoreDefenceCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::ATTACK_IGNORE_DEFENCE;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

  virtual void OnExecute() {
		this->passed_ = false;
	}

 private:
  bool OnEventAttackHit(const void *data, size_t size);
};

class AttackedHitCondition : public Condition {
 public:
  AttackedHitCondition();
  virtual ~AttackedHitCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::ATTACKED_HIT;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

  virtual void OnExecute() {
		this->passed_ = false;
	}

 private:
  bool OnEventAttackedHit(const void *data, size_t size);
};

class AttackedCritCondition : public Condition {
 public:
  AttackedCritCondition();
  virtual ~AttackedCritCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::ATTACKED_CRIT;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

  virtual void OnExecute() {
		this->passed_ = false;
	}

 private:
  bool OnEventAttackedHit(const void *data, size_t size);
};

class AttackedDodgeCondition : public Condition {
 public:
  AttackedDodgeCondition();
  virtual ~AttackedDodgeCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::ATTACKED_DODGE;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

  virtual void OnExecute() {
		this->passed_ = false;
	}

 private:
  bool OnEventAttackedHit(const void *data, size_t size);
};

class AttackedIgnoreDefenceCondition : public Condition {
 public:
  AttackedIgnoreDefenceCondition();
  virtual ~AttackedIgnoreDefenceCondition();

  virtual bool Initialize(core::int32 id, BuffRole *role);
  virtual void Finalize();

  virtual entity::BuffConditionType::type GetType() const {
    return entity::BuffConditionType::ATTACKED_IGNORE_DEFENCE;
  }

  virtual bool OnEvent(event::EventType::type type,
      const void *data, size_t size);

  virtual void OnExecute() {
		this->passed_ = false;
	}

 private:
  bool OnEventAttackedHit(const void *data, size_t size);
};

}  // namespace buff

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BUFF__ATTACK__CONDITION__H

