#ifndef BRICKRED_FUNCTION_H
#define BRICKRED_FUNCTION_H

#include <brickred/class_util.h>

#define BRICKRED_BIND_FREE_FUNC(_free_func_ptr) \
    brickred::_getFunctionFactory(_free_func_ptr).bind<_free_func_ptr>()

#define BRICKRED_BIND_MEM_FUNC(_mem_func_ptr, _instance_ptr) \
    brickred::_getFunctionFactory(_mem_func_ptr).bind<_mem_func_ptr>(_instance_ptr)

#define BRICKRED_BIND_TEMPLATE_MEM_FUNC(_mem_func_ptr, _instance_ptr) \
    brickred::_getFunctionFactory(_mem_func_ptr).template bind<_mem_func_ptr>(_instance_ptr)

namespace brickred {

template <typename FunctionSignature>
class Function;

class NullFunction {};

// 0 parameter
template <typename R>
class Function<R ()> {
public:
    Function() :
        func_(0), obj_(0) {}
    Function(NullFunction) :
        func_(0), obj_(0) {}
    Function(const Function &rhs) :
        func_(rhs.func_), obj_(rhs.obj_) {}

    Function &operator=(NullFunction)
        { func_ = 0; obj_ = 0; return *this; }
    Function &operator=(const Function &rhs)
        { func_ = rhs.func_; obj_ = rhs.obj_; return *this; }

    inline R operator()() const
    {
        return (*func_)(obj_);
    }

    BRICKRED_SAFE_BOOL_TYPE(Function)
    operator SafeBoolType() const
    {
        return func_ != 0 ? &Function::SafeBoolTypeNotNull : 0;
    }

private:
    typedef R (*FunctionType)(const void *);
    Function(FunctionType func, const void *obj) :
        func_(func), obj_(obj) {}

    template <typename FR>
    friend class FreeFunctionFactory0;
    template <typename FR, class FT>
    friend class MemberFunctionFactory0;
    template <typename FR, class FT>
    friend class ConstMemberFunctionFactory0;

    FunctionType func_;
    const void *obj_;
};

template <typename R>
void operator==(const Function<R ()> &, const Function<R ()> &);
template <typename R>
void operator!=(const Function<R ()> &, const Function<R ()> &);

template <typename R>
class FreeFunctionFactory0 {
private:
    template <R (*func)()>
    static R wrapper(const void *)
    {
        return (*func)();
    }

public:
    template <R (*func)()>
    inline static Function<R ()> bind()
    {
        return Function<R ()>(&FreeFunctionFactory0::template wrapper<func>, 0);
    }
};

template <typename R>
inline FreeFunctionFactory0<R> _getFunctionFactory(R (*)())
{
    return FreeFunctionFactory0<R>();
}

template <typename R, class T>
class MemberFunctionFactory0 {
private:
    template <R (T::*func)()>
    static R wrapper(const void *o)
    {
        T *obj = const_cast<T *>(static_cast<const T *>(o));
        return (obj->*func)();
    }

public:
    template <R (T::*func)()>
    inline static Function<R ()> bind(T *o)
    {
        return Function<R ()>(&MemberFunctionFactory0::template wrapper<func>,
                              static_cast<const void *>(o));
    }
};

template <typename R, class T>
inline MemberFunctionFactory0<R, T> _getFunctionFactory(R (T::*)())
{
    return MemberFunctionFactory0<R, T>();
}

template <typename R, class T>
class ConstMemberFunctionFactory0 {
private:
    template <R (T::*func)() const>
    static R wrapper(const void *o)
    {
        const T *obj = static_cast<const T *>(o);
        return (obj->*func)();
    }

public:
    template <R (T::*func)() const>
    inline static Function<R ()> bind(const T *o)
    {
        return Function<R ()>(&ConstMemberFunctionFactory0::template wrapper<func>,
                              static_cast<const void *>(o));
    }
};

template <typename R, class T>
inline ConstMemberFunctionFactory0<R, T> _getFunctionFactory(R (T::*)() const)
{
    return ConstMemberFunctionFactory0<R, T>();
}

// 1 parameter
template <typename R, typename P1>
class Function<R (P1)> {
public:
    Function() :
        func_(0), obj_(0) {}
    Function(NullFunction) :
        func_(0), obj_(0) {}
    Function(const Function &rhs) :
        func_(rhs.func_), obj_(rhs.obj_) {}

    Function &operator=(NullFunction)
        { func_ = 0; obj_ = 0; return *this; }
    Function &operator=(const Function &rhs)
        { func_ = rhs.func_; obj_ = rhs.obj_; return *this; }

    inline R operator()(P1 a1) const
    {
        return (*func_)(obj_, a1);
    }

    BRICKRED_SAFE_BOOL_TYPE(Function)
    operator SafeBoolType() const
    {
        return func_ != 0 ? &Function::SafeBoolTypeNotNull : 0;
    }

private:
    typedef R (*FunctionType)(const void *, P1);
    Function(FunctionType func, const void *obj) :
        func_(func), obj_(obj) {}

    template <typename FR, typename FP1>
    friend class FreeFunctionFactory1;
    template <typename FR, class FT, typename FP1>
    friend class MemberFunctionFactory1;
    template <typename FR, class FT, typename FP1>
    friend class ConstMemberFunctionFactory1;

    FunctionType func_;
    const void *obj_;
};

template <typename R, typename P1>
void operator==(const Function<R (P1)> &, const Function<R (P1)> &);
template <typename R, typename P1>
void operator!=(const Function<R (P1)> &, const Function<R (P1)> &);

template <typename R, typename P1>
class FreeFunctionFactory1 {
private:
    template <R (*func)(P1)>
    static R wrapper(const void *, P1 a1)
    {
        return (*func)(a1);
    }

public:
    template <R (*func)(P1)>
    inline static Function<R (P1)> bind()
    {
        return Function<R (P1)>(&FreeFunctionFactory1::template wrapper<func>, 0);
    }
};

template <typename R, typename P1>
inline FreeFunctionFactory1<R, P1> _getFunctionFactory(R (*)(P1))
{
    return FreeFunctionFactory1<R, P1>();
}

template <typename R, class T, typename P1>
class MemberFunctionFactory1 {
private:
    template <R (T::*func)(P1)>
    static R wrapper(const void *o, P1 a1)
    {
        T *obj = const_cast<T *>(static_cast<const T *>(o));
        return (obj->*func)(a1);
    }

public:
    template <R (T::*func)(P1)>
    inline static Function<R (P1)> bind(T *o)
    {
        return Function<R (P1)>(&MemberFunctionFactory1::template wrapper<func>,
                                static_cast<const void *>(o));
    }
};

template <typename R, class T, typename P1>
inline MemberFunctionFactory1<R, T, P1> _getFunctionFactory(R (T::*)(P1))
{
    return MemberFunctionFactory1<R, T, P1>();
}

template <typename R, class T, typename P1>
class ConstMemberFunctionFactory1 {
private:
    template <R (T::*func)(P1) const>
    static R wrapper(const void *o, P1 a1)
    {
        const T *obj = static_cast<const T *>(o);
        return (obj->*func)(a1);
    }

public:
    template <R (T::*func)(P1) const>
    inline static Function<R (P1)> bind(const T *o)
    {
        return Function<R (P1)>(&ConstMemberFunctionFactory1::template wrapper<func>,
                              static_cast<const void *>(o));
    }
};

template <typename R, class T, typename P1>
inline ConstMemberFunctionFactory1<R, T, P1> _getFunctionFactory(R (T::*)(P1) const)
{
    return ConstMemberFunctionFactory1<R, T, P1>();
}

// 2 parameter
template <typename R, typename P1, typename P2>
class Function<R (P1, P2)> {
public:
    Function() :
        func_(0), obj_(0) {}
    Function(NullFunction) :
        func_(0), obj_(0) {}
    Function(const Function &rhs) :
        func_(rhs.func_), obj_(rhs.obj_) {}

    Function &operator=(NullFunction)
        { func_ = 0; obj_ = 0; return *this; }
    Function &operator=(const Function &rhs)
        { func_ = rhs.func_; obj_ = rhs.obj_; return *this; }

    inline R operator()(P1 a1, P2 a2) const
    {
        return (*func_)(obj_, a1, a2);
    }

    BRICKRED_SAFE_BOOL_TYPE(Function)
    operator SafeBoolType() const
    {
        return func_ != 0 ? &Function::SafeBoolTypeNotNull : 0;
    }

private:
    typedef R (*FunctionType)(const void *, P1, P2);
    Function(FunctionType func, const void *obj) :
        func_(func), obj_(obj) {}

    template <typename FR, typename FP1, typename FP2>
    friend class FreeFunctionFactory2;
    template <typename FR, class FT, typename FP1, typename FP2>
    friend class MemberFunctionFactory2;
    template <typename FR, class FT, typename FP1, typename FP2>
    friend class ConstMemberFunctionFactory2;

    FunctionType func_;
    const void *obj_;
};

template <typename R, typename P1, typename P2>
void operator==(const Function<R (P1, P2)> &, const Function<R (P1, P2)> &);
template <typename R, typename P1, typename P2>
void operator!=(const Function<R (P1, P2)> &, const Function<R (P1, P2)> &);

template <typename R, typename P1, typename P2>
class FreeFunctionFactory2 {
private:
    template <R (*func)(P1, P2)>
    static R wrapper(const void *, P1 a1, P2 a2)
    {
        return (*func)(a1, a2);
    }

public:
    template <R (*func)(P1, P2)>
    inline static Function<R (P1, P2)> bind()
    {
        return Function<R (P1, P2)>(&FreeFunctionFactory2::template wrapper<func>, 0);
    }
};

template <typename R, typename P1, typename P2>
inline FreeFunctionFactory2<R, P1, P2> _getFunctionFactory(R (*)(P1, P2))
{
    return FreeFunctionFactory2<R, P1, P2>();
}

template <typename R, class T, typename P1, typename P2>
class MemberFunctionFactory2 {
private:
    template <R (T::*func)(P1, P2)>
    static R wrapper(const void *o, P1 a1, P2 a2)
    {
        T *obj = const_cast<T *>(static_cast<const T *>(o));
        return (obj->*func)(a1, a2);
    }

public:
    template <R (T::*func)(P1, P2)>
    inline static Function<R (P1, P2)> bind(T *o)
    {
        return Function<R (P1, P2)>(&MemberFunctionFactory2::template wrapper<func>,
                                static_cast<const void *>(o));
    }
};

template <typename R, class T, typename P1, typename P2>
inline MemberFunctionFactory2<R, T, P1, P2> _getFunctionFactory(R (T::*)(P1, P2))
{
    return MemberFunctionFactory2<R, T, P1, P2>();
}

template <typename R, class T, typename P1, typename P2>
class ConstMemberFunctionFactory2 {
private:
    template <R (T::*func)(P1, P2) const>
    static R wrapper(const void *o, P1 a1, P2 a2)
    {
        const T *obj = static_cast<const T *>(o);
        return (obj->*func)(a1, a2);
    }

public:
    template <R (T::*func)(P1, P2) const>
    inline static Function<R (P1, P2)> bind(const T *o)
    {
        return Function<R (P1, P2)>(&ConstMemberFunctionFactory2::template wrapper<func>,
                              static_cast<const void *>(o));
    }
};

template <typename R, class T, typename P1, typename P2>
inline ConstMemberFunctionFactory2<R, T, P1, P2> _getFunctionFactory(R (T::*)(P1, P2) const)
{
    return ConstMemberFunctionFactory2<R, T, P1, P2>();
}

// 3 parameter
template <typename R, typename P1, typename P2, typename P3>
class Function<R (P1, P2, P3)> {
public:
    Function() :
        func_(0), obj_(0) {}
    Function(NullFunction) :
        func_(0), obj_(0) {}
    Function(const Function &rhs) :
        func_(rhs.func_), obj_(rhs.obj_) {}

    Function &operator=(NullFunction)
        { func_ = 0; obj_ = 0; return *this; }
    Function &operator=(const Function &rhs)
        { func_ = rhs.func_; obj_ = rhs.obj_; return *this; }

    inline R operator()(P1 a1, P2 a2, P3 a3) const
    {
        return (*func_)(obj_, a1, a2, a3);
    }

    BRICKRED_SAFE_BOOL_TYPE(Function)
    operator SafeBoolType() const
    {
        return func_ != 0 ? &Function::SafeBoolTypeNotNull : 0;
    }

private:
    typedef R (*FunctionType)(const void *, P1, P2, P3);
    Function(FunctionType func, const void *obj) :
        func_(func), obj_(obj) {}

    template <typename FR, typename FP1, typename FP2, typename FP3>
    friend class FreeFunctionFactory3;
    template <typename FR, class FT, typename FP1, typename FP2, typename FP3>
    friend class MemberFunctionFactory3;
    template <typename FR, class FT, typename FP1, typename FP2, typename FP3>
    friend class ConstMemberFunctionFactory3;

    FunctionType func_;
    const void *obj_;
};

template <typename R, typename P1, typename P2, typename P3>
void operator==(const Function<R (P1, P2, P3)> &, const Function<R (P1, P2, P3)> &);
template <typename R, typename P1, typename P2, typename P3>
void operator!=(const Function<R (P1, P2, P3)> &, const Function<R (P1, P2, P3)> &);

template <typename R, typename P1, typename P2, typename P3>
class FreeFunctionFactory3 {
private:
    template <R (*func)(P1, P2, P3)>
    static R wrapper(const void *, P1 a1, P2 a2, P3 a3)
    {
        return (*func)(a1, a2, a3);
    }

public:
    template <R (*func)(P1, P2, P3)>
    inline static Function<R (P1, P2, P3)> bind()
    {
        return Function<R (P1, P2, P3)>(&FreeFunctionFactory3::template wrapper<func>, 0);
    }
};

template <typename R, typename P1, typename P2, typename P3>
inline FreeFunctionFactory3<R, P1, P2, P3> _getFunctionFactory(R (*)(P1, P2, P3))
{
    return FreeFunctionFactory3<R, P1, P2, P3>();
}

template <typename R, class T, typename P1, typename P2, typename P3>
class MemberFunctionFactory3 {
private:
    template <R (T::*func)(P1, P2, P3)>
    static R wrapper(const void *o, P1 a1, P2 a2, P3 a3)
    {
        T *obj = const_cast<T *>(static_cast<const T *>(o));
        return (obj->*func)(a1, a2, a3);
    }

public:
    template <R (T::*func)(P1, P2, P3)>
    inline static Function<R (P1, P2, P3)> bind(T *o)
    {
        return Function<R (P1, P2, P3)>(&MemberFunctionFactory3::template wrapper<func>,
                                static_cast<const void *>(o));
    }
};

template <typename R, class T, typename P1, typename P2, typename P3>
inline MemberFunctionFactory3<R, T, P1, P2, P3> _getFunctionFactory(R (T::*)(P1, P2, P3))
{
    return MemberFunctionFactory3<R, T, P1, P2, P3>();
}

template <typename R, class T, typename P1, typename P2, typename P3>
class ConstMemberFunctionFactory3 {
private:
    template <R (T::*func)(P1, P2, P3) const>
    static R wrapper(const void *o, P1 a1, P2 a2, P3 a3)
    {
        const T *obj = static_cast<const T *>(o);
        return (obj->*func)(a1, a2, a3);
    }

public:
    template <R (T::*func)(P1, P2, P3) const>
    inline static Function<R (P1, P2, P3)> bind(const T *o)
    {
        return Function<R (P1, P2, P3)>(&ConstMemberFunctionFactory3::template wrapper<func>,
                              static_cast<const void *>(o));
    }
};

template <typename R, class T, typename P1, typename P2, typename P3>
inline ConstMemberFunctionFactory3<R, T, P1, P2, P3> _getFunctionFactory(R (T::*)(P1, P2, P3) const)
{
    return ConstMemberFunctionFactory3<R, T, P1, P2, P3>();
}

// 4 parameter
template <typename R, typename P1, typename P2, typename P3, typename P4>
class Function<R (P1, P2, P3, P4)> {
public:
    Function() :
        func_(0), obj_(0) {}
    Function(NullFunction) :
        func_(0), obj_(0) {}
    Function(const Function &rhs) :
        func_(rhs.func_), obj_(rhs.obj_) {}

    Function &operator=(NullFunction)
        { func_ = 0; obj_ = 0; return *this; }
    Function &operator=(const Function &rhs)
        { func_ = rhs.func_; obj_ = rhs.obj_; return *this; }

    inline R operator()(P1 a1, P2 a2, P3 a3, P4 a4) const
    {
        return (*func_)(obj_, a1, a2, a3, a4);
    }

    BRICKRED_SAFE_BOOL_TYPE(Function)
    operator SafeBoolType() const
    {
        return func_ != 0 ? &Function::SafeBoolTypeNotNull : 0;
    }

private:
    typedef R (*FunctionType)(const void *, P1, P2, P3, P4);
    Function(FunctionType func, const void *obj) :
        func_(func), obj_(obj) {}

    template <typename FR, typename FP1, typename FP2, typename FP3, typename FP4>
    friend class FreeFunctionFactory4;
    template <typename FR, class FT, typename FP1, typename FP2, typename FP3, typename FP4>
    friend class MemberFunctionFactory4;
    template <typename FR, class FT, typename FP1, typename FP2, typename FP3, typename FP4>
    friend class ConstMemberFunctionFactory4;

    FunctionType func_;
    const void *obj_;
};

template <typename R, typename P1, typename P2, typename P3, typename P4>
void operator==(const Function<R (P1, P2, P3, P4)> &, const Function<R (P1, P2, P3, P4)> &);
template <typename R, typename P1, typename P2, typename P3, typename P4>
void operator!=(const Function<R (P1, P2, P3, P4)> &, const Function<R (P1, P2, P3, P4)> &);

template <typename R, typename P1, typename P2, typename P3, typename P4>
class FreeFunctionFactory4 {
private:
    template <R (*func)(P1, P2, P3, P4)>
    static R wrapper(const void *, P1 a1, P2 a2, P3 a3, P4 a4)
    {
        return (*func)(a1, a2, a3, a4);
    }

public:
    template <R (*func)(P1, P2, P3, P4)>
    inline static Function<R (P1, P2, P3, P4)> bind()
    {
        return Function<R (P1, P2, P3, P4)>(&FreeFunctionFactory4::template wrapper<func>, 0);
    }
};

template <typename R, typename P1, typename P2, typename P3, typename P4>
inline FreeFunctionFactory4<R, P1, P2, P3, P4> _getFunctionFactory(R (*)(P1, P2, P3, P4))
{
    return FreeFunctionFactory4<R, P1, P2, P3, P4>();
}

template <typename R, class T, typename P1, typename P2, typename P3, typename P4>
class MemberFunctionFactory4 {
private:
    template <R (T::*func)(P1, P2, P3, P4)>
    static R wrapper(const void *o, P1 a1, P2 a2, P3 a3, P4 a4)
    {
        T *obj = const_cast<T *>(static_cast<const T *>(o));
        return (obj->*func)(a1, a2, a3, a4);
    }

public:
    template <R (T::*func)(P1, P2, P3, P4)>
    inline static Function<R (P1, P2, P3, P4)> bind(T *o)
    {
        return Function<R (P1, P2, P3, P4)>(&MemberFunctionFactory4::template wrapper<func>,
                                static_cast<const void *>(o));
    }
};

template <typename R, class T, typename P1, typename P2, typename P3, typename P4>
inline MemberFunctionFactory4<R, T, P1, P2, P3, P4> _getFunctionFactory(R (T::*)(P1, P2, P3, P4))
{
    return MemberFunctionFactory4<R, T, P1, P2, P3, P4>();
}

template <typename R, class T, typename P1, typename P2, typename P3, typename P4>
class ConstMemberFunctionFactory4 {
private:
    template <R (T::*func)(P1, P2, P3, P4) const>
    static R wrapper(const void *o, P1 a1, P2 a2, P3 a3, P4 a4)
    {
        const T *obj = static_cast<const T *>(o);
        return (obj->*func)(a1, a2, a3, a4);
    }

public:
    template <R (T::*func)(P1, P2, P3, P4) const>
    inline static Function<R (P1, P2, P3, P4)> bind(const T *o)
    {
        return Function<R (P1, P2, P3, P4)>(&ConstMemberFunctionFactory4::template wrapper<func>,
                              static_cast<const void *>(o));
    }
};

template <typename R, class T, typename P1, typename P2, typename P3, typename P4>
inline ConstMemberFunctionFactory4<R, T, P1, P2, P3, P4> _getFunctionFactory(R (T::*)(P1, P2, P3, P4) const)
{
    return ConstMemberFunctionFactory4<R, T, P1, P2, P3, P4>();
}

// 5 parameter
template <typename R, typename P1, typename P2, typename P3, typename P4, typename P5>
class Function<R (P1, P2, P3, P4, P5)> {
public:
    Function() :
        func_(0), obj_(0) {}
    Function(NullFunction) :
        func_(0), obj_(0) {}
    Function(const Function &rhs) :
        func_(rhs.func_), obj_(rhs.obj_) {}

    Function &operator=(NullFunction)
        { func_ = 0; obj_ = 0; return *this; }
    Function &operator=(const Function &rhs)
        { func_ = rhs.func_; obj_ = rhs.obj_; return *this; }

    inline R operator()(P1 a1, P2 a2, P3 a3, P4 a4, P5 a5) const
    {
        return (*func_)(obj_, a1, a2, a3, a4, a5);
    }

    BRICKRED_SAFE_BOOL_TYPE(Function)
    operator SafeBoolType() const
    {
        return func_ != 0 ? &Function::SafeBoolTypeNotNull : 0;
    }

private:
    typedef R (*FunctionType)(const void *, P1, P2, P3, P4, P5);
    Function(FunctionType func, const void *obj) :
        func_(func), obj_(obj) {}

    template <typename FR, typename FP1, typename FP2, typename FP3, typename FP4, typename FP5>
    friend class FreeFunctionFactory5;
    template <typename FR, class FT, typename FP1, typename FP2, typename FP3, typename FP4, typename FP5>
    friend class MemberFunctionFactory5;
    template <typename FR, class FT, typename FP1, typename FP2, typename FP3, typename FP4, typename FP5>
    friend class ConstMemberFunctionFactory5;

    FunctionType func_;
    const void *obj_;
};

template <typename R, typename P1, typename P2, typename P3, typename P4, typename P5>
void operator==(const Function<R (P1, P2, P3, P4, P5)> &, const Function<R (P1, P2, P3, P4, P5)> &);
template <typename R, typename P1, typename P2, typename P3, typename P4, typename P5>
void operator!=(const Function<R (P1, P2, P3, P4, P5)> &, const Function<R (P1, P2, P3, P4, P5)> &);

template <typename R, typename P1, typename P2, typename P3, typename P4, typename P5>
class FreeFunctionFactory5 {
private:
    template <R (*func)(P1, P2, P3, P4, P5)>
    static R wrapper(const void *, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5)
    {
        return (*func)(a1, a2, a3, a4, a5);
    }

public:
    template <R (*func)(P1, P2, P3, P4, P5)>
    inline static Function<R (P1, P2, P3, P4, P5)> bind()
    {
        return Function<R (P1, P2, P3, P4, P5)>(&FreeFunctionFactory5::template wrapper<func>, 0);
    }
};

template <typename R, typename P1, typename P2, typename P3, typename P4, typename P5>
inline FreeFunctionFactory5<R, P1, P2, P3, P4, P5> _getFunctionFactory(R (*)(P1, P2, P3, P4, P5))
{
    return FreeFunctionFactory5<R, P1, P2, P3, P4, P5>();
}

template <typename R, class T, typename P1, typename P2, typename P3, typename P4, typename P5>
class MemberFunctionFactory5 {
private:
    template <R (T::*func)(P1, P2, P3, P4, P5)>
    static R wrapper(const void *o, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5)
    {
        T *obj = const_cast<T *>(static_cast<const T *>(o));
        return (obj->*func)(a1, a2, a3, a4, a5);
    }

public:
    template <R (T::*func)(P1, P2, P3, P4, P5)>
    inline static Function<R (P1, P2, P3, P4, P5)> bind(T *o)
    {
        return Function<R (P1, P2, P3, P4, P5)>(&MemberFunctionFactory5::template wrapper<func>,
                                static_cast<const void *>(o));
    }
};

template <typename R, class T, typename P1, typename P2, typename P3, typename P4, typename P5>
inline MemberFunctionFactory5<R, T, P1, P2, P3, P4, P5> _getFunctionFactory(R (T::*)(P1, P2, P3, P4, P5))
{
    return MemberFunctionFactory5<R, T, P1, P2, P3, P4, P5>();
}

template <typename R, class T, typename P1, typename P2, typename P3, typename P4, typename P5>
class ConstMemberFunctionFactory5 {
private:
    template <R (T::*func)(P1, P2, P3, P4, P5) const>
    static R wrapper(const void *o, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5)
    {
        const T *obj = static_cast<const T *>(o);
        return (obj->*func)(a1, a2, a3, a4, a5);
    }

public:
    template <R (T::*func)(P1, P2, P3, P4, P5) const>
    inline static Function<R (P1, P2, P3, P4, P5)> bind(const T *o)
    {
        return Function<R (P1, P2, P3, P4, P5)>(&ConstMemberFunctionFactory5::template wrapper<func>,
                              static_cast<const void *>(o));
    }
};

template <typename R, class T, typename P1, typename P2, typename P3, typename P4, typename P5>
inline ConstMemberFunctionFactory5<R, T, P1, P2, P3, P4, P5> _getFunctionFactory(R (T::*)(P1, P2, P3, P4, P5) const)
{
    return ConstMemberFunctionFactory5<R, T, P1, P2, P3, P4, P5>();
}

// 6 parameter
template <typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
class Function<R (P1, P2, P3, P4, P5, P6)> {
public:
    Function() :
        func_(0), obj_(0) {}
    Function(NullFunction) :
        func_(0), obj_(0) {}
    Function(const Function &rhs) :
        func_(rhs.func_), obj_(rhs.obj_) {}

    Function &operator=(NullFunction)
        { func_ = 0; obj_ = 0; return *this; }
    Function &operator=(const Function &rhs)
        { func_ = rhs.func_; obj_ = rhs.obj_; return *this; }

    inline R operator()(P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6) const
    {
        return (*func_)(obj_, a1, a2, a3, a4, a5, a6);
    }

    BRICKRED_SAFE_BOOL_TYPE(Function)
    operator SafeBoolType() const
    {
        return func_ != 0 ? &Function::SafeBoolTypeNotNull : 0;
    }

private:
    typedef R (*FunctionType)(const void *, P1, P2, P3, P4, P5, P6);
    Function(FunctionType func, const void *obj) :
        func_(func), obj_(obj) {}

    template <typename FR, typename FP1, typename FP2, typename FP3, typename FP4, typename FP5, typename FP6>
    friend class FreeFunctionFactory6;
    template <typename FR, class FT, typename FP1, typename FP2, typename FP3, typename FP4, typename FP5, typename FP6>
    friend class MemberFunctionFactory6;
    template <typename FR, class FT, typename FP1, typename FP2, typename FP3, typename FP4, typename FP5, typename FP6>
    friend class ConstMemberFunctionFactory6;

    FunctionType func_;
    const void *obj_;
};

template <typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
void operator==(const Function<R (P1, P2, P3, P4, P5, P6)> &, const Function<R (P1, P2, P3, P4, P5, P6)> &);
template <typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
void operator!=(const Function<R (P1, P2, P3, P4, P5, P6)> &, const Function<R (P1, P2, P3, P4, P5, P6)> &);

template <typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
class FreeFunctionFactory6 {
private:
    template <R (*func)(P1, P2, P3, P4, P5, P6)>
    static R wrapper(const void *, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6)
    {
        return (*func)(a1, a2, a3, a4, a5, a6);
    }

public:
    template <R (*func)(P1, P2, P3, P4, P5, P6)>
    inline static Function<R (P1, P2, P3, P4, P5, P6)> bind()
    {
        return Function<R (P1, P2, P3, P4, P5, P6)>(&FreeFunctionFactory6::template wrapper<func>, 0);
    }
};

template <typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
inline FreeFunctionFactory6<R, P1, P2, P3, P4, P5, P6> _getFunctionFactory(R (*)(P1, P2, P3, P4, P5, P6))
{
    return FreeFunctionFactory6<R, P1, P2, P3, P4, P5, P6>();
}

template <typename R, class T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
class MemberFunctionFactory6 {
private:
    template <R (T::*func)(P1, P2, P3, P4, P5, P6)>
    static R wrapper(const void *o, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6)
    {
        T *obj = const_cast<T *>(static_cast<const T *>(o));
        return (obj->*func)(a1, a2, a3, a4, a5, a6);
    }

public:
    template <R (T::*func)(P1, P2, P3, P4, P5, P6)>
    inline static Function<R (P1, P2, P3, P4, P5, P6)> bind(T *o)
    {
        return Function<R (P1, P2, P3, P4, P5, P6)>(&MemberFunctionFactory6::template wrapper<func>,
                                static_cast<const void *>(o));
    }
};

template <typename R, class T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
inline MemberFunctionFactory6<R, T, P1, P2, P3, P4, P5, P6> _getFunctionFactory(R (T::*)(P1, P2, P3, P4, P5, P6))
{
    return MemberFunctionFactory6<R, T, P1, P2, P3, P4, P5, P6>();
}

template <typename R, class T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
class ConstMemberFunctionFactory6 {
private:
    template <R (T::*func)(P1, P2, P3, P4, P5, P6) const>
    static R wrapper(const void *o, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6)
    {
        const T *obj = static_cast<const T *>(o);
        return (obj->*func)(a1, a2, a3, a4, a5, a6);
    }

public:
    template <R (T::*func)(P1, P2, P3, P4, P5, P6) const>
    inline static Function<R (P1, P2, P3, P4, P5, P6)> bind(const T *o)
    {
        return Function<R (P1, P2, P3, P4, P5, P6)>(&ConstMemberFunctionFactory6::template wrapper<func>,
                              static_cast<const void *>(o));
    }
};

template <typename R, class T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
inline ConstMemberFunctionFactory6<R, T, P1, P2, P3, P4, P5, P6> _getFunctionFactory(R (T::*)(P1, P2, P3, P4, P5, P6) const)
{
    return ConstMemberFunctionFactory6<R, T, P1, P2, P3, P4, P5, P6>();
}

// 7 parameter
template <typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
class Function<R (P1, P2, P3, P4, P5, P6, P7)> {
public:
    Function() :
        func_(0), obj_(0) {}
    Function(NullFunction) :
        func_(0), obj_(0) {}
    Function(const Function &rhs) :
        func_(rhs.func_), obj_(rhs.obj_) {}

    Function &operator=(NullFunction)
        { func_ = 0; obj_ = 0; return *this; }
    Function &operator=(const Function &rhs)
        { func_ = rhs.func_; obj_ = rhs.obj_; return *this; }

    inline R operator()(P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6, P7 a7) const
    {
        return (*func_)(obj_, a1, a2, a3, a4, a5, a6, a7);
    }

    BRICKRED_SAFE_BOOL_TYPE(Function)
    operator SafeBoolType() const
    {
        return func_ != 0 ? &Function::SafeBoolTypeNotNull : 0;
    }

private:
    typedef R (*FunctionType)(const void *, P1, P2, P3, P4, P5, P6, P7);
    Function(FunctionType func, const void *obj) :
        func_(func), obj_(obj) {}

    template <typename FR, typename FP1, typename FP2, typename FP3, typename FP4, typename FP5, typename FP6, typename FP7>
    friend class FreeFunctionFactory7;
    template <typename FR, class FT, typename FP1, typename FP2, typename FP3, typename FP4, typename FP5, typename FP6, typename FP7>
    friend class MemberFunctionFactory7;
    template <typename FR, class FT, typename FP1, typename FP2, typename FP3, typename FP4, typename FP5, typename FP6, typename FP7>
    friend class ConstMemberFunctionFactory7;

    FunctionType func_;
    const void *obj_;
};

template <typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
void operator==(const Function<R (P1, P2, P3, P4, P5, P6, P7)> &, const Function<R (P1, P2, P3, P4, P5, P6, P7)> &);
template <typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
void operator!=(const Function<R (P1, P2, P3, P4, P5, P6, P7)> &, const Function<R (P1, P2, P3, P4, P5, P6, P7)> &);

template <typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
class FreeFunctionFactory7 {
private:
    template <R (*func)(P1, P2, P3, P4, P5, P6, P7)>
    static R wrapper(const void *, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6, P7 a7)
    {
        return (*func)(a1, a2, a3, a4, a5, a6, a7);
    }

public:
    template <R (*func)(P1, P2, P3, P4, P5, P6, P7)>
    inline static Function<R (P1, P2, P3, P4, P5, P6, P7)> bind()
    {
        return Function<R (P1, P2, P3, P4, P5, P6, P7)>(&FreeFunctionFactory7::template wrapper<func>, 0);
    }
};

template <typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
inline FreeFunctionFactory7<R, P1, P2, P3, P4, P5, P6, P7> _getFunctionFactory(R (*)(P1, P2, P3, P4, P5, P6, P7))
{
    return FreeFunctionFactory7<R, P1, P2, P3, P4, P5, P6, P7>();
}

template <typename R, class T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
class MemberFunctionFactory7 {
private:
    template <R (T::*func)(P1, P2, P3, P4, P5, P6, P7)>
    static R wrapper(const void *o, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6, P7 a7)
    {
        T *obj = const_cast<T *>(static_cast<const T *>(o));
        return (obj->*func)(a1, a2, a3, a4, a5, a6, a7);
    }

public:
    template <R (T::*func)(P1, P2, P3, P4, P5, P6, P7)>
    inline static Function<R (P1, P2, P3, P4, P5, P6, P7)> bind(T *o)
    {
        return Function<R (P1, P2, P3, P4, P5, P6, P7)>(&MemberFunctionFactory7::template wrapper<func>,
                                static_cast<const void *>(o));
    }
};

template <typename R, class T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
inline MemberFunctionFactory7<R, T, P1, P2, P3, P4, P5, P6, P7> _getFunctionFactory(R (T::*)(P1, P2, P3, P4, P5, P6, P7))
{
    return MemberFunctionFactory7<R, T, P1, P2, P3, P4, P5, P6, P7>();
}

template <typename R, class T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
class ConstMemberFunctionFactory7 {
private:
    template <R (T::*func)(P1, P2, P3, P4, P5, P6, P7) const>
    static R wrapper(const void *o, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6, P7 a7)
    {
        const T *obj = static_cast<const T *>(o);
        return (obj->*func)(a1, a2, a3, a4, a5, a6, a7);
    }

public:
    template <R (T::*func)(P1, P2, P3, P4, P5, P6, P7) const>
    inline static Function<R (P1, P2, P3, P4, P5, P6, P7)> bind(const T *o)
    {
        return Function<R (P1, P2, P3, P4, P5, P6, P7)>(&ConstMemberFunctionFactory7::template wrapper<func>,
                              static_cast<const void *>(o));
    }
};

template <typename R, class T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
inline ConstMemberFunctionFactory7<R, T, P1, P2, P3, P4, P5, P6, P7> _getFunctionFactory(R (T::*)(P1, P2, P3, P4, P5, P6, P7) const)
{
    return ConstMemberFunctionFactory7<R, T, P1, P2, P3, P4, P5, P6, P7>();
}

// 8 parameter
template <typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
class Function<R (P1, P2, P3, P4, P5, P6, P7, P8)> {
public:
    Function() :
        func_(0), obj_(0) {}
    Function(NullFunction) :
        func_(0), obj_(0) {}
    Function(const Function &rhs) :
        func_(rhs.func_), obj_(rhs.obj_) {}

    Function &operator=(NullFunction)
        { func_ = 0; obj_ = 0; return *this; }
    Function &operator=(const Function &rhs)
        { func_ = rhs.func_; obj_ = rhs.obj_; return *this; }

    inline R operator()(P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6, P7 a7, P8 a8) const
    {
        return (*func_)(obj_, a1, a2, a3, a4, a5, a6, a7, a8);
    }

    BRICKRED_SAFE_BOOL_TYPE(Function)
    operator SafeBoolType() const
    {
        return func_ != 0 ? &Function::SafeBoolTypeNotNull : 0;
    }

private:
    typedef R (*FunctionType)(const void *, P1, P2, P3, P4, P5, P6, P7, P8);
    Function(FunctionType func, const void *obj) :
        func_(func), obj_(obj) {}

    template <typename FR, typename FP1, typename FP2, typename FP3, typename FP4, typename FP5, typename FP6, typename FP7, typename FP8>
    friend class FreeFunctionFactory8;
    template <typename FR, class FT, typename FP1, typename FP2, typename FP3, typename FP4, typename FP5, typename FP6, typename FP7, typename FP8>
    friend class MemberFunctionFactory8;
    template <typename FR, class FT, typename FP1, typename FP2, typename FP3, typename FP4, typename FP5, typename FP6, typename FP7, typename FP8>
    friend class ConstMemberFunctionFactory8;

    FunctionType func_;
    const void *obj_;
};

template <typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
void operator==(const Function<R (P1, P2, P3, P4, P5, P6, P7, P8)> &, const Function<R (P1, P2, P3, P4, P5, P6, P7, P8)> &);
template <typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
void operator!=(const Function<R (P1, P2, P3, P4, P5, P6, P7, P8)> &, const Function<R (P1, P2, P3, P4, P5, P6, P7, P8)> &);

template <typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
class FreeFunctionFactory8 {
private:
    template <R (*func)(P1, P2, P3, P4, P5, P6, P7, P8)>
    static R wrapper(const void *, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6, P7 a7, P8 a8)
    {
        return (*func)(a1, a2, a3, a4, a5, a6, a7, a8);
    }

public:
    template <R (*func)(P1, P2, P3, P4, P5, P6, P7, P8)>
    inline static Function<R (P1, P2, P3, P4, P5, P6, P7, P8)> bind()
    {
        return Function<R (P1, P2, P3, P4, P5, P6, P7, P8)>(&FreeFunctionFactory8::template wrapper<func>, 0);
    }
};

template <typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
inline FreeFunctionFactory8<R, P1, P2, P3, P4, P5, P6, P7, P8> _getFunctionFactory(R (*)(P1, P2, P3, P4, P5, P6, P7, P8))
{
    return FreeFunctionFactory8<R, P1, P2, P3, P4, P5, P6, P7, P8>();
}

template <typename R, class T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
class MemberFunctionFactory8 {
private:
    template <R (T::*func)(P1, P2, P3, P4, P5, P6, P7, P8)>
    static R wrapper(const void *o, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6, P7 a7, P8 a8)
    {
        T *obj = const_cast<T *>(static_cast<const T *>(o));
        return (obj->*func)(a1, a2, a3, a4, a5, a6, a7, a8);
    }

public:
    template <R (T::*func)(P1, P2, P3, P4, P5, P6, P7, P8)>
    inline static Function<R (P1, P2, P3, P4, P5, P6, P7, P8)> bind(T *o)
    {
        return Function<R (P1, P2, P3, P4, P5, P6, P7, P8)>(&MemberFunctionFactory8::template wrapper<func>,
                                static_cast<const void *>(o));
    }
};

template <typename R, class T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
inline MemberFunctionFactory8<R, T, P1, P2, P3, P4, P5, P6, P7, P8> _getFunctionFactory(R (T::*)(P1, P2, P3, P4, P5, P6, P7, P8))
{
    return MemberFunctionFactory8<R, T, P1, P2, P3, P4, P5, P6, P7, P8>();
}

template <typename R, class T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
class ConstMemberFunctionFactory8 {
private:
    template <R (T::*func)(P1, P2, P3, P4, P5, P6, P7, P8) const>
    static R wrapper(const void *o, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6, P7 a7, P8 a8)
    {
        const T *obj = static_cast<const T *>(o);
        return (obj->*func)(a1, a2, a3, a4, a5, a6, a7, a8);
    }

public:
    template <R (T::*func)(P1, P2, P3, P4, P5, P6, P7, P8) const>
    inline static Function<R (P1, P2, P3, P4, P5, P6, P7, P8)> bind(const T *o)
    {
        return Function<R (P1, P2, P3, P4, P5, P6, P7, P8)>(&ConstMemberFunctionFactory8::template wrapper<func>,
                              static_cast<const void *>(o));
    }
};

template <typename R, class T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
inline ConstMemberFunctionFactory8<R, T, P1, P2, P3, P4, P5, P6, P7, P8> _getFunctionFactory(R (T::*)(P1, P2, P3, P4, P5, P6, P7, P8) const)
{
    return ConstMemberFunctionFactory8<R, T, P1, P2, P3, P4, P5, P6, P7, P8>();
}

// 9 parameter
template <typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
class Function<R (P1, P2, P3, P4, P5, P6, P7, P8, P9)> {
public:
    Function() :
        func_(0), obj_(0) {}
    Function(NullFunction) :
        func_(0), obj_(0) {}
    Function(const Function &rhs) :
        func_(rhs.func_), obj_(rhs.obj_) {}

    Function &operator=(NullFunction)
        { func_ = 0; obj_ = 0; return *this; }
    Function &operator=(const Function &rhs)
        { func_ = rhs.func_; obj_ = rhs.obj_; return *this; }

    inline R operator()(P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6, P7 a7, P8 a8, P9 a9) const
    {
        return (*func_)(obj_, a1, a2, a3, a4, a5, a6, a7, a8, a9);
    }

    BRICKRED_SAFE_BOOL_TYPE(Function)
    operator SafeBoolType() const
    {
        return func_ != 0 ? &Function::SafeBoolTypeNotNull : 0;
    }

private:
    typedef R (*FunctionType)(const void *, P1, P2, P3, P4, P5, P6, P7, P8, P9);
    Function(FunctionType func, const void *obj) :
        func_(func), obj_(obj) {}

    template <typename FR, typename FP1, typename FP2, typename FP3, typename FP4, typename FP5, typename FP6, typename FP7, typename FP8, typename FP9>
    friend class FreeFunctionFactory9;
    template <typename FR, class FT, typename FP1, typename FP2, typename FP3, typename FP4, typename FP5, typename FP6, typename FP7, typename FP8, typename FP9>
    friend class MemberFunctionFactory9;
    template <typename FR, class FT, typename FP1, typename FP2, typename FP3, typename FP4, typename FP5, typename FP6, typename FP7, typename FP8, typename FP9>
    friend class ConstMemberFunctionFactory9;

    FunctionType func_;
    const void *obj_;
};

template <typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
void operator==(const Function<R (P1, P2, P3, P4, P5, P6, P7, P8, P9)> &, const Function<R (P1, P2, P3, P4, P5, P6, P7, P8, P9)> &);
template <typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
void operator!=(const Function<R (P1, P2, P3, P4, P5, P6, P7, P8, P9)> &, const Function<R (P1, P2, P3, P4, P5, P6, P7, P8, P9)> &);

template <typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
class FreeFunctionFactory9 {
private:
    template <R (*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9)>
    static R wrapper(const void *, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6, P7 a7, P8 a8, P9 a9)
    {
        return (*func)(a1, a2, a3, a4, a5, a6, a7, a8, a9);
    }

public:
    template <R (*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9)>
    inline static Function<R (P1, P2, P3, P4, P5, P6, P7, P8, P9)> bind()
    {
        return Function<R (P1, P2, P3, P4, P5, P6, P7, P8, P9)>(&FreeFunctionFactory9::template wrapper<func>, 0);
    }
};

template <typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
inline FreeFunctionFactory9<R, P1, P2, P3, P4, P5, P6, P7, P8, P9> _getFunctionFactory(R (*)(P1, P2, P3, P4, P5, P6, P7, P8, P9))
{
    return FreeFunctionFactory9<R, P1, P2, P3, P4, P5, P6, P7, P8, P9>();
}

template <typename R, class T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
class MemberFunctionFactory9 {
private:
    template <R (T::*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9)>
    static R wrapper(const void *o, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6, P7 a7, P8 a8, P9 a9)
    {
        T *obj = const_cast<T *>(static_cast<const T *>(o));
        return (obj->*func)(a1, a2, a3, a4, a5, a6, a7, a8, a9);
    }

public:
    template <R (T::*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9)>
    inline static Function<R (P1, P2, P3, P4, P5, P6, P7, P8, P9)> bind(T *o)
    {
        return Function<R (P1, P2, P3, P4, P5, P6, P7, P8, P9)>(&MemberFunctionFactory9::template wrapper<func>,
                                static_cast<const void *>(o));
    }
};

template <typename R, class T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
inline MemberFunctionFactory9<R, T, P1, P2, P3, P4, P5, P6, P7, P8, P9> _getFunctionFactory(R (T::*)(P1, P2, P3, P4, P5, P6, P7, P8, P9))
{
    return MemberFunctionFactory9<R, T, P1, P2, P3, P4, P5, P6, P7, P8, P9>();
}

template <typename R, class T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
class ConstMemberFunctionFactory9 {
private:
    template <R (T::*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9) const>
    static R wrapper(const void *o, P1 a1, P2 a2, P3 a3, P4 a4, P5 a5, P6 a6, P7 a7, P8 a8, P9 a9)
    {
        const T *obj = static_cast<const T *>(o);
        return (obj->*func)(a1, a2, a3, a4, a5, a6, a7, a8, a9);
    }

public:
    template <R (T::*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9) const>
    inline static Function<R (P1, P2, P3, P4, P5, P6, P7, P8, P9)> bind(const T *o)
    {
        return Function<R (P1, P2, P3, P4, P5, P6, P7, P8, P9)>(&ConstMemberFunctionFactory9::template wrapper<func>,
                              static_cast<const void *>(o));
    }
};

template <typename R, class T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
inline ConstMemberFunctionFactory9<R, T, P1, P2, P3, P4, P5, P6, P7, P8, P9> _getFunctionFactory(R (T::*)(P1, P2, P3, P4, P5, P6, P7, P8, P9) const)
{
    return ConstMemberFunctionFactory9<R, T, P1, P2, P3, P4, P5, P6, P7, P8, P9>();
}

} // namespace brickred

#endif
