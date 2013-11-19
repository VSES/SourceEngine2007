

#include <windows.h>
#include <shlwapi.h>
#include <process.h>

#include <vector>
#include <map>
#include <string>
#include <algorithm>

#pragma warning(push)
#pragma warning(disable : 4242) // conversion of int to BYTE
#include "max.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include "decomp.h"
#include "notify.h"
#include "guplib.h"
#include "IDxMaterial.h"

#include "maxscrpt/maxscrpt.h"
#include "maxscrpt/3dmath.h"
#include "maxscrpt/numbers.h"
#include "maxscrpt/strings.h"
#include "maxscrpt/name.h"

#include "IGame\IGame.h"
#include "IGame\IGameObject.h"
#include "IGame\IGameProperty.h"
#include "IGame\IGameControl.h"
#include "IGame\IGameModifier.h"
#include "IGame\IConversionManager.h"
#include "IGame\IGameError.h"
#pragma warning(pop)

#include "strsafe.h"

//#include "dxstdafx.h"

#include "dxcc.h"

#include "comptr.h"

typedef std::basic_string<_TCHAR, std::char_traits<_TCHAR>, std::allocator<_TCHAR> > tstring;
