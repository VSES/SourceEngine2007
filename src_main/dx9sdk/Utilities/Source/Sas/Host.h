//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#ifndef _HOST_H_
#define _HOST_H_
#include "sas.h"
//#include <stdio.h>
//#include <stdarg.h>

namespace Sas
{
	class Integrator;
	class BoundValueTable;

    enum MessageLevel
    {
        LEVEL_NOMESSAGES,
        LEVEL_FATALERROR,
        LEVEL_INTERNALERROR,
        LEVEL_ERROR,
        LEVEL_WARNING,
        LEVEL_STATUS,
        LEVEL_VERBOSE,
        LEVEL_DEBUG,
        LEVEL_MAX
    };

    class IMessageListener
    {
    public:
        virtual void OnSasMessage( MessageLevel level, const WCHAR* message ) = 0;
    };

	// Static class
	class Host
	{
	public:
		static Integrator* GetIntegrator() { return ((s_Integrator == NULL) ? &s_DefaultIntegrator : s_Integrator); }
		static void SetIntegrator(Integrator* i){ s_Integrator = i; }

        static BoundValueTable* GetRootTable();

        static void SetMessageListener( IMessageListener* pMessageListener ) { s_pMessageListener = pMessageListener; }

        static const WCHAR* MessageLevelToString( MessageLevel l )
        {
            static const WCHAR* MessageLevelStrings[] =
            {
                L"No Messages",
                L"Fatal Error",
                L"Internal Error",
                L"Error",
                L"Warning",
                L"Status",
                L"Verbose",
                L"Debug"
            };

            return MessageLevelStrings[l];
        }

        static void Out( MessageLevel level, const WCHAR* formatting, ... )
        {
            HRESULT hr;

            if( NULL == s_pMessageListener ||
                level > s_pMessageLevelBar )
                return;

            // Peform formatting
            static WCHAR message[SAS_STRINGSIZE] = {0};

            va_list argList;
            va_start(argList, formatting);
            hr = StringCchVPrintfW(message, SAS_STRINGSIZE, formatting, argList);
            va_end(argList);

            s_pMessageListener->OnSasMessage( level, message );
        }

        static void OutAnsi( MessageLevel level, const char* formatting, ... )
        {
            HRESULT hr;

            if( NULL == s_pMessageListener ||
                level > s_pMessageLevelBar )
                return;

            // Peform formatting
            static char message[SAS_STRINGSIZE] = {0};

            va_list argList;
            va_start(argList, formatting);
            hr = StringCchVPrintfA(message, SAS_STRINGSIZE, formatting, argList);
            va_end(argList);

            static WCHAR wstr[SAS_STRINGSIZE] = {0};
            MultiByteToWideChar( CP_ACP, 0, message, -1, wstr, SAS_STRINGSIZE );
    
            s_pMessageListener->OnSasMessage( level, wstr );
        }

        static void OutputVersionString()
        {
            Out( LEVEL_STATUS, L"Microsoft(R) Standard Annotations and Semantics version 1.1.0" );
        }


	private:
        Host();

        static IMessageListener* s_pMessageListener;
        static MessageLevel s_pMessageLevelBar;

        static Integrator* s_Integrator;
        //static IDirect3DDevice9* s_pDevice;
        //static IResourceCache* s_pResourceCache;
        
		// The 'Defaults' BoundValueTable contains the Sas-spec default values for values
		// which are not found. It is therefore the parent of the root BoundValueTable;
		static BoundValueTable s_DefaultsTable;
		static BoundValueTable s_RootTable;
		static Integrator s_DefaultIntegrator;
        //static DummyResourceCache s_DummyResourceCache;
	};
}

#endif //_HOST_H_
