#ifndef BRICKRED_RANDOM_H
#define BRICKRED_RANDOM_H

#include <stdint.h>
#include <cstddef>

#include <brickred/class_util.h>
#include <brickred/unique_ptr.h>

namespace brickred {

class Random {
public:
    Random();
    Random(uint32_t s);
    Random(uint32_t key[], size_t key_length);
    ~Random();

    void seed(uint32_t s);
    void seed(uint32_t key[], size_t key_length);

    // uniformly distributed on range [0, 0xffffffff]
    uint32_t nextInt();
    // uniformly distributed on range [0, max)
    uint32_t nextInt(uint32_t max);
    // uniformly distributed on range [0, 1)
    double nextDouble();

private:
    BRICKRED_NONCOPYABLE(Random)

    class Impl;
    UniquePtr<Impl> pimpl_;
};

} // namespace brickred

#endif
