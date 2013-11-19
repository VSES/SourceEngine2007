//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: An example Maya locator node (definition)
//
//=============================================================================

// Maya includes

#include <maya/MPlug.h>

// Valve Includes

#include "VsMayaMPxFactory.h"

class CVstInfoTransform : public CVsMayaMPxTransform
{
public:
	static MStatus Initialize() { return MS::kSuccess; }

};

// Make node visible in Maya

INSTALL_MAYA_MPXTRANSFORM(
	CVstInfoTransform,
	vstInfo,
	0x0010e4c2,
	CVstInfoTransform::Initialize,
	"A transform node which is easily identifiable in a Maya scene.  Valve models need this as their root node."
);