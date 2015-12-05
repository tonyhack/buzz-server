#ifndef BRICKRED_CONDITION_VARIABLE_H
#define BRICKRED_CONDITION_VARIABLE_H

#include <brickred/class_util.h>
#include <brickred/unique_ptr.h>

namespace brickred {

class Mutex;

class ConditionVariable {
public:
    ConditionVariable();
    ~ConditionVariable();

    void wait(Mutex &m);
    void notifyOne();
    void notifyAll();
    void *nativeHandle();

private:
    BRICKRED_NONCOPYABLE(ConditionVariable)

    class Impl;
    UniquePtr<Impl> pimpl_;
};

} // namespace brickred

#endif
