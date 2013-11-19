
#include "StdAfx.h"
#include "MAXScrpt/definsfn.h"


def_struct_primitive( DxeTestIsLoaded, dxetest, "IsLoaded" );
Value* DxeTestIsLoaded_cf(Value** argv, int argc)
{
	Interface* ip = GetCOREInterface();
	assert(ip);

	DllDir dir = ip->GetDllDir();

	for (int i=0; i<dir.Count(); i++)
	{
		DllDesc& dll = dir[i];

		if (_tcsicmp(dll.fname, _T("DxExtensionsMax.dle")) == 0)
		{
			mputs("DxeTestIsLoaded: found DxExtensionsMax.dle");

			if (dll.Description() == NULL)
			{
				mputs("DxeTestIsLoaded: DxExtensionsMax dll has no description.");
			}

			if (dll.NumberOfClasses() != 1)
			{
				mputs("DxeTestIsLoaded: Wrong number of classes in DxExtensionsMax dll.");
			}

			ClassDesc* xfx = dll[0];

			if (_tcsicmp(xfx->InternalName(), _T("XFileExporter")) != 0)
			{
				mputs("DxeTestIsLoaded: XFileExporter not first class in DxExtensionsMax dll.");
			}
			
			break;
		}
	}

	return &true_value;
}

