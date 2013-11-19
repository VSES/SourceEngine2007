//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: An example Maya locator node (definition)
//
//=============================================================================

// Maya includes

#include <maya/MPlug.h>
#include <maya/MPxTransform.h>

// Valve Includes
#include "VsMayaMPxFactory.h"


//-----------------------------------------------------------------------------
// Example custom Maya transform node
//-----------------------------------------------------------------------------
class CVstExampleTransform : public CVsMayaMPxTransform
{
public:
	CVstExampleTransform();

	virtual ~CVstExampleTransform();

	static void *Factory();

	static MStatus Initialize();

public:
	static const MTypeId s_id;
	static const char *const s_name;

protected:
};


INSTALL_MAYA_MPXTRANSFORM(
	CVstExampleTransform,
	vstExampleTransform,
	0x0010e4c6,
	CVstExampleTransform::Initialize,
	"An example custom Maya transform node" );


//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CVstExampleTransform::CVstExampleTransform()
{
}


//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CVstExampleTransform::~CVstExampleTransform()
{
}


//-----------------------------------------------------------------------------
// Purpose: Initialize the node, add static attributes, etc...
// Output : MStatus::kSuccess if everything was ok
//-----------------------------------------------------------------------------
MStatus CVstExampleTransform::Initialize()
{
	return MS::kSuccess;
}