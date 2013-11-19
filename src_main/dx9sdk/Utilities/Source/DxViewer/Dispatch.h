#pragma once

#pragma warning(disable: 4995)
#include <list>
#include <string>
#pragma warning(default: 4995)

enum DISPATCH_MODE
{
    DISPATCH_FIFO,
    DISPATCH_LIFO,
};

#define DISPATCH_FUNCTION_ITERATE( T, V, I )  for( Dispatcher< T >::Iterator I = V##.Iterate(); ! I##.Done();    I##.Next() )
#define DISPATCH_FUNCTION_ITERATE_HR( T, V, I, hr )  for( Dispatcher< T >::Iterator I = V##.Iterate(); SUCCEEDED( hr ) && ! I##.Done();    I##.Next() )


template<typename T> 
class Dispatcher
{
public:
    struct Element
    {
        Element(T fn, void* ud) { Function = fn; UserData = ud; }

        typename T Function;
        void* UserData;

        bool operator == ( const Element& value )
        {
            return Function == value.Function;
        }
    };

    class Iterator  
    {
    protected:
        typename std::list<Element>::const_iterator Itr;
        typename std::list<Element>::const_iterator Beg;
        typename std::list<Element>::const_iterator End;
        DISPATCH_MODE Mode;
    public:

        Iterator(){ Mode =  DISPATCH_FIFO; }
        Iterator( typename std::list<Element>& list, DISPATCH_MODE mode ){ Begin(list, mode); }

        Iterator& Begin( typename std::list<Element>& list, DISPATCH_MODE mode )
        {
            Mode = mode;
            if( Mode ==  DISPATCH_FIFO)
            {
                Beg = Itr = list.begin();
                End = list.end();
            }
            else
            {
                Beg = list.begin();
                Itr = --list.end();
                End = list.end();
            }

            return *this;
        }
        
        Iterator& Next()
        {
            if( Mode ==  DISPATCH_FIFO && !Done())
                Itr++;
            else if( Mode ==  DISPATCH_LIFO && !Done())
            {
                if (Itr == Beg)
                    Itr = End;   // wrap to end to mark Done()
                else
                    Itr--;
            }

            return *this;
        }

        bool Done()
        {
            return Itr == End;
        }

        T GetFunction() 
        { 
            return Itr->Function; 
        }
        
        void* GetUserData() 
        { 
            return Itr->UserData; 
        }
    };


    Dispatcher( DISPATCH_MODE mode = DISPATCH_FIFO) { SetMode(mode); }
    
    DISPATCH_MODE GetMode() { return Mode; }
    void SetMode(DISPATCH_MODE mode) { Mode = mode; }
    
    void AddFunc(T function, void* userdata = NULL){ FuncList.push_back( Element(function, userdata) ); }
    void RemoveFunc(T function, void* userdata = NULL){ FuncList.remove( Element(function, userdata) ); }

    Iterator Iterate() { Iterator result( FuncList, Mode); return result; }

protected:
    std::list<Element> FuncList;
    DISPATCH_MODE Mode;
};


