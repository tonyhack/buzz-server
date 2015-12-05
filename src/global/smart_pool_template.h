//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-27 14:52:35.
// File name: smart_pool_template.h
//
// Description:
// Define class SmartPoolTemplate.
//

#ifndef __GLOBAL__SMART__POOL__TEMPLATE__H
#define __GLOBAL__SMART__POOL__TEMPLATE__H

#include <list>
#include <set>

#include <boost/shared_ptr.hpp>

#include "core/base/noncopyable.h"
#include "global/logging.h"

namespace global {

template <typename T>
class SmartPoolTemplate : public core::Noncopyable {
  typedef boost::shared_ptr<T> SmartPointer;
  typedef std::list<SmartPointer> SmartPointerList;
  typedef typename SmartPointerList::iterator SmartPointerListIterator;
  typedef std::set<SmartPointer> SmartPointerSet;
  typedef typename SmartPointerSet::iterator SmartPointerSetIterator;

 public:
  SmartPoolTemplate();
  ~SmartPoolTemplate();

  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  SmartPointer Allocate();
  void Deallocate(const SmartPointer &element);

 private:
  void Extend();
  void Reduce(size_t size);

  size_t initial_number_;
  size_t extend_number_;

  SmartPointerList frees_;
  SmartPointerSet busys_;
};

template <typename T>
SmartPoolTemplate<T>::SmartPoolTemplate() : initial_number_(0), extend_number_(0) {}

template <typename T>
SmartPoolTemplate<T>::~SmartPoolTemplate() {}

template <typename T>
bool SmartPoolTemplate<T>::Initialize(size_t initial_number, size_t extend_number) {
  this->initial_number_ = initial_number;
  this->extend_number_ = extend_number;
  size_t pos = 0;
  for(; pos < initial_number; ++pos) {
    SmartPointer element(new (std::nothrow) T());
    if(element.get() == NULL) {
      LogError("%s:%d (%s) Allocate element failed.", __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
    this->frees_.push_back(element);
  }
  return true;
}

template <typename T>
void SmartPoolTemplate<T>::Finalize() {
  this->frees_.clear();
  this->busys_.clear();
}

template <typename T>
boost::shared_ptr<T> SmartPoolTemplate<T>::Allocate() {
  if(this->frees_.empty()) {
    this->Extend();
  }
  SmartPointer element;
  SmartPointerListIterator iterator = this->frees_.begin();
  if(iterator != this->frees_.end()) {
    element = *iterator;
    this->frees_.erase(iterator);
    this->busys_.insert(element);
  }
  return element;
}

template <typename T>
void SmartPoolTemplate<T>::Deallocate(const SmartPointer &element) {
  SmartPointerSetIterator iterator = this->busys_.find(element);
  if(iterator != this->busys_.end()) {
    this->busys_.erase(iterator);
    this->frees_.push_back(element);
  } else {
    LogError("%s:%d (%s) Deallocate a non-busy element",
        __FILE__, __LINE__, __FUNCTION__);
  }
  if(this->frees_.size() > this->initial_number_) {
    this->Reduce(this->frees_.size() - this->initial_number_);
  }
}

template <typename T>
void SmartPoolTemplate<T>::Extend() {
  size_t pos = 0;
  for(; pos < this->extend_number_; ++pos) {
    SmartPointer element(new (std::nothrow) T());
    if(element.get() == NULL) {
      LogError("%s:%d (%s) Allocate element failed.", __FILE__, __LINE__, __FUNCTION__);
      return ;
    }
    this->frees_.push_back(element);
  }
}

template <typename T>
void SmartPoolTemplate<T>::Reduce(size_t size) {
  size_t pos = 0;
  for(; pos < size; ++pos) {
    SmartPointerListIterator iterator = this->frees_.begin();
    if(iterator == this->frees_.end()) {
      break;
    }
    this->frees_.erase(iterator);
  }
}

}  // namespace global

#endif  // __GLOBAL__SMART__POOL__TEMPLATE__H

