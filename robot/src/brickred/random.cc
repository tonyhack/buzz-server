/*
 *  Based on MT19937 Coded by Takuji Nishimura and Makoto Matsumoto.

 *  Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
 *  All rights reserved.

 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:

 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.

 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.

 *    3. The names of its contributors may not be used to endorse or promote
 *       products derived from this software without specific prior written
 *       permission.

 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER
 *  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 *  Any feedback is very welcome.
 *  http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
 *  email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
 */

#include <brickred/random.h>

#include <cmath>

#include <brickred/timestamp.h>

namespace brickred {

class Random::Impl {
public:
    static const int N = 624;
    static const int M = 397;
    static const uint32_t MATRIX_A = 0x9908b0df;
    static const uint32_t UPPER_MASK = 0x80000000;
    static const uint32_t LOWER_MASK = 0x7fffffff;

    Impl();
    Impl(uint32_t s);
    Impl(uint32_t key[], size_t key_length);
    ~Impl();

    void seed(uint32_t s);
    void seed(uint32_t key[], size_t key_length);

    uint32_t nextInt();
    uint32_t nextInt(uint32_t max);
    double nextDouble();

private:
    uint32_t mt_[N];
    int mti_;
};

///////////////////////////////////////////////////////////////////////////////
Random::Impl::Impl()
{
    Timestamp now;
    now.setNow();
    seed(now.getSecond() +
         (now.getSecond() & 0x1 ? -1 : 1) *
         now.getMilliSecond() * 24 * 60 * 60);
}

Random::Impl::Impl(uint32_t s)
{
    seed(s);
}

Random::Impl::Impl(uint32_t key[], size_t key_length)
{
    seed(key, key_length);
}

Random::Impl::~Impl()
{
}

void Random::Impl::seed(uint32_t s)
{
    mt_[0] = s;
    for (mti_ = 1; mti_ < N; ++mti_) {
        mt_[mti_] = (1812433253 *
            (mt_[mti_ - 1] ^ (mt_[mti_- 1] >> 30)) + mti_);
    }
}

void Random::Impl::seed(uint32_t key[], size_t key_length)
{
    seed(19650218);

    int i = 1;
    int j = 0;
    int k = N > (int)key_length ? N : key_length;

    for (; k > 0; k--) {
        // non linear
        mt_[i] = (mt_[i] ^ ((mt_[i - 1] ^ (mt_[i - 1] >> 30)) * 1664525))
                 + key[j] + j;
        ++i;
        ++j;

        if (i >= N) {
            mt_[0] = mt_[N - 1];
            i = 1;
        }
        if (j >= (int)key_length) {
            j = 0;
        }
    }
    for (k = N - 1; k > 0; k--) {
        // non linear
        mt_[i] = (mt_[i] ^ ((mt_[i - 1] ^ (mt_[i - 1] >> 30)) * 1566083941))
                  - i;
        ++i;

        if (i >= N) {
            mt_[0] = mt_[N - 1];
            i = 1;
        }
    }

    // MSB is 1; assuring non-zero initial array
    mt_[0] = 0x80000000;
}

uint32_t Random::Impl::nextInt()
{
    // mag01[x] = x * MATRIX_A  for x=0,1
    static const uint32_t mag01[2] = {0, MATRIX_A};
    uint32_t y;

    // generate N words at one time
    if (mti_ >= N) {
        int i = 0;

        for (; i < N - M; ++i) {
            y = (mt_[i] & UPPER_MASK) | (mt_[i + 1] & LOWER_MASK);
            mt_[i] = mt_[i + M] ^ (y >> 1) ^ mag01[y & 0x1];
        }
        for (; i < N - 1; ++i) {
            y = (mt_[i] & UPPER_MASK) | (mt_[i + 1] & LOWER_MASK);
            mt_[i] = mt_[i + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1];
        }
        y = (mt_[N - 1] & UPPER_MASK) | (mt_[0] & LOWER_MASK);
        mt_[N - 1] = mt_[M - 1] ^ (y >> 1) ^ mag01[y & 0x1];

        mti_ = 0;
    }

    y = mt_[mti_++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680;
    y ^= (y << 15) & 0xefc60000;
    y ^= (y >> 18);

    return y;
}

uint32_t Random::Impl::nextInt(uint32_t max)
{
    return (uint32_t)(nextDouble() * max);
}

double Random::Impl::nextDouble()
{
    uint32_t a = nextInt() >> 5;
    uint32_t b = nextInt() >> 6;

    return (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
}

///////////////////////////////////////////////////////////////////////////////
Random::Random() :
    pimpl_(new Impl())
{
}

Random::Random(uint32_t s) :
    pimpl_(new Impl(s))
{
}

Random::Random(uint32_t key[], size_t key_length) :
    pimpl_(new Impl(key, key_length))
{
}

Random::~Random()
{
}

void Random::seed(uint32_t s)
{
    pimpl_->seed(s);
}

void Random::seed(uint32_t key[], size_t key_length)
{
    pimpl_->seed(key, key_length);
}

uint32_t Random::nextInt()
{
    return pimpl_->nextInt();
}

uint32_t Random::nextInt(uint32_t max)
{
    return pimpl_->nextInt(max);
}

double Random::nextDouble()
{
    return pimpl_->nextDouble();
}

} // namespace brickred
