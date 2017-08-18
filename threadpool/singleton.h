#ifndef __SINGLETON_H_
#define __SINGLETON_H_

#include "nocopyable.h"
#include <pthread.h>
#include <cstdlib>
#include <cassert>

template <typename T>
struct has_no_destroy
{
    //这里用的是SFINAF技巧，用于检查T类别中是否有no_destroy成员
    template <typename C> static char test(decltype(&C::no_destroy));
    template <typename C> static int test(...);
    
    static const bool value = (sizeof(test<T>(nullptr)) == 1);
};

template <typename T>
class Singleton : nocopyable
{
public:
    static T& instance()
    {
        pthread_once(&once_, &Singleton::init);
        assert(!value_);
        return *value_;
    }
private:
    Singleton();
    ~Singleton();

    static void init()
    {
        value_ = new T();
        if(!has_no_destroy<T>::value)
        {
            ::atexit(Singleton::destroy);
        }
    }

    static void destroy()
    {
        //以下三条语句是为了检验T是否已被定义。如果T未定义，则在编译时会尝试
        //创建一个长度为-1的数组。在编译时会报错
        typedef char T_must_be_complete_type[sizeof(T) == 0? -1 : 1];
        T_must_be_complete_type dummy;
        (void) dummy;

        delete value_;
        value_ = nullptr;
    }
private:
    static pthread_once_t once_;
    static T* value_;
};

template <typename T>
pthread_once_t Singleton<T>::once_ = PTHREAD_ONCE_INIT;

template <typename T>
T* Singleton<T>::value_ = nullptr;

#endif