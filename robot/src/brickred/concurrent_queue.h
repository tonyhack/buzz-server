#ifndef BRICKRED_CONCURRENT_QUEUE_H
#define BRICKRED_CONCURRENT_QUEUE_H

#include <cstddef>
#include <queue>

#include <brickred/condition_variable.h>
#include <brickred/class_util.h>
#include <brickred/mutex.h>

namespace brickred {

template <class T>
class ConcurrentQueue {
public:
    ConcurrentQueue(size_t max_size = 0) :
        max_size_(max_size), is_bounded_(max_size != 0)
    {}

    ~ConcurrentQueue() {}

    size_t size()
    {
        LockGuard lock(data_mutex_);
        return queue_.size();
    }

    size_t maxSize() const
    {
        return max_size_;
    }

    bool isBounded() const
    {
        return is_bounded_;
    }

    bool empty()
    {
        LockGuard lock(data_mutex_);
        return queue_.empty();
    }

    bool full()
    {
        LockGuard lock(data_mutex_);
        return fullNoLock();
    }

    void push(const T &item)
    {
        LockGuard lock(data_mutex_);
        while (fullNoLock()) {
            not_full_cond_.wait(data_mutex_);
        }
        queue_.push(item);
        not_empty_cond_.notifyOne();
    }

    bool pushIfNotFull(const T &item)
    {
        LockGuard lock(data_mutex_);
        if (fullNoLock()) {
            return false;
        }
        queue_.push(item);
        not_empty_cond_.notifyOne();
        return true;
    }

    void pop(T &item)
    {
        LockGuard lock(data_mutex_);
        while (queue_.empty()) {
            not_empty_cond_.wait(data_mutex_);
        }
        T front = queue_.front();
        queue_.pop();
        item = front;
        not_full_cond_.notifyOne();
    }

    bool popIfNotEmpty(T &item)
    {
        LockGuard lock(data_mutex_);
        if (queue_.empty()) {
            return false;
        }
        T front = queue_.front();
        queue_.pop();
        item = front;
        not_full_cond_.notifyOne();
        return true;
    }

private:
    bool fullNoLock() const
    {
        if (!is_bounded_) {
            return false;
        } else {
            return queue_.size() >= max_size_;
        }
    }

private:
    BRICKRED_NONCOPYABLE(ConcurrentQueue)

    std::queue<T> queue_;
    size_t max_size_;
    bool is_bounded_;
    Mutex data_mutex_;
    ConditionVariable not_full_cond_;
    ConditionVariable not_empty_cond_;
};

} // namespace brickred

#endif
