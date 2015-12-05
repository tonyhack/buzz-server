#ifndef BRICKRED_CLASS_UTIL_H
#define BRICKRED_CLASS_UTIL_H

#define BRICKRED_NONCOPYABLE(_T) \
    _T(const _T &);              \
    _T &operator=(const _T &);   \

#define BRICKRED_SINGLETON(_T)                 \
public:                                        \
    static _T *getInstance()                   \
    {                                          \
        static _T obj;                         \
        return &obj;                           \
    }                                          \
                                               \
private:                                       \
    _T();                                      \
    ~_T();                                     \
    _T(const _T &);                            \
    _T &operator=(const _T &);                 \

#define BRICKRED_SINGLETON2(_T)                \
public:                                        \
    static _T *getInstance()                   \
    {                                          \
        static _T obj;                         \
        return &obj;                           \
    }                                          \
                                               \
private:                                       \
    struct ObjectCreator {                     \
        ObjectCreator() { _T::getInstance(); } \
    };                                         \
    static ObjectCreator oc;                   \
                                               \
    _T();                                      \
    ~_T();                                     \
    _T(const _T &);                            \
    _T &operator=(const _T &);                 \


#define BRICKRED_SINGLETON2_IMPL(_T) \
    _T::ObjectCreator _T::oc;        \

#define BRICKRED_SAFE_BOOL_TYPE(_T)           \
private:                                      \
    typedef void (_T::*SafeBoolType)() const; \
    void SafeBoolTypeNotNull() const {}       \
public:                                       \


#endif
