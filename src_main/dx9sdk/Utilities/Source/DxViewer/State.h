#pragma once

#pragma warning(disable: 4995)
#include <list>
#include <string>
#pragma warning(default: 4995)
#include <windows.h>


#define STATE_LOCK( _state_ , _type_ )  STATE_LOCKV( _state_ , _type_ , _state_ )
#define STATE_LOCKV( _state_ , _type_ , _variable_ )  { StackAssert UnlockCheck_##_state_ ( __LINE__ ); _type_ & _variable_ = DXVGetStateBlock().##_state_.Lock();
#define STATE_UNLOCK( _state_ ) DXVGetStateBlock().##_state_##.Unlock(); UnlockCheck_##_state_##.Disable(); }

#define STATE_GET( _state_ ) DXVGetStateBlock().##_state_##.Get()
#define STATE_SET( _state_ , _value_ ) DXVGetStateBlock().##_state_##.Set( _value_ )

typedef void (CALLBACK *LPDXVONSTATECHANGE)();

template < typename T > 
class State
{
protected:
    State(){}//MUST USE DERIVES CLASSES
    std::list<LPDXVONSTATECHANGE> m_Listeners;
    T m_Value;

public:

    const T& Get() //read only
    {
        return m_Value;
    }

    void Notify()
    {
        for(std::list<LPDXVONSTATECHANGE>::const_iterator itr = m_Listeners.begin();
            itr != m_Listeners.end();
            itr++ )
        {
            (*itr)();//call the function stored
        }
    }

    void Listen( LPDXVONSTATECHANGE p )
    {
        m_Listeners.push_back(p);
    }

    void Unlisten( LPDXVONSTATECHANGE p )
    {
        for(std::list<LPDXVONSTATECHANGE>::const_iterator itr = m_Listeners.begin();
            itr != m_Listeners.end();
            itr++ )
        {
            if( *itr == p )
            {
                m_Listeners.erase(itr);
                return;
            }
        }
    }
};

template < typename T > 
class StateV : public State<T>
{
public:
    StateV() {}

    T Get()
    {
        return m_Value;
    }    
    
    void Set( T value )
    {
        m_Value = value;

        Notify();
    }
};

template < typename T > 
class StateR : public State<T>
{
private:
    bool Locked;
public:
    StateR() { Locked = false; }

    T& Lock()
    {
        if(Locked)
            DebugBreak();
        Locked = true;
        return m_Value;
    }

    void Unlock()
    {
        Locked = false;
        Notify();
    }
};


class StackAssert
{
private:
    long Line;
    bool Active;
public:
    StackAssert(long line ) { Line = line; Active = true;} 
    void Disable(){ Active = false; }
    ~StackAssert() { if(Active){ DebugBreak(); } }
};