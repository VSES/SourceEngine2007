
#ifndef __ComPtr_h__
#define __ComPtr_h__
#pragma once

// The world's simplest com ptr

template<typename Ty>
class ComPtr
{
public:

    ComPtr()
    {
        ptr = NULL;
    }

    ComPtr(const ComPtr& rhs)
    {
        ptr = NULL;
        Set(rhs.ptr);
    }

    ComPtr(Ty* p)
    {
        ptr = NULL;
        Set(p);
    }

    ~ComPtr()
    {
        Reset();
    }

    const ComPtr& operator=(const ComPtr& rhs)
    {
        Set(rhs.ptr);
        return *this;
    }

    Ty* operator=(Ty* p)
    {
        Set(p);
        return p;
    }

    operator Ty* ()
    {
        return ptr;
    }

    Ty* operator->()
    {
        return ptr;
    }

    operator Ty** ()
    {
        Reset();
        return &ptr;
    }

    operator void** ()
    {
        Reset();
        return (void**)&ptr;
    }

    bool IsNull()
    {
        return ptr == NULL;
    }

    void Set(Ty* p)
    {
        Reset();
        
        ptr = p;
        if (ptr)
        {
            ptr->AddRef();
        }
    }

    void Reset()
    {
        if (ptr)
        {
            ptr->Release();
            ptr = NULL;
        }
    }

protected:
    Ty* ptr;
};

#endif
