#ifndef BRICKRED_UNIQUE_PTR_H
#define BRICKRED_UNIQUE_PTR_H

#include <brickred/class_util.h>

namespace brickred {

template <class T>
class UniquePtr {
public:
    explicit UniquePtr(T *p = 0) : px_(p) {}
    ~UniquePtr() { delete px_; }

    T *operator->() const { return px_; }
    T &operator*() const { return *px_; }
    T *get() const { return px_; }

    void swap(UniquePtr &b)
    {
        T *tmp = b.px_;
        b.px_ = px_;
        px_ = tmp;
    }

    void reset(T *p = 0)
    {
        UniquePtr<T>(p).swap(*this);
    }

    BRICKRED_SAFE_BOOL_TYPE(UniquePtr)
    operator SafeBoolType() const
    {
        return px_ != 0 ? &UniquePtr::SafeBoolTypeNotNull : 0;
    }

    T *release()
    {
        T *p = px_;
        px_ = 0;

        return p;
    }

private:
    BRICKRED_NONCOPYABLE(UniquePtr)

    T *px_;
};

template <class T>
inline void swap(UniquePtr<T> &a, UniquePtr<T> &b)
{
    a.swap(b);
}

} // namespace brickred

#endif
