#pragma once

//BEGIN//DO NOT USE THESE OUTSIDE OF THIS FILE
#define SINGLETON_STDFUNCTION( __TYPE ) __TYPE##& DXVGet##__TYPE##()
#define SINGLETON_DIRFUNCTION( __TYPE ) __TYPE##& DXVGet##__TYPE##( const WCHAR* contentDirectory )
#define SINGLETON_NAME( __TYPE ) s_##__TYPE
#define SINGLETON_TYPE( __TYPE ) __TYPE
#define SINGLETON_VARIABLE( __TYPE ) SINGLETON_TYPE( __TYPE ) SINGLETON_NAME( __TYPE )
#define WATCH_NAME( __TYPE ) g_##__TYPE
#define WATCH_TYPE( __TYPE ) __TYPE##*
#define WATCH_VARIABLE( __TYPE ) WATCH_TYPE( __TYPE ) WATCH_NAME( __TYPE )

#if defined(DEBUG) | defined(_DEBUG)
    #define WATCH_VARIABLE_EXTERN( __TYPE ) extern WATCH_VARIABLE( __TYPE )
    #define WATCH_VARIABLE_GLOBAL( __TYPE ) WATCH_VARIABLE( __TYPE ) = NULL;
    #define WATCH_ASSIGNMENT( __TYPE ) \
        static bool FirstTime = true; \
        if(FirstTime) \
        { \
            FirstTime = false; \
            WATCH_NAME( __TYPE ) = & SINGLETON_NAME( __TYPE ); \
        } 
#else
    #define WATCH_VARIABLE_EXTERN( __TYPE )
    #define WATCH_VARIABLE_GLOBAL( __TYPE )
    #define WATCH_ASSIGNMENT( __TYPE ) 
#endif

//END //DO NOT USE THESE OUTSIDE OF THIS FILE


#define DECLARE_SINGLETON_H( __TYPE ) \
    WATCH_VARIABLE_EXTERN( __TYPE ); \
    SINGLETON_STDFUNCTION( __TYPE);


#define DECLARE_SINGLETON_CPP( __TYPE ) \
    WATCH_VARIABLE_GLOBAL( __TYPE ) \
    SINGLETON_STDFUNCTION( __TYPE) \
    { \
        static SINGLETON_VARIABLE( __TYPE ); \
        WATCH_ASSIGNMENT( __TYPE ); \
        return SINGLETON_NAME( __TYPE ); \
    }

#define FRIEND_SINGLETON_H( __TYPE ) friend SINGLETON_STDFUNCTION( __TYPE );


#define DECLARE_SINGLETON_DIR_H( __TYPE ) \
    WATCH_VARIABLE_EXTERN( __TYPE ); \
    SINGLETON_DIRFUNCTION( __TYPE);

#define DECLARE_SINGLETON_DIR_CPP( __TYPE, __DIR ) \
    WATCH_VARIABLE_GLOBAL( __TYPE ) \
    SINGLETON_DIRFUNCTION( __TYPE) \
    { \
        static SINGLETON_VARIABLE( __TYPE ); \
        WATCH_ASSIGNMENT( __TYPE ); \
        SINGLETON_NAME( __TYPE ).##__DIR = (contentDirectory ? contentDirectory : L""); \
        return SINGLETON_NAME( __TYPE ); \
    }

#define FRIEND_SINGLETON_DIR_H( __TYPE ) friend SINGLETON_DIRFUNCTION( __TYPE );



#ifndef SAFE_CAST_DELETE
    #define SAFE_CAST_DELETE(p, c)       { if(p) { delete ((c*)(p)); (p)=NULL; } }
#endif 
#ifndef LOOPSAFE_DELETE
    #define LOOPSAFE_DELETE(p, c)       { if(p) { c* __tmp = (c*)(p); (p) = NULL; delete ((c*)(__tmp)); __tmp = NULL; } }
#endif 