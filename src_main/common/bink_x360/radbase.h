//  __RAD16__ means 16 bit code (Win16)
//  __RAD32__ means 32 bit code (DOS, Win386, Win32s, Mac AND Win64)
//  __RAD64__ means 64 bit code (x64)
//
// Note oddness - __RAD32__ essentially means "at *least* 32-bit code". 
// So, on 64-bit systems, both __RAD32__ and __RAD64__ will be defined.

//  __RADDOS__ means DOS code (16 or 32 bit)
//  __RADWIN__ means Windows code (Win16, Win386, Win32s, Win64s)
//  __RADWINEXT__ means Windows 386 extender (Win386)
//  __RADNT__ means Win32s or Win64 code
//  __RADMAC__ means Macintosh
//  __RADCARBON__ means Carbon
//  __RADMACH__ means MachO
//  __RADXBOX__ means the XBox console
//  __RADXENON__ means the Xenon console
//  __RADNGC__ means the Nintendo GameCube
//  __RADWII__ means the Nintendo Wii
//  __RADNDS__ means the Nintendo DS
//  __RADPS2__ means the Sony PlayStation 2
//  __RADPSP__ means the Sony PlayStation Portable
//  __RADPS3__ means the Sony PlayStation 3
//  __RADNTBUILDLINUX__ means building Linux on NT
//  __RADLINUX__ means actually building on Linux (most likely with GCC)

//  __RADX86__ means Intel x86 
//  __RADMMX__ means Intel x86 MMX instructions are allowed 
//  __RAD64__ means Intel/AMD x64 (NOT IA64=Itanium)
//  __RAD68K__ means 68K
//  __RADPPC__ means PowerPC
//  __RADMIPS__ means Mips (only R5900 right now)

// __RADLITTLEENDIAN__ means processor is little-endian (x86)
// __RADBIGENDIAN__ means processor is big-endian (680x0, PPC)

#ifndef __RADBASEH__
  #define __RADBASEH__

  #define RADCOPYRIGHT "Copyright (C) 1994-2007, RAD Game Tools, Inc."

  #ifndef __RADRES__

    #define RADSTRUCT struct

    #if defined(_WIN64)

      #define __RADWIN__
      #define __RADNT__
      // See note at top for why both __RAD32__ and __RAD64__ are defined.
      #define __RAD32__
      #define __RAD64__
      #define __RADX64__
      #define __RADMMX__
      #define __RADX86__
      #define __RADLITTLEENDIAN__
      #define RADINLINE __inline
      #define RADRESTRICT __restrict

    #elif defined(HOLLYWOOD_REV) || defined(REVOLUTION) 

      #define __RADWII__
      #define __RAD32__
      #define __RADPPC__
      #define __RADBIGENDIAN__
      #define RADINLINE inline
      #define RADRESTRICT __restrict

    #elif defined(GEKKO)

      #define __RADNGC__
      #define __RAD32__
      #define __RADPPC__
      #define __RADBIGENDIAN__
      #define RADINLINE inline
      #define RADRESTRICT // __restrict not supported on cw

    #elif defined(__arm) && defined(__MWERKS__)

      #define __RADNDS__
      #define __RAD32__
      #define __RADARM__
      #define __RADLITTLEENDIAN__
      #define __RADFIXEDPOINT__
      #define RADINLINE inline
      #define RADRESTRICT // __restrict not supported on cw

    #elif defined(R5900)

      #define __RADPS2__
      #define __RAD32__
      #define __RADMIPS__
      #define __RADLITTLEENDIAN__
      #define RADINLINE inline
      #define RADRESTRICT __restrict

      #if !defined(__MWERKS__)
        #undef RADSTRUCT
        #define RADSTRUCT struct __attribute__((__packed__))
      #endif

    #elif defined(__psp__)

      #define __RADPSP__
      #define __RAD32__
      #define __RADMIPS__
      #define __RADLITTLEENDIAN__
      #define RADINLINE inline
      #define RADRESTRICT __restrict

      #if !defined(__MWERKS__)
        #undef RADSTRUCT
        #define RADSTRUCT struct __attribute__((__packed__))
      #endif

    #elif defined(__CELLOS_LV2__)

      #define __RADPS3__
      #define __RADPPC__
      #define __RAD32__
      #define __RADCELL__
      #define __RADBIGENDIAN__
      #define RADINLINE inline
      #define RADRESTRICT __restrict

      #ifndef __LP32__
      #error "PS3 32bit ABI support only"
      #endif

    #elif (defined(__MWERKS__) && !defined(__INTEL__)) || defined(__MRC__) || defined(THINK_C) || defined(powerc) || defined(macintosh) || defined(__powerc) || defined(__APPLE__) || defined(__MACH__)

      #define __RADMAC__

      #if defined(powerc) || defined(__powerc) || defined(__ppc__)
        #define __RADPPC__
        #define __RADBIGENDIAN__
        #define __RADALTIVEC__
        #define RADRESTRICT 
      #elif defined(__i386__)
        #define __RADX86__
        #define __RADMMX__
        #define __RADLITTLEENDIAN__
        #define RADRESTRICT __restrict
      #else
        #define __RAD68K__
        #define __RADBIGENDIAN__
        #define __RADALTIVEC__
        #define RADRESTRICT 
      #endif

      #define __RAD32__

      #if defined(__MWERKS__)
        #if (defined(__cplusplus) || ! __option(only_std_keywords))
          #define RADINLINE inline
        #endif
        #ifdef __MACH__
          #define __RADMACH__
        #endif
      #elif defined(__MRC__)
        #if defined(__cplusplus)
          #define RADINLINE inline
        #endif
      #elif defined(__GNUC__) || defined(__GNUG__) || defined(__MACH__)
        #define RADINLINE inline
        #define __RADMACH__

        #undef RADRESTRICT  /* could have been defined above... */
        #define RADRESTRICT __restrict
      #endif

      #ifdef __RADX86__
        #ifndef __RADCARBON__
          #define __RADCARBON__
        #endif
      #endif

      #ifdef TARGET_API_MAC_CARBON
        #if TARGET_API_MAC_CARBON
          #ifndef __RADCARBON__
            #define __RADCARBON__
          #endif
        #endif
      #endif

  #elif defined(linux)

      #define __RADLINUX__
      #define __RADX86__
      #define __RADMMX__
      #define __RAD32__
      #define __RADLITTLEENDIAN__
      #define RADINLINE inline

    #else

       #if _MSC_VER >= 1400
           #undef RADRESTRICT 
           #define RADRESTRICT __restrict
       #else
           #define RADRESTRICT
       #endif

      #if defined(_XENON) || (_XBOX_VER == 200)
        // Remember that Xenon also defines _XBOX
        #define __RADPPC__
        #define __RADBIGENDIAN__
        #define __RADALTIVEC__
      #else
        #define __RADX86__
        #define __RADMMX__
        #define __RADLITTLEENDIAN__
      #endif

      #ifdef __MWERKS__
        #define _WIN32
      #endif

      #ifdef __DOS__
        #define __RADDOS__
      #endif

      #ifdef __386__
        #define __RAD32__
      #endif

      #ifdef _Windows    //For Borland
        #ifdef __WIN32__
          #define WIN32
        #else
          #define __WINDOWS__
        #endif
      #endif

      #ifdef _WINDOWS    //For MS
        #ifndef _WIN32
          #define __WINDOWS__
        #endif
      #endif

      #ifdef _WIN32
        #if defined(_XENON) || (_XBOX_VER == 200)
          // Remember that Xenon also defines _XBOX
          #define __RADXENON__
        #elif defined(_XBOX)
          #define __RADXBOX__
        #else
          #define __RADNT__
        #endif
        #define __RADWIN__
        #define __RAD32__
      #else
        #ifdef __NT__
          #if defined(_XENON) || (_XBOX_VER == 200)
          // Remember that Xenon also defines _XBOX
            #define __RADXENON__
          #elif defined(_XBOX)
            #define __RADXBOX__
          #else
            #define __RADNT__
          #endif
          #define __RADWIN__
          #define __RAD32__
        #else
          #ifdef __WINDOWS_386__
            #define __RADWIN__
            #define __RADWINEXT__
            #define __RAD32__
          #else
            #ifdef __WINDOWS__
              #define __RADWIN__
              #define __RAD16__
            #else
              #ifdef WIN32
                #if defined(_XENON) || (_XBOX_VER == 200)
                  // Remember that Xenon also defines _XBOX
                  #define __RADXENON__
                #elif defined(_XBOX)
                  #define __RADXBOX__
                #else
                  #define __RADNT__
                #endif
                #define __RADWIN__
                #define __RAD32__
              #endif
            #endif
          #endif
        #endif
      #endif

      #ifdef __WATCOMC__
        #define RADINLINE
      #else
        #define RADINLINE __inline
      #endif
    #endif

    #if (!defined(__RADDOS__) && !defined(__RADWIN__) && !defined(__RADMAC__) &&      \
         !defined(__RADNGC__) && !defined(__RADNDS__) && !defined(__RADXBOX__) &&     \
         !defined(__RADXENON__) && !defined(__RADLINUX__) && !defined(__RADPS2__) &&  \
         !defined(__RADPSP__) && !defined(__RADPS3__) && !defined(__RADWII__))
      #error "RAD.H did not detect your platform.  Define DOS, WINDOWS, WIN32, macintosh, powerpc, or appropriate console."
    #endif

    #ifdef __RADFINAL__
      #define RADTODO(str) { char __str[0]=str; }
    #else
      #define RADTODO(str)
    #endif

    #if (defined(__RADNGC__) || defined(__RADWII__) || defined( __RADPS2__) || \
         defined(__RADPSP__) || defined(__RADPS3__) || defined(__RADNDS__))

      #define RADLINK
      #define RADEXPLINK
      #define RADEXPFUNC RADDEFFUNC
      #define RADASMLINK
      #define PTR4

    #elif defined(__RADLINUX__)

      #define RADLINK __attribute__((cdecl))
      #define RADEXPLINK __attribute__((cdecl))
      #define RADEXPFUNC RADDEFFUNC
      #define RADASMLINK
      #define PTR4

    #elif defined(__RADMAC__)

      // this define is for CodeWarrior 11's stupid new libs (even though
      //   we don't use longlong's).

      #define __MSL_LONGLONG_SUPPORT__

      #define RADLINK
      #define RADEXPLINK

      #if defined(__CFM68K__) || defined(__MWERKS__)
        #ifdef __RADINDLL__
          #define RADEXPFUNC RADDEFFUNC __declspec(export)
        #else
          #define RADEXPFUNC RADDEFFUNC __declspec(import)
        #endif
      #else
        #if defined(__RADMACH__) && !defined(__MWERKS__)
          #ifdef __RADINDLL__
            #define RADEXPFUNC RADDEFFUNC __attribute__((visibility("default")))
          #else
            #define RADEXPFUNC RADDEFFUNC
          #endif
        #else
          #define RADEXPFUNC RADDEFFUNC
        #endif
      #endif
      #define RADASMLINK

    #else

      #ifdef __RADNT__
        #ifndef _WIN32
          #define _WIN32
        #endif
        #ifndef WIN32
          #define WIN32
        #endif
      #endif

      #ifdef __RADWIN__
        #ifdef __RAD32__
          #ifdef __RADXBOX__

             #define RADLINK __stdcall
             #define RADEXPLINK __stdcall
             #define RADEXPFUNC RADDEFFUNC

          #elif defined(__RADXENON__)

             #define RADLINK __stdcall
             #define RADEXPLINK __stdcall
             #define RADEXPFUNC RADDEFFUNC

          #elif defined(__RADNTBUILDLINUX__)

            #define RADLINK __cdecl
            #define RADEXPLINK __cdecl
            #define RADEXPFUNC RADDEFFUNC

          #else
            #ifdef __RADNT__

              #define RADLINK __stdcall
              #define RADEXPLINK __stdcall

              #ifdef __RADINEXE__
                #define RADEXPFUNC RADDEFFUNC
              #else
                #ifndef __RADINDLL__
                  #define RADEXPFUNC RADDEFFUNC __declspec(dllimport)
                  #ifdef __BORLANDC__
                    #if __BORLANDC__<=0x460
                      #undef RADEXPFUNC
                      #define RADEXPFUNC RADDEFFUNC
                    #endif
                  #endif
                #else
                  #define RADEXPFUNC RADDEFFUNC __declspec(dllexport)
                #endif
              #endif
            #else
              #define RADLINK __pascal
              #define RADEXPLINK __far __pascal
              #define RADEXPFUNC RADDEFFUNC
            #endif
          #endif
        #else
          #define RADLINK __pascal
          #define RADEXPLINK __far __pascal __export
          #define RADEXPFUNC RADDEFFUNC
        #endif
      #else
        #define RADLINK __pascal
        #define RADEXPLINK __pascal
        #define RADEXPFUNC RADDEFFUNC
      #endif

      #define RADASMLINK __cdecl

    #endif

    #if !defined(__RADXBOX__) && !defined(__RADXENON__)
      #ifdef __RADWIN__
        #ifndef _WINDOWS
          #define _WINDOWS
        #endif
      #endif
    #endif

    #ifndef RADDEFFUNC

      #ifdef __cplusplus
        #define RADDEFFUNC extern "C"
        #define RADDEFSTART extern "C" {
        #define RADDEFEND }
        #define RADDEFINEDATA extern "C"
        #define RADDECLAREDATA extern "C"
        #define RADDEFAULT( val ) =val
      #else
        #define RADDEFFUNC
        #define RADDEFSTART
        #define RADDEFEND
        #define RADDEFINEDATA
        #define RADDECLAREDATA extern
        #define RADDEFAULT( val )
      #endif

    #endif

   // probably s.b: RAD_DECLARE_ALIGNED(type, name, alignment)
    #if (defined(__RADNGC__) || defined(__RADWII__) || defined(__RADPS2__)  || defined(__RADPSP__) || \
         defined(__RADPS3__) || defined(__RADLINUX__) || defined(__RADMAC__)) || defined(__RADNDS__)
      #define RAD_ALIGN(type,var,num) type __attribute__ ((aligned (num))) var
    #elif (_MSC_VER >= 1300)
      #define RAD_ALIGN(type,var,num) type __declspec(align(num)) var
    #else
      #define RAD_ALIGN(type,var,num) type var
    #endif

     // Note that __RAD16__/__RAD32__/__RAD64__ refers to the size of a pointer.
    // The size of integers is specified explicitly in the code, i.e. u32 or whatever.

    #define S8 signed char
    #define U8 unsigned char
    #if defined(__RAD64__)
      // Remember that __RAD32__ will also be defined!
      // This can be either an X64, or maybe a Cell/PS3
      #if defined(__RADX64__)
        // x64 still has 32-bit ints!
        #define U32 unsigned int
        #define S32 signed int
        // But pointers are 64 bits.
        #if (_MSC_VER >= 1300 && _Wp64)
          #define INTADDR __w64 __int64
          #define UINTADDR __w64 unsigned __int64

          #define SINTa __w64 signed __int64
          #define UINTa __w64 unsigned __int64
        #else // non-vc.net compiler or /Wp64 turned off
          #define UINTADDR unsigned __int64
          #define INTADDR __int64

          #define UINTa unsigned __int64
          #define SINTa signed __int64
        #endif
      #else
        #error Unknown 64-bit processor (see radbase.h)
      #endif
    #elif defined(__RAD32__)
      #define U32 unsigned int
      #define S32 signed int
      // Pointers are 32 bits.
      #if (_MSC_VER >= 1300 && _Wp64)
        #define INTADDR __w64 long
        #define UINTADDR __w64 unsigned long

        #define SINTa __w64 signed long
        #define UINTa __w64 unsigned long
      #else // non-vc.net compiler or /Wp64 turned off
        #ifdef _Wp64
          #define INTADDR long
          #define UINTADDR unsigned long

          #define SINTa signed long
          #define UINTa unsigned long
        #else
          #define INTADDR int
          #define UINTADDR unsigned int

          #define SINTa signed int
          #define UINTa unsigned int
        #endif
      #endif
    #else
      #define U32 unsigned long
      #define S32 signed long
      // Pointers in 16-bit land are still 32 bits.
      #define UINTADDR unsigned long
      #define INTADDR signed long

      #define UINTa unsigned long
      #define SINTa signed long
    #endif
    #define F32 float
    #if defined(__RADPS2__) || defined(__RADPSP__)
      typedef RADSTRUCT F64  // do this so that we don't accidentally use doubles
      {                   //  while using the same space
        U32 vals[ 2 ];
      } F64;
    #else
      #define F64 double
    #endif

    #if defined(__RADMAC__) || defined(__MRC__) || defined( __RADNGC__ ) || defined( __RADWII__ ) || defined(__RADNDS__) || defined(__RADPSP__) || defined(__RADPS3__)
      #define U64 unsigned long long
      #define S64 signed long long
    #elif defined(__RADPS2__)
      #define U64 unsigned long
      #define S64 signed long
    #elif defined(__RADX64__) || defined(__RAD32__)
      #define U64 unsigned __int64
      #define S64 signed __int64
    #else
      // 16-bit
      typedef RADSTRUCT U64  // do this so that we don't accidentally use U64s
      {                   //  while using the same space
        U32 vals[ 2 ];
      } U64;
      typedef RADSTRUCT S64  // do this so that we don't accidentally use S64s
      {                   //  while using the same space
        S32 vals[ 2 ];
      } S64;
    #endif

    #if defined(__RAD32__)
      #define PTR4
      #define U16 unsigned short
      #define S16 signed short
    #else
      #define PTR4 __far
      #define U16 unsigned int
      #define S16 signed int
    #endif

    #if defined( __RADXENON__ ) || defined( __RADPS3__ ) || defined( __RADWII__ )
      // on next gen platforms always turn off lower case types 
      //   (so we can eventually remove them)
      #undef RAD_NO_LOWERCASE_TYPES  // prevents redef warning
      #define RAD_NO_LOWERCASE_TYPES
    #endif

    #ifndef RAD_NO_LOWERCASE_TYPES

      #ifdef __RADNGC__
        // Unfortunately dolphin\types.h typedefs the
        // same types that we use.
        // So we use the typedefs for this platform.

        #include <dolphin\types.h>

      #elif defined(__RADWII__)

        // Unfortunately revolution\types.h typedefs the
        // same types that we use.
        // So we use the typedefs for this platform.

        #include <revolution\types.h>

      #elif defined(__RADNDS__)

        // Ditto for Nitro/NDS
        #include <nitro\types.h>

      #else

        #define u8  U8
        #define s8  S8
        #define u16 U16
        #define s16 S16
        #define u32 U32
        #define s32 S32
        #define u64 U64
        #define s64 S64
        #define f32 F32
        #define f64 F64
 
      #endif

    #endif

    /// Some error-checking.
    #if defined(__RAD64__) && !defined(__RAD32__)
      // See top of file for why this is.
      #error __RAD64__ must not be defined without __RAD32__ (see radbase.h)
    #endif

    // This warning is completely useless.  Turn it off in all files...
    #ifdef _MSC_VER
      #pragma warning( disable : 4514) // unreferenced inline function removed.
    #endif

  #endif

  // Must be placed after variable declarations for code compiled as .c
  #define RAD_UNUSED_VARIABLE(x) (void)(sizeof(x))

#endif
