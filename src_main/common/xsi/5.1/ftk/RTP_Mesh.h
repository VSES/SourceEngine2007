/****************************************************************************************
THIS CODE IS PUBLISHED AS A SAMPLE ONLY AND IS PROVIDED "AS IS".
IN NO EVENT SHALL SOFTIMAGE, AVID TECHNOLOGY, INC. AND/OR THEIR RESPECTIVE 
SUPPLIERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS CODE . 
 
COPYRIGHT NOTICE. Copyright © 1999-2002 Avid Technology Inc. . All rights reserved. 

SOFTIMAGE is a registered trademark of Avid Technology Inc. or its subsidiaries 
or divisions. Windows NT is a registered trademark of Microsoft Corp. All other
trademarks contained herein are the property of their respective owners. 
****************************************************************************************/

#ifndef _RTP_MESH_H_
#define _RTP_MESH_H_

#include <SIRTProcess.h>
#include <SIBaseApplication.h>
#include <Scene.h>
#include <TriangleList.h>
#include <TriangleStrip.h>
#include <PolygonList.h>
#include <BaseShape.h>
#include <Material.h>
#include <Primitive.h>

#include <RTP_BasicProcessesTypes.h>

/** 
* Mesh runtime process
* 
* When evaluated, the mesh will loop thru all it's list of CSIMaterialCluster and will update the associated CSIRTP_Material.
*
*/
SI_Error	RegisterSIMesh ( CSIBaseApplication* in_pApplication, CSLScene *in_pCurrentScene );

class XSIEXPORT CSIRTP_Mesh : public CSIRTProcess
{
public:

	//! Constructor for the CSIRTP_Mesh class
	/*! 
	\return a new pointer to a CSIRTP_Mesh
	*/
	CSIRTP_Mesh();

	//! Destructor for the CSIRTP_Mesh class
	/*! 
	\return Error code
	*/
	~CSIRTP_Mesh();

	static	SLTemplateCallback	m_sNativeCallbacks;

	//! Evaluates this mesh
	/*! During this call, the mesh will send all its triangle lists or polygon lists
	/*! To their CSIRTP_Material they reference, causing them to be rendered during the Material pass
	\return Error code
	*/

	virtual SI_Error			Evaluate();
	
	//! Initialize this mesh from a CSLPrimitive object
	/*! When this is called, the CSIRTP_Mesh will create lists of CSIMaterialCluster for each
	/*! triangle list, polygon list or tri-strip list contained in the CSLPrimitive.
	/*! This primitive's bounding volume is also calculated here
	\return Error code
	*/
	SI_Error					Initialize ( CSLPrimitive * );

	/*! 
	\return the classification of the runtime process
	*/
	virtual CSIRTProcessType	GetType() {return CSIRTProcess::RENDER;}

	SI_Error					GetMaterialProcessList ( CSIBCArray<CSIRTProcess*>& in_pProcessList );


	//! class factory for runtime linked runtime processes
	/*! This is used when an external entity wants to create a process of this 
	/*! type without having to link at compile time. The mechanism is simple.
	/*! the external entity uses the class factory to create a new CSIRTProcess
	/*! and sets the initialization parameters in the user data "SIRTINIT".
	/*! The external entity then calls CSIRTProcess::Init to initialize the data
	/*! extracted from the user data
	/*!
	\return a new CSIRTProcess
	*/
	virtual SI_Error			CreateRTProcess( CSIRTProcess**out_pProcess );

private:

	//! Converts a CSLTriangleList to a format compatible with the realtime shader architecture
	/*! (see CSIMaterialCluster)
	\param CSLBaseShape*	The shape containing the vertex, normnal UV and color information
	\param	CSLTriangleList* The triangle list to convert
	\param	CSIRTP_BoundingVolume*	A pointer to a bounding volume structure
	\return Error code
	*/
	SI_Error	CompileTriangleList ( CSLBaseShape *in_pShape, CSLTriangleList *in_pTriList, CSIRTP_BoundingVolume* in_pBV );

	//! Converts a CSLTriangleStripList to a format compatible with the realtime shader architecture
	/*! (see CSIMaterialCluster)
	\param CSLBaseShape*	The shape containing the vertex, normnal UV and color information
	\param	CSLTriangleStripList* The triangle strip list to convert
	\param	CSIRTP_BoundingVolume*	A pointer to a bounding volume structure
	\param	CSLBaseMaterial*	A pointer to the material assigned to the strip
	\return Error code
	*/	
	SI_Error	CompileTriangleStripList ( CSLBaseShape *in_pShape, 
											CSLTriangleStrip *in_pTriStrip, 
											CSIRTP_BoundingVolume* in_pBV,
											CSLBaseMaterial* in_pMaterial );


	//! Converts a CSLPolygonList to a format compatible with the realtime shader architecture
	/*! (see CSIMaterialCluster)
	\param CSLBaseShape*	The shape containing the vertex, normnal UV and color information
	\param	CSLPolygonList* The polygon list to convert
	\param	CSIRTP_BoundingVolume*	A pointer to a bounding volume structure
	\return Error code
	*/
	SI_Error	CompilePolygonList	( CSLBaseShape *in_pShape, CSLPolygonList *in_pPolygonList, CSIRTP_BoundingVolume* in_pBV );
	
	CSIBCArray<CSIMaterialCluster>	m_aLists;

	SI_Char*	m_pStripData;
	SI_Long		m_lDataSize;
	SI_Int		m_iClusterCount;
	SI_Void		*m_pUnused;
	SI_Int		m_iUserDataIndex;


};


#endif
