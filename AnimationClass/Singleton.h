#ifndef SINGLETON_H
#define SINGLETON_H

template <class T>
class Singleton
{
public:
    static void DestroyT()
    {
        delete Instance();
    }

    static T* Instance()
    {
        static T* p = 0;
        if (!p)
        {
            p = new T;
            atexit(DestroyT);
        }
        return p;
    }
};

class SingletonBase
{
public:
    SingletonBase() {}
private:
    // Prevent copying
    SingletonBase(const SingletonBase&);
    SingletonBase& operator=(const SingletonBase&);
};

#endif
