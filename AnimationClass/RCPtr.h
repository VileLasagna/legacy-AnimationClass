/*
Amju Games source code (c) Copyright Jason Colman 2000-2008
*/

#ifndef RCPTR_H
#define RCPTR_H

class RefCounted
{
public:
    RefCounted() : m_refCount(0) {}
    virtual ~RefCounted() {}
    int m_refCount;
};

template <class T>
class RCPtr
{
public:
    RCPtr(T* ptr = 0) : m_ptr(ptr)
    {
        if (m_ptr)
        {
            m_ptr->m_refCount++;
        }
    }

    RCPtr(const RCPtr<T>& r)
    {
        m_ptr = r.m_ptr;
        if (m_ptr)
        {
            m_ptr->m_refCount++;
        }
    }

    RCPtr<T>& operator=(const RCPtr<T>& r)
    {
        if (m_ptr == r.m_ptr)
        {
            return *this;
        }

        if (m_ptr)
        {
            m_ptr->m_refCount--;
            if (m_ptr->m_refCount == 0)
            {
                delete m_ptr;
            }
        }

        m_ptr = r.m_ptr;
        if (m_ptr)
        {
            m_ptr->m_refCount++;
        }

        return *this;
    }

    ~RCPtr()
    {
        if (m_ptr)
        {
            m_ptr->m_refCount--;
            if (m_ptr->m_refCount == 0)
            {
                delete m_ptr;
            }
        }
    }

    T* operator->() { return m_ptr; }

    operator T*() { return m_ptr; }

private:
    T* m_ptr;
};

#endif
