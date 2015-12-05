#include <brickred/condition_variable.h>

#include <pthread.h>

#include <brickred/exception.h>
#include <brickred/mutex.h>

namespace brickred {

class ConditionVariable::Impl {
public:
    Impl();
    ~Impl();

    void wait(Mutex &m);
    void notifyOne();
    void notifyAll();
    void *nativeHandle();

private:
    pthread_cond_t cond_;
};

///////////////////////////////////////////////////////////////////////////////
ConditionVariable::Impl::Impl()
{
    if (::pthread_cond_init(&cond_, NULL) != 0) {
        throw SystemErrorException(
            "ConditionVariable constructor failed in pthread_cond_init"
        );
    }
}

ConditionVariable::Impl::~Impl()
{
    ::pthread_cond_destroy(&cond_);
}

void ConditionVariable::Impl::wait(Mutex &m)
{
    if (::pthread_cond_wait(&cond_,
                            (pthread_mutex_t *)m.nativeHandle()) != 0) {
        throw SystemErrorException(
            "ConditionVariable wait failed in pthread_cond_wait"
        );
    }
}

void ConditionVariable::Impl::notifyOne()
{
    ::pthread_cond_signal(&cond_);
}

void ConditionVariable::Impl::notifyAll()
{
    ::pthread_cond_broadcast(&cond_);
}

void *ConditionVariable::Impl::nativeHandle()
{
    return static_cast<void *>(&cond_);
}

///////////////////////////////////////////////////////////////////////////////
ConditionVariable::ConditionVariable() :
    pimpl_(new Impl())
{
}

ConditionVariable::~ConditionVariable()
{
}

void ConditionVariable::wait(Mutex &m)
{
    pimpl_->wait(m);
}

void ConditionVariable::notifyOne()
{
    pimpl_->notifyOne();
}

void ConditionVariable::notifyAll()
{
    pimpl_->notifyAll();
}

void *ConditionVariable::nativeHandle()
{
    return pimpl_->nativeHandle();
}

} // namespace brickred
