#ifndef BRICKRED_OBJECT_POOL_H
#define BRICKRED_OBJECT_POOL_H

#include <cstddef>
#include <vector>

namespace brickred {

template <class T>
class ObjectPool {
public:
    ObjectPool() {}
    ~ObjectPool()
    {
        for (size_t i = 0; i < reused_object_list_.size(); ++i) {
            delete reused_object_list_[i];
        }
    }

    T *getObject()
    {
        if (!reused_object_list_.empty()) {
            T *obj = reused_object_list_.back();
            reused_object_list_.pop_back();
            return obj;
        }
        return new T();
    }

    void returnObject(T *obj)
    {
        reused_object_list_.push_back(obj);
    }

private:
    std::vector<T *> reused_object_list_;
};

} // namespace brickred

#endif
