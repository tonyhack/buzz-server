//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-23 18:31:18.
// File name: pool_template.h
//
// Description:
// Define class PoolTemplate.
//

#ifndef __GLOBAL__POOL__TEMPLATE__H
#define __GLOBAL__POOL__TEMPLATE__H

#include <assert.h>

#include <list>

#include "core/base/noncopyable.h"
#include "global/logging.h"

namespace global {

template <typename T>
class PoolTemplate : public core::Noncopyable {
  typedef std::list<T *> PoolList;
  typedef typename std::list<T *>::iterator PoolListIterator;

 public:
  PoolTemplate();
  ~PoolTemplate();

  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  T *Allocate();
  void Deallocate(T *element);

 private:
  void Extend();
  void Reduce(size_t size);

  size_t initial_number_;
  size_t extend_number_;
  PoolList list_;
};

template <typename T>
PoolTemplate<T>::PoolTemplate() : initial_number_(0), extend_number_(0) {}

template <typename T>
PoolTemplate<T>::~PoolTemplate() {}

template <typename T>
bool PoolTemplate<T>::Initialize(size_t initial_number, size_t extend_number) {
  this->initial_number_ = initial_number;
  this->extend_number_ = extend_number;
  size_t pos = 0;
  for(; pos < initial_number; ++pos) {
    T *element = new (std::nothrow) T();
    if(element == NULL) {
      LogError("%s:%d (%s) Allocate connection failed.", __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
    this->list_.push_back(element);
  }
  return true;
}

template <typename T>
void PoolTemplate<T>::Finalize() {
  PoolListIterator iterator = this->list_.begin();
  for(; iterator != this->list_.end(); ++iterator) {
    delete *iterator;
  }
  this->list_.clear();
}

template <typename T>
T *PoolTemplate<T>::Allocate() {
  if(this->list_.empty()) {
    this->Extend();
  }
  T *element = NULL;
  PoolListIterator iterator = this->list_.begin();
  if(iterator != this->list_.end()) {
    element = *iterator;
    this->list_.erase(iterator);
  }
  return element;
}

template <typename T>
void PoolTemplate<T>::Deallocate(T *element) {
  assert(element);
  this->list_.push_back(element);
  if(this->list_.size() > this->initial_number_) {
    this->Reduce(this->list_.size() - this->initial_number_);
  }
}

template <typename T>
void PoolTemplate<T>::Extend() {
  size_t pos = 0;
  for(; pos < this->extend_number_; ++pos) {
    T *element = new (std::nothrow) T();
    if(element) {
      this->list_.push_back(element);
    }
  }
}

template <typename T>
void PoolTemplate<T>::Reduce(size_t size) {
  size_t pos = 0;
  for(; pos < size; ++pos) {
    PoolListIterator iterator = this->list_.begin();
    if(iterator == this->list_.end()) {
      break;
    }
    delete *iterator;
    this->list_.erase(iterator);
  }
}

}  // namespace global

#endif  // __GLOBAL__POOL__TEMPLATE__H

