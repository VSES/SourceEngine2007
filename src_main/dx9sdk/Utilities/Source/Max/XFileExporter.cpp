
#include "StdAfx.h"

#include "MaxPipeline.h"
#include "PipelineLock.h"

#define XFILEEXPORTER_CLASS_ID Class_ID(0x2b903b29, 0x47e409cf)

BOOL CALLBACK XFileExporterOptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
HRESULT ExportScene(const TCHAR* name, bool showPrompts, bool exportSelected);

class XFileExporter : public SceneExport 
{
public:
    XFileExporter();
    ~XFileExporter();		

    int				ExtCount();					// Number of extensions supported
    const TCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")
    const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
    const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
    const TCHAR *	AuthorName();				// ASCII Author name
    const TCHAR *	CopyrightMessage();			// ASCII Copyright message
    const TCHAR *	OtherMessage1();			// Other message #1
    const TCHAR *	OtherMessage2();			// Other message #2
    unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
    void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box

    BOOL SupportsOptions(int ext, DWORD options);
    int	DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);
};


XFileExporter::XFileExporter()
{
}

XFileExporter::~XFileExporter() 
{
}

int XFileExporter::ExtCount()
{
	//TODO: Returns the number of file name extensions supported by the plug-in.
	return 1;
}

const TCHAR *XFileExporter::Ext(int n)
{		
	return _T("x");
}

const TCHAR *XFileExporter::LongDesc()
{
	return _T("Microsoft DirectX X-File");
}
const TCHAR *XFileExporter::ShortDesc() 
{			
	return _T("DirectX X-File");
}

const TCHAR *XFileExporter::AuthorName()
{			
	return _T("Microsoft Corporation");
}

const TCHAR *XFileExporter::CopyrightMessage() 
{	
	return _T("");
}

const TCHAR *XFileExporter::OtherMessage1() 
{		
	return _T("");
}

const TCHAR *XFileExporter::OtherMessage2() 
{		
	return _T("");
}

unsigned int XFileExporter::Version()
{				
	return 100;
}

void XFileExporter::ShowAbout(HWND hWnd)
{			
}

BOOL XFileExporter::SupportsOptions(int ext, DWORD options)
{
	return TRUE;
}


int	XFileExporter::DoExport(const TCHAR* name, ExpInterface *ei, Interface *i, BOOL suppressPrompts, DWORD options)
{
	bool showPrompts = suppressPrompts ? false : true;
	bool exportSelected = (options & SCENE_EXPORT_SELECTED) ? true : false;

	return SUCCEEDED(ExportScene(name, showPrompts, exportSelected));
}


DWORD WINAPI ProgressCb(LPVOID arg)
{
	// Dummy function for progress bar
	return(0);
}

HRESULT MassageOutputPath(const TCHAR* pathName, TCHAR* outPathName, int maxPathName)
{
	HRESULT hr = S_OK;

	if (SUCCEEDED(hr = StringCchCopy(outPathName, maxPathName, pathName)))
	{
		PathAddExtension(outPathName, ".x");
		PathMakePretty(outPathName);

		for (TCHAR* c=outPathName; *c; c++)
		{
			if (*c == '/')
			{
				*c = '\\';
			}
		}
	}

	return hr;
}

extern void Dpf(const char* fmt, ...);

void DumpFrameCounts(IDXCCFrame* f)
{
    UINT c = f->AddRef();
    c = f->Release();

    const char* name = "unknown";

    ComPtr<IDXCCResource> res;
    if (SUCCEEDED(ThePipeline.AccessManager()->FindResourceByPointer(f, 0, res)))
    {
        name = res->GetName();
    }

    Dpf("Frame %s: %d", name, c);

    for (UINT i=0; i<f->NumChildren(); i++)
    {
        ComPtr<IDXCCFrame> cf;
        f->GetChild(i, cf);

        DumpFrameCounts(cf);
    }
}

HRESULT ExportScene(const TCHAR* name, bool showPrompts, bool exportSelected)
{
	HRESULT hr = S_OK;

	Interface* ip = GetCOREInterface();

	TCHAR* msg = exportSelected?
						_T("Exporting selected via Microsoft Extensions for Max ...") :
						_T("Exporting via Microsoft Extensions for Max ...");

	ip->ProgressStart(msg, TRUE, ProgressCb, NULL);
	
	TCHAR OutPath[512];
	
	if (SUCCEEDED(hr = MassageOutputPath(name, OutPath, sizeof(OutPath)/sizeof(OutPath[0]))))
	{
		//TODO: if (!showPrompts || SUCCEEDED(hr = DoExportOptionsDlg()))
		{
			CPipelineLock lock;
			ThePipeline.SceneWriteLock(true, lock);

			try
			{
                ThePipeline.ResetData();
				ThePipeline.SyncData(false, exportSelected);

                DXCCSaveSceneArgs options =
                {
	                TRUE, //NoAddRefs;	
	                FALSE, //NoAnimation;	
	                FALSE, //NoFrameNames;		
	                FALSE, //NoMeshNames;		
	                FALSE, //NoMeshes;		
	                FALSE, //NoMeshAdjacencies;	
	                FALSE, //NoMeshSkins;	
	                FALSE, //NoMeshMaterials;	
	                FALSE, //ReplaceEffectPaths;
	                "", //ReplacementEffectPath[MAX_PATH];
	                FALSE, //ReplaceTexturePaths;
	                "", //ReplacementTexturePath[MAX_PATH];
	                FALSE, //RelativePaths;
	                "", //RelativeToPath[MAX_PATH];
                };

                DumpFrameCounts(ThePipeline.AccessRoot());

                hr = DXCCSaveScene(OutPath, D3DXF_FILEFORMAT_TEXT, ThePipeline.AccessManager(), ThePipeline.AccessRoot(), NULL, &options);

                DumpFrameCounts(ThePipeline.AccessRoot());

                if (FAILED(hr))
				{
					// print message
				}
			}
			catch (...)
			{
				ThePipeline.SceneWriteUnlock(lock);
				throw;
			}

			ThePipeline.SceneWriteUnlock(lock);
		}
	}

	ip->ProgressEnd();	

	return hr;

}


class XFileExporterClassDesc : public ClassDesc2 
{
public:

	int IsPublic() 
	{ 
		return TRUE; 
	}

	void* Create(BOOL loading = FALSE) 
	{ 
		return new XFileExporter(); 
	}

	const TCHAR* ClassName() 
	{ 
		return TEXT("XFileExporter"); 
	}

	SClass_ID SuperClassID() 
	{ 
		return SCENE_EXPORT_CLASS_ID; 
	}

	Class_ID ClassID() 
	{ 
		return XFILEEXPORTER_CLASS_ID; 
	}

	const TCHAR* Category() 
	{ 
		return TEXT("Exporters"); 
	}

	const TCHAR* InternalName() 
	{ 
		return _T("XFileExporter"); 
	}	

	HINSTANCE HInstance() 
	{ 
		extern HINSTANCE Instance;
		return Instance; 
	}	
};

ClassDesc* XFileExporter_GetClassDesc() 
{ 
	static XFileExporterClassDesc Desc;
	
	return &Desc; 
}
