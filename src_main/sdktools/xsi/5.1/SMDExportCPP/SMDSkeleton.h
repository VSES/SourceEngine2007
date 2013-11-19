// SMDSkeleton.h: interface for the SMDSkeleton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMDSKELETON_H__C6237C41_4B7D_4405_9B70_14157A4CB651__INCLUDED_)
#define AFX_SMDSKELETON_H__C6237C41_4B7D_4405_9B70_14157A4CB651__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SIBCUtil.h"
#include "SIBCVector.h"
#include "SIBCArray.h"

/** 
* SMDKey struct
*
* This class holds the information for a single animation key
*
*/
typedef struct _SMDKey 
{
	SI_Int			m_iFrame;		//* The key's time (in frame)
	SI_Int			m_iBoneID;		//* The Node ID
	CSIBCVector3D	m_vPosition;	//* The Node's position
	CSIBCVector3D	m_vRotation;	//* The Node's rotation (euler angles)
} SMDKey;

/** 
* SMDSkeleton class
*
* This class holds a list of SMDKey structures
*
*/
class SMDSkeleton  
{
public:
	SMDSkeleton();
	virtual ~SMDSkeleton();

	/*!	Adds a single key to the list
	\param	int	Key's time (in frames)
	\param	int	Node ID
	\param	CSIBCVector3D	Node's position
	\param	CSIBCVector3D	Node's rotation
	\return	nothing
	*/
	void	AddKey ( int in_iFrame, 
						int in_szBoneId,
						CSIBCVector3D in_vPosition,
						CSIBCVector3D in_vRotation );

	/*!	Writes the list SMDKey to a file
	\param	FILE*	File pointer
	\return	nothing
	*/
	void	Write ( FILE* l_fptr, float in_fStart );

	/*!	Sets the SMDNodeList object that contains the node being referenced 
	\param	SMDNodeList*	A pointer to a SMDNodeList object
	\return	nothing
	*/
	void	SetReferenceList ( SMDNodeList* in_pList ) { m_pList = in_pList; };

	/*!	By definition a ModelSMD file is a file with a single bone located at the 
		center of the world to which every vertex is assigned with no animation data. 
		When called, this  function will erase it's key list and remplace it with 
		a single key at frame 0, position and rotation 0.
	\return nothing
	*/
	void	ConvertToModelSMD ();

	void	CreateDummyKeys ( int in_iNbKeys );
	
private:

	void	Sort ( float in_fBase  );

	CSIBCArray<SMDKey*>	m_lKeyList;

	SMDNodeList*		m_pList;
};

#endif // !defined(AFX_SMDSKELETON_H__C6237C41_4B7D_4405_9B70_14157A4CB651__INCLUDED_)
