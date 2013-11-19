//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifdef _LINUX
#include "cpu_linux.cpp"
#endif

#include "pch_tier0.h"

#if defined(_WIN32) && !defined(_X360)
#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#elif defined(_LINUX)
#include <stdio.h>
#endif

static bool cpuid(unsigned long function, unsigned long& out_eax, unsigned long& out_ebx, unsigned long& out_ecx, unsigned long& out_edx)
{
#ifdef _LINUX
	return false;
#elif defined( _X360 )
	return false;
#else
	bool retval = true;
	unsigned long local_eax, local_ebx, local_ecx, local_edx;
	_asm pushad;

	__try
	{
        _asm
		{
			xor edx, edx		// Clue the compiler that EDX is about to be used.
            mov eax, function   // set up CPUID to return processor version and features
								//      0 = vendor string, 1 = version info, 2 = cache info
            cpuid				// code bytes = 0fh,  0a2h
            mov local_eax, eax	// features returned in eax
            mov local_ebx, ebx	// features returned in ebx
            mov local_ecx, ecx	// features returned in ecx
            mov local_edx, edx	// features returned in edx
		}
    } 
	__except(EXCEPTION_EXECUTE_HANDLER) 
	{ 
		retval = false; 
	}

	out_eax = local_eax;
	out_ebx = local_ebx;
	out_ecx = local_ecx;
	out_edx = local_edx;

	_asm popad

	return retval;
#endif
}

bool CheckMMXTechnology(void)
{
#ifdef _LINUX
	return true;
#else
    unsigned long eax,ebx,edx,unused;
    if ( !cpuid(1,eax,ebx,unused,edx) )
		return false;

    return ( edx & 0x800000 ) != 0;
#endif
}

//-----------------------------------------------------------------------------
// Purpose: This is a bit of a hack because it appears 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
static bool IsWin98OrOlder()
{
#if defined(_WIN32) && !defined( _X360 )
	bool retval = false;

	OSVERSIONINFOEX osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	
	BOOL bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi);
	if( !bOsVersionInfoEx )
	{
		// If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
		
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if ( !GetVersionEx ( (OSVERSIONINFO *) &osvi) )
		{
			Error( _T("IsWin98OrOlder:  Unable to get OS version information") );
		}
	}

	switch (osvi.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_NT:
		// NT, XP, Win2K, etc. all OK for SSE
		break;
	case VER_PLATFORM_WIN32_WINDOWS:
		// Win95, 98, Me can't do SSE
		retval = true;
		break;
	case VER_PLATFORM_WIN32s:
		// Can't really run this way I don't think...
		retval = true;
		break;
	default:
		break;
	}

	return retval;
#else
	return false;
#endif
}


bool CheckSSETechnology(void)
{
#ifdef _LINUX
	return true;
#else
if ( IsWin98OrOlder() )
	{
		return false;
	}

    unsigned long eax,ebx,edx,unused;
    if ( !cpuid(1,eax,ebx,unused,edx) )
	{
		return false;
	}

    return ( edx & 0x2000000L ) != 0;
#endif
}

bool CheckSSE2Technology(void)
{
#ifdef _LINUX
    return false;
#else
	unsigned long eax,ebx,edx,unused;
    if ( !cpuid(1,eax,ebx,unused,edx) )
		return false;

    return ( edx & 0x04000000 ) != 0;
#endif
}

bool Check3DNowTechnology(void)
{
#ifdef _LINUX
    return false;
#else
	unsigned long eax, unused;
    if ( !cpuid(0x80000000,eax,unused,unused,unused) )
		return false;

    if ( eax > 0x80000000L )
    {
     	if ( !cpuid(0x80000001,unused,unused,unused,eax) )
			return false;

		return ( eax & 1<<31 ) != 0;
    }
    return false;
#endif
}

bool CheckCMOVTechnology()
{
#ifdef _LINUX
    return false;
#else
	unsigned long eax,ebx,edx,unused;
    if ( !cpuid(1,eax,ebx,unused,edx) )
		return false;

    return ( edx & (1<<15) ) != 0;
#endif
}

bool CheckFCMOVTechnology(void)
{
#ifdef _LINUX
    return false;
#else
    unsigned long eax,ebx,edx,unused;
    if ( !cpuid(1,eax,ebx,unused,edx) )
		return false;

    return ( edx & (1<<16) ) != 0;
#endif
}

bool CheckRDTSCTechnology(void)
{
#ifdef _LINUX
    return true;
#else
	unsigned long eax,ebx,edx,unused;
    if ( !cpuid(1,eax,ebx,unused,edx) )
		return false;

    return ( edx & 0x10 ) != 0;
#endif
}

// Return the Processor's vendor identification string, or "Generic_x86" if it doesn't exist on this CPU
const tchar* GetProcessorVendorId()
{
#ifdef _LINUX
	return "Generic_x86";
#else
	unsigned long unused, VendorIDRegisters[3];

	static tchar VendorID[13];
	
	memset( VendorID, 0, sizeof(VendorID) );
	if ( !cpuid(0,unused, VendorIDRegisters[0], VendorIDRegisters[2], VendorIDRegisters[1] ) )
	{
		if ( IsPC() )
		{
			_tcscpy( VendorID, _T( "Generic_x86" ) ); 
		}
		else if ( IsX360() )
		{
			_tcscpy( VendorID, _T( "PowerPC" ) ); 
		}
	}
	else
	{
		memcpy( VendorID+0, &(VendorIDRegisters[0]), sizeof( VendorIDRegisters[0] ) );
		memcpy( VendorID+4, &(VendorIDRegisters[1]), sizeof( VendorIDRegisters[1] ) );
		memcpy( VendorID+8, &(VendorIDRegisters[2]), sizeof( VendorIDRegisters[2] ) );
	}

	return VendorID;
#endif
}

// Returns non-zero if Hyper-Threading Technology is supported on the processors and zero if not.  This does not mean that 
// Hyper-Threading Technology is necessarily enabled.
static bool HTSupported(void)
{
#if ( defined( _X360 ) || defined(_LINUX) )
	// not entirtely sure about the semantic of HT support, it being an intel name
	// are we asking about HW threads or HT?
	return true;
#else
	const unsigned int HT_BIT		 = 0x10000000;  // EDX[28] - Bit 28 set indicates Hyper-Threading Technology is supported in hardware.
	const unsigned int FAMILY_ID     = 0x0f00;      // EAX[11:8] - Bit 11 thru 8 contains family processor id
	const unsigned int EXT_FAMILY_ID = 0x0f00000;	// EAX[23:20] - Bit 23 thru 20 contains extended family  processor id
	const unsigned int PENTIUM4_ID   = 0x0f00;		// Pentium 4 family processor id

	unsigned long unused,
				  reg_eax = 0, 
				  reg_edx = 0,
				  vendor_id[3] = {0, 0, 0};

	// verify cpuid instruction is supported
	if( !cpuid(0,unused, vendor_id[0],vendor_id[2],vendor_id[1]) 
	 || !cpuid(1,reg_eax,unused,unused,reg_edx) )
	 return false;

	//  Check to see if this is a Pentium 4 or later processor
	if (((reg_eax & FAMILY_ID) ==  PENTIUM4_ID) || (reg_eax & EXT_FAMILY_ID))
		if (vendor_id[0] == 'uneG' && vendor_id[1] == 'Ieni' && vendor_id[2] == 'letn')
			return (reg_edx & HT_BIT) != 0;	// Genuine Intel Processor with Hyper-Threading Technology

	return false;  // This is not a genuine Intel processor.
#endif
}

// Returns the number of logical processors per physical processors.
static uint8 LogicalProcessorsPerPackage(void)
{
#if defined( _X360 )
	return 2;
#else
	// EBX[23:16] indicate number of logical processors per package
	const unsigned NUM_LOGICAL_BITS = 0x00FF0000;

    unsigned long unused, reg_ebx = 0;

	if ( !HTSupported() ) 
		return 1; 

	if ( !cpuid(1,unused,reg_ebx,unused,unused) )
		return 1;

	return (uint8) ((reg_ebx & NUM_LOGICAL_BITS) >> 16);
#endif
}

// Measure the processor clock speed by sampling the cycle count, waiting
// for some fraction of a second, then measuring the elapsed number of cycles.
static int64 CalculateClockSpeed()
{
#if defined( _WIN32 )
#if !defined( _CERT )
	LARGE_INTEGER waitTime, startCount, curCount;
	CCycleCount start, end;

	// Take 1/32 of a second for the measurement.
	QueryPerformanceFrequency( &waitTime );
	int scale = 5;
	waitTime.QuadPart >>= scale;

	QueryPerformanceCounter( &startCount );
	start.Sample();
	do
	{
		QueryPerformanceCounter( &curCount );
	}
	while ( curCount.QuadPart - startCount.QuadPart < waitTime.QuadPart );
	end.Sample();

	return (end.m_Int64 - start.m_Int64) << scale;
#else
	return 3200000000LL;
#endif
#elif defined(_LINUX)
	uint64 CalculateCPUFreq(); // from cpu_linux.cpp
	int64 freq =(int64)CalculateCPUFreq();
	if ( freq == 0 ) // couldn't calculate clock speed
	{
		Error( "Unable to determine CPU Frequency\n" );
	}
	return freq;
#endif
}

const CPUInformation& GetCPUInformation()
{
	static CPUInformation pi;

	// Has the structure already been initialized and filled out?
	if ( pi.m_Size == sizeof(pi) )
		return pi;

	// Redundant, but just in case the user somehow messes with the size.
	memset(&pi, 0x0, sizeof(pi));

	// Fill out the structure, and return it:
	pi.m_Size = sizeof(pi);

	// Grab the processor frequency:
	pi.m_Speed = CalculateClockSpeed();
	
	// Get the logical and physical processor counts:
	pi.m_nLogicalProcessors = LogicalProcessorsPerPackage();

#if defined(_WIN32) && !defined( _X360 )
	SYSTEM_INFO si;
	ZeroMemory( &si, sizeof(si) );

	GetSystemInfo( &si );

	pi.m_nPhysicalProcessors = (unsigned char)(si.dwNumberOfProcessors / pi.m_nLogicalProcessors);
	pi.m_nLogicalProcessors = (unsigned char)(pi.m_nLogicalProcessors * pi.m_nPhysicalProcessors);

	// Make sure I always report at least one, when running WinXP with the /ONECPU switch, 
	// it likes to report 0 processors for some reason.
	if ( pi.m_nPhysicalProcessors == 0 && pi.m_nLogicalProcessors == 0 )
	{
		pi.m_nPhysicalProcessors = 1;
		pi.m_nLogicalProcessors  = 1;
	}
#elif defined( _X360 )
	pi.m_nPhysicalProcessors = 3;
	pi.m_nLogicalProcessors  = 6;
#elif defined(_LINUX)
	// TODO: poll /dev/cpuinfo when we have some benefits from multithreading
	pi.m_nPhysicalProcessors = 1;
	pi.m_nLogicalProcessors  = 1;
#endif

	// Determine Processor Features:
	pi.m_bRDTSC        = CheckRDTSCTechnology();
	pi.m_bCMOV         = CheckCMOVTechnology();
	pi.m_bFCMOV        = CheckFCMOVTechnology();
	pi.m_bMMX          = CheckMMXTechnology();
	pi.m_bSSE          = CheckSSETechnology();
	pi.m_bSSE2         = CheckSSE2Technology();
	pi.m_b3DNow        = Check3DNowTechnology();
	pi.m_szProcessorID = (tchar*)GetProcessorVendorId();
	pi.m_bHT		   = HTSupported();

	return pi;
}

