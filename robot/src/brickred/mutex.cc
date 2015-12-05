#include <brickred/mutex.h>

#include <pthread.h>

#include <brickred/exception.h>

namespace brickred {

class Mutex::Impl {
public:
    Impl();
    ~Impl();

    void lock();
    void unlock();
    void *nativeHandle();

private:
    pthread_mutex_t mutex_;
};

///////////////////////////////////////////////////////////////////////////////
Mutex::Impl::Impl()
{
    if (::pthread_mutex_init(&mutex_, NULL) != 0) {
        throw SystemErrorException(
            "Mutex constructor failed in pthread_mutex_init"
        );
    }
}

Mutex::Impl::~Impl()
{
    ::pthread_mutex_destroy(&mutex_);
}

void Mutex::Impl::lock()
{
    if (::pthread_mutex_lock(&mutex_) != 0) {
        throw SystemErrorException(
            "Mutex lock failed in pthread_mutex_lock"
        );
    }
}

void Mutex::Impl::unlock()
{
    if (::pthread_mutex_unlock(&mutex_) != 0) {
        throw SystemErrorException(
            "Mutex unlock failed in pthread_mutex_unlock"
        );
    }
}

void *Mutex::Impl::nativeHandle()
{
    return static_cast<void *>(&mutex_);
}

///////////////////////////////////////////////////////////////////////////////
Mutex::Mutex() :
    pimpl_(new Impl())
{
}

Mutex::~Mutex()
{
}

void Mutex::lock()
{
    pimpl_->lock();
}

void Mutex::unlock()
{
    pimpl_->unlock();
}

void *Mutex::nativeHandle()
{
    return pimpl_->nativeHandle();
}

} // namespace brickred
