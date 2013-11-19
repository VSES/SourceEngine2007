#include "pch.h"





//-----------------------------------------------------------------------------
// Name: initializePlugin()
// Desc: Initializes the Maya XFile Exporter plugin
//-----------------------------------------------------------------------------

MStatus	
initializePlugin(MObject obj)
{
	MStatus stat = MS::kSuccess;

	
	
	g_PreviewPipeline.Create(obj);

	
	

	return stat;
};


//-----------------------------------------------------------------------------
// Name: uninitializePlugin()
// Desc: Uninitializes the Maya XFile Exporter plugin
//-----------------------------------------------------------------------------


MStatus	
uninitializePlugin(MObject obj) 
{
	MStatus stat = MS::kSuccess;
	try
	{
    	g_PreviewPipeline.Destroy(obj);

	}
	catch( ... )
	{
	}

	return stat;
}

