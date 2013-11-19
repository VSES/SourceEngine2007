//File resolver
//texture loader
//effect loader
//parameter management
#pragma once
#include "sas.h"

#pragma warning(disable: 4995)
#include <string>
#pragma warning(default: 4995)
#include "windows.h"

namespace Sas
{
	class Integrator
	{
	public:
		virtual bool FindFile(const WCHAR* find, const WCHAR* alternateDirectory, SourceInfo& found );
	};

}