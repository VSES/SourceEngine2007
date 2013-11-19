// SMDImporter.cpp: implementation of the SMDImporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SMDImporter.h"
#include <xsi_math.h>

extern char ___gTexturePathOverride[MAX_PATH];
extern void	XSILogMessage ( wchar_t *in_szMessage, XSI::siSeverityType in_eSeverity );
XSI::Null DoesObjectExist ( XSI::X3DObject in_pModel, XSI::CString in_szName );

char g_name[1024];
char*	FixName ( char* in_szname );
void fixNames ( char* buffer );

typedef struct _tagTriCluster
{
	char					m_szName[1024];
	XSI::CLongArray			m_indices;
} TriCluster;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SMDImporter::SMDImporter()
{
	m_pMeshNode = NULL;
}

SMDImporter::~SMDImporter()
{
	for (int c=0;c<m_pNodes.GetUsed();c++)
	{
		delete m_pNodes[c];
	}

}

bool SMDImporter::Import ( char *in_szFilename )
{

	FILE *f = fopen ( in_szFilename, "rb" );

	if ( !f )
	{
		LPWSTR l_wszModelName;
		DSA2W(&l_wszModelName,in_szFilename);

		XSI::CString cstr = L"SMDImport: Cannot open the file ";
		cstr += l_wszModelName;

		XSILogMessage ( (unsigned short*)cstr.GetWideString(), XSI::siErrorMsg );
		return false;
	}
	
	memset ( m_szActionName, 0, 1024 );
	_splitpath ( in_szFilename, NULL, NULL, m_szActionName, NULL );

	char drive[1024];
	char dirc[1024];

	memset ( m_szDirectory, 0, 1024 );

	if ( strlen(___gTexturePathOverride))
	{
		sprintf ( ___gTexturePathOverride, "%s\\", ___gTexturePathOverride );
		_splitpath ( ___gTexturePathOverride, drive, dirc, NULL, NULL );
		sprintf ( m_szDirectory, "%s\\%s\\", drive, dirc );
	} else {
		_splitpath ( in_szFilename, drive, dirc, NULL, NULL );
		sprintf ( m_szDirectory, "%s\\%s\\", drive, dirc );
	}

	
	bool Success = Parse ( f );

	fclose (f);

	if ( !Success )
	{
		return Success;
	}

	DetectModel();

	CreateHierarchy();

	CreateGeometry();

	WeightGeometry();

	CreateShapeClips();

	CreateAnimationSource();

	return true;
}

bool	SMDImporter::Parse ( FILE *f )
{
	char *buffer = NULL;

	try 
	{
		fseek(f, 0, SEEK_END );
		long fl = ftell ( f );
		fseek (f, 0,SEEK_SET);
		buffer = new char [fl + 7];
		fread (buffer,fl,sizeof(char),f);
		
		buffer[fl] = '\r';
		buffer[fl+1] = '\n';
		buffer[fl+2] = 'E';
		buffer[fl+3] = 'N';
		buffer[fl+4] = 'D';
		buffer[fl+5] = 0;
		
		fixNames ( buffer );

		char seps[] = " \t\n\r";
		char *token = GetNextToken (buffer, seps);
		int	smdversion = NextTokenAsInteger();

		if ( strcmp ( token, "version" ))
		{
			XSILogMessage ( L"SMDImport: Not an SMD file.", XSI::siErrorMsg );
			return false;
		}

		if ( smdversion != 1 )
		{
			XSILogMessage ( L"SMDImport: Attempting to load unsupported SMD version.", XSI::siWarningMsg );
		}

		while (1)
		{
			char *currenttok = GetNextToken (NULL, seps);

			if ( !strcmp ( currenttok, "nodes"))
			{
				if ( !ImportNodes () )
				{
					return false;
				}
			}

			if ( !strcmp ( currenttok, "skeleton"))
			{
				if ( !ImportSkeleton () )
				{
					return false;
				}
			}

			
			if ( !strcmp ( currenttok, "triangles"))
			{
				if ( !ImportTriangles () )
				{
					return false;
				}
			}

			if ( !strcmp ( currenttok, "vertexanimation"))
			{
				if ( !ImportVertexAnimation () )
				{
					return false;
				}
			}

			
			
			if (!strcmp ( currenttok, "END"))
			{
				break;	// were done
			}

		}

		//
		// Create Hierarchy
		//

		
	}
	catch (...)
	{
		XSILogMessage ( L"SMDImport: Invalid or corrupted SMD file.", XSI::siErrorMsg );
		
		return false;
	}

	delete [] buffer;

	return true;
}

bool SMDImporter::ImportNodes ()
{
	char seps[] = " \t\n\r";

	while (1)
	{
		char *token = GetNextToken (NULL, seps);
		if ( !strcmp ( token, "end" ) )
			return true;

		SMDNode* l_pNode = new SMDNode;
		m_pNodes.Extend(1);
		m_pNodes[m_pNodes.GetUsed()-1] = l_pNode;

		int nodeNum = atoi(token);
		token = GetNextToken (NULL, seps);

		l_pNode->m_iID = nodeNum;
		memset ( l_pNode->m_szName,0,sizeof(char)*MAX_PATH);
		memcpy ( l_pNode->m_szName, token+1, strlen(token)-2);

		token = GetNextToken (NULL, seps);
		int nodeParentNum = atoi(token);

		l_pNode->m_iParentID = nodeParentNum;

		// do parenting
		for (int c=0;c<m_pNodes.GetUsed();c++)
		{
			if ( m_pNodes[c]->m_iID == nodeParentNum )
			{
				l_pNode->m_pParent = m_pNodes[c];
				m_pNodes[c]->AddChild ( l_pNode );
			}
		}
	}

}


bool SMDImporter::ImportTriangles ()
{
	char seps[] = " \t\r\n";
	char sepswnl[] = " \t\r\10";

	bool nextIsTexture = false;

	char *token = NULL;
	while (1)
	{
		
		if (!nextIsTexture)
		{
			token = GetNextToken (NULL, seps);
		} else {
			token++;
		}

		if ( !strcmp ( token, "end" ) )
			return true;

		SMDTriangle* l_pTriangle = new SMDTriangle;
		m_pTriangles.Extend(1);
		
		m_pTriangles[m_pTriangles.GetUsed()-1] = l_pTriangle;
		memset ( l_pTriangle->m_szTexture, 0, MAX_PATH );
		strcpy ( l_pTriangle->m_szTexture, token );


		bool jumpNext = false;

		int rigid = 0;
		for (int v=0;v<3;v++)
		{
			
			if ( !jumpNext)
				rigid = atoi(GetNextToken (NULL, seps));

			l_pTriangle->m_pVertex.Extend(1);
			SMDVertex* l_pVert = &l_pTriangle->m_pVertex[l_pTriangle->m_pVertex.GetUsed()-1];
			l_pVert->m_vPosition.PutX ( atof(GetNextToken (NULL, seps)) );
			l_pVert->m_vPosition.PutY ( atof(GetNextToken (NULL, seps)) );
			l_pVert->m_vPosition.PutZ ( atof(GetNextToken (NULL, seps)) );
			l_pVert->m_vNormal.PutX ( atof(GetNextToken (NULL, seps)) );
			l_pVert->m_vNormal.PutY ( atof(GetNextToken (NULL, seps)) );
			l_pVert->m_vNormal.PutZ ( atof(GetNextToken (NULL, seps)) );
			l_pVert->m_vUV.PutX ( atof(GetNextToken (NULL, seps)) );
			l_pVert->m_vUV.PutY ( atof(GetNextToken (NULL, seps)) );

			token = GetNextToken (NULL, sepswnl);
			if ( isdigit( token[0] ) )
			{
				//
				// import weights
				//

				int numWeights = atoi ( token );

				for (int w=0;w<numWeights;w++)
				{
					l_pVert->m_vWeights.Extend(1);
					l_pVert->m_vWeights[l_pVert->m_vWeights.GetUsed()-1].m_iBoneID = atoi(GetNextToken (NULL, seps));
					l_pVert->m_vWeights[l_pVert->m_vWeights.GetUsed()-1].m_fWeight = atof(GetNextToken (NULL, seps));

				}
			
				jumpNext = false;
				nextIsTexture = false;

			} else {

				if ( rigid != 0 )
				{
					//
					// add rigid weight
					//

					l_pVert->m_vWeights.Extend(1);
					l_pVert->m_vWeights[l_pVert->m_vWeights.GetUsed()-1].m_iBoneID = rigid;
					l_pVert->m_vWeights[l_pVert->m_vWeights.GetUsed()-1].m_fWeight = 1.0f;

				}

				if ((token[0] == 10 ) && ( strlen(token) == 1 ))
				{
					token = GetNextToken (NULL, sepswnl);
					rigid = atoi(token);
				}
				jumpNext = true;
				
				if ( v == 2 )
					nextIsTexture = true;
			}


		}

	}

}

bool SMDImporter::ImportSkeleton ()
{
	char seps[] = " \t\n\r";

	int currentFrame = 0;

	while (1)
	{
		

		char *token = GetNextToken (NULL, seps);
		if ( !strcmp ( token, "end" ) )
			return true;

		if ( strcmp ( token, "time" ))
		{
		
			int nodeNum = atoi(token);
			float x = NextTokenAsFloat();
			float y = NextTokenAsFloat();
			float z = NextTokenAsFloat();
			float rx = NextTokenAsFloat();
			float ry = NextTokenAsFloat();
			float rz = NextTokenAsFloat();
			
			for (int c=0;c<m_pNodes.GetUsed();c++)
			{
				if ( m_pNodes[c]->m_iID == nodeNum )
				{
					SMDKey* l_pKey = new SMDKey;
					l_pKey->m_fTime = currentFrame;
					l_pKey->m_vPosition.PutX ( x );
					l_pKey->m_vPosition.PutY ( y );
					l_pKey->m_vPosition.PutZ ( z );

					l_pKey->m_vRotation.PutX ( rx );
					l_pKey->m_vRotation.PutY ( ry );
					l_pKey->m_vRotation.PutZ ( rz );

					m_pNodes[c]->AddKey ( l_pKey );
					break;
				}
			}
			
		} else {
			
			currentFrame = NextTokenAsInteger();
		}

	
		
	}

}

bool SMDImporter::ImportVertexAnimation ()
{
	char seps[] = " \t\n\r";

	VTAClip*	currentClip = NULL;

	m_pVTA.Extend(1);
	currentClip = &m_pVTA[0];

	while (1)
	{
		int currentFrame = 0;

		char *token = GetNextToken (NULL, seps);
		if ( !strcmp ( token, "end" ) )
			return true;

		if ( strcmp ( token, "time" ))
		{
		
			int vertexIndex = atoi(token);
			float x = NextTokenAsFloat();
			float y = NextTokenAsFloat();
			float z = NextTokenAsFloat();
			float nx = NextTokenAsFloat();
			float ny = NextTokenAsFloat();
			float nz = NextTokenAsFloat();

			currentClip->m_pVertices.Extend(1);
			VTAVertex* v = &currentClip->m_pVertices[currentClip->m_pVertices.GetUsed()-1];
			v->m_iIndex = vertexIndex;
			v->m_vPosition.PutX(x);
			v->m_vPosition.PutY(x);
			v->m_vPosition.PutZ(x);

			v->m_vNormal.PutX(x);
			v->m_vNormal.PutY(x);
			v->m_vNormal.PutZ(x);
			
		} else {
			
			currentFrame = NextTokenAsInteger();
			m_pVTA.Extend(1);
			currentClip = &m_pVTA[m_pVTA.GetUsed()-1];

		}

	
		
	}

}


int		SMDImporter::NextTokenAsInteger ()
{
	char seps[] = " \t\n\r";
	char *token = GetNextToken (NULL, seps);
	return atoi ( token );
}

float	SMDImporter::NextTokenAsFloat ()
{
	char seps[] = " \t\n\r";
	char *token = GetNextToken (NULL, seps);
	return atof ( token );
}

void	SMDImporter::DetectModel()
{
	XSI::Application app;

	m_pModel = app.GetActiveSceneRoot();

	for (int c=0;c<m_pNodes.GetUsed();c++)
	{
		if ( !m_pNodes[c]->m_pChildren.GetUsed() )
			continue;

		char name[MAX_PATH];
		memset ( name,0,MAX_PATH);
		strcpy ( name, m_pNodes[c]->m_szName );
		char * subname = strchr ( name, '.' );
		if ( subname )
		{
			subname[0] =0;
			subname++;

			LPWSTR l_wszModelName;
			DSA2W(&l_wszModelName,name);

			XSI::CRefArray models = m_pModel.GetModels(false);
			for (int m=0;m<models.GetCount();m++)
			{
				XSI::Model mm = models[m];

				XSI::CString l_szName =l_wszModelName;

				if ( l_szName == mm.GetName() )
				{
					m_pModel = mm;
					return;

				}

			}

			//
			// Create the model
			//

			XSI::Model rootModel;
			XSI::CRefArray nullList;
			

			m_pModel.AddModel(nullList, l_wszModelName, rootModel);

			m_pModel = rootModel;

			m_pModel.AddMixer();
			
			break;

		}
	}

}

void	SMDImporter::CreateHierarchy()
{
	XSI::X3DObject currentParent = m_pModel;

	for (int c=0;c<m_pNodes.GetUsed();c++)
	{
		SMDNode* l_pNode = m_pNodes[c];

		if ( l_pNode->m_iParentID != -1 )
			continue;

		if ( !l_pNode->m_pChildren.GetUsed() )
		{
			if ( l_pNode->m_iParentID == -1 )
			{
				//
				// this is a mesh object so dont do anything
				//
				m_pMeshNode = l_pNode;
				continue;
			}

		}

		if ( !l_pNode->m_pChildren.GetUsed() )
		{
			//
			// create a NULL
			//

			LPWSTR l_wszModelName;
			DSA2W(&l_wszModelName,l_pNode->m_szName);

			XSI::Null myNull = DoesObjectExist ( currentParent, XSI::CString(l_wszModelName) );

			if ( !myNull.IsValid() )
			{
				XSI::Null myNull;
				currentParent.AddNull ( l_wszModelName, myNull );
			}
			
			l_pNode->m_x3d = myNull;
			
		}

		if ( l_pNode->m_pChildren.GetUsed() )
		{
			RecurseCreateNull ( currentParent, l_pNode );

		}

	}


}

void SMDImporter::RecurseCreateNull ( XSI::X3DObject in_pParent, SMDNode* in_pNode )
{
	
	LPWSTR l_wszModelName;
	DSA2W(&l_wszModelName,FixName ( in_pNode->m_szName ) );
				
	XSI::Null myNull = DoesObjectExist ( in_pParent, XSI::CString(l_wszModelName) );

	if ( !myNull.IsValid() )
	{
		in_pParent.AddNull ( l_wszModelName, myNull );
	}

	in_pNode->m_x3d = myNull;

	XSI::KinematicState l_lLocal = myNull.GetKinematics().GetLocal();

	XSI::MATH::CTransformation xfo;

	if ( in_pNode->m_pKeys.GetUsed() )
	{

		SMDKey* l_pKey = in_pNode->GetKey (0);
		
		xfo = l_lLocal.GetTransform();
		xfo.SetRotationFromXYZAnglesValues ( l_pKey->m_vRotation.GetX(), l_pKey->m_vRotation.GetY(), l_pKey->m_vRotation.GetZ() );
		xfo.SetTranslationFromValues ( l_pKey->m_vPosition.GetX(), l_pKey->m_vPosition.GetY(), l_pKey->m_vPosition.GetZ() );
	}

	if ( in_pNode->m_pParent ==NULL )
	{
		XSI::MATH::CTransformation xfo2;
		xfo2.SetRotationFromXYZAnglesValues ( -1.570796, 0.0, 0.0 );
		xfo.MulInPlace(xfo2);
	}

	l_lLocal.PutTransform ( xfo );

	for (int c=0;c<in_pNode->m_pChildren.GetUsed();c++)
	{
		RecurseCreateNull ( myNull, in_pNode->GetChild(c));
	}

}

void	SMDImporter::CreateChainFromThisNode( SMDNode* in_pNode, XSI::X3DObject in_Parent, int from)
{
	
	XSI::ChainRoot myChainRoot;
	SMDKey *l_pKey = in_pNode->GetKey (0);

	for (int c=from;c<in_pNode->m_pChildren.GetUsed();c++)
	{

		XSI::MATH::CVector3 position;
		XSI::MATH::CVector3 positioneff;
		if ( l_pKey )
		{
			position = GetGlobalPosition ( in_pNode );
			positioneff = position;
			positioneff.PutX ( positioneff.GetX() + 1.0f );
		}

		if (!myChainRoot.IsValid() )
		{
			LPWSTR l_wszModelName;
			DSA2W(&l_wszModelName,in_pNode->m_szName);
			
			in_Parent.Add3DChain(	position,
				positioneff,
				XSI::MATH::CVector3(0,0,1),
				l_wszModelName,
				myChainRoot);
		}

		SMDNode* bone = in_pNode->m_pChildren[c];

		while ( bone != NULL )
		{
			LPWSTR l_wszBoneName;
			DSA2W(&l_wszBoneName,bone->m_szName);

			SMDKey *l_pBKey = bone->GetKey (0);
			XSI::ChainBone myChainBone1;
			myChainRoot.AddBone(GetGlobalPosition(bone),XSI::siChainBonePin,l_wszBoneName,myChainBone1);

			if ( !bone->m_pChildren.GetUsed() )
			{
				bone = NULL;
			} else {

				if ( bone->m_pChildren.GetUsed() > 1 )
				{
					for (int s=1;s<bone->m_pChildren.GetUsed();s++)
					{
						CreateChainFromThisNode( bone, myChainBone1, s);
					}
				}

				bone = bone->GetChild(0);
			}
		}
	}

}

XSI::MATH::CVector3 SMDImporter::GetGlobalPosition ( SMDNode* in_pNode )
{
	XSI::MATH::CVector3 ret;

	ret = in_pNode->GetKey(0)->m_vPosition;

	SMDNode* parent = in_pNode->m_pParent;

	while ( parent != NULL )
	{
		ret += parent->GetKey(0)->m_vPosition;
		parent = parent->m_pParent;
	}
	return ret;
}

char*	FixName ( char* in_szname )
{
	memset ( g_name, 0, 1024 );
	strcpy ( g_name, in_szname );

	char * subname = strchr ( g_name, '.' );

	if ( subname )
	{
		subname++;
		return subname;
	}

	return in_szname;
}

void SMDImporter::CreateAnimationSource()
{
	XSI::ActionSource actionSource;

	float animStart = 9999;
	float animEnd = -9999;

	XSI::CString animatedObjects;

	for (int c=0;c<m_pNodes.GetUsed();c++)
	{
		SMDNode* node = m_pNodes[c];

		if ( node->m_pKeys.GetUsed() > 1 )
		{
			if ( !actionSource.IsValid() )
			{
				LPWSTR l_wszActionName;
				DSA2W(&l_wszActionName,m_szActionName);

				actionSource = m_pModel.AddActionSource( l_wszActionName );
			}

			XSI::Parameter x = node->m_x3d.GetParameters().GetItem( L"posx" );
			XSI::Parameter y = node->m_x3d.GetParameters().GetItem( L"posy" );
			XSI::Parameter z = node->m_x3d.GetParameters().GetItem( L"posz" );

			XSI::Parameter rx = node->m_x3d.GetParameters().GetItem( L"rotx" );
			XSI::Parameter ry = node->m_x3d.GetParameters().GetItem( L"roty" );
			XSI::Parameter rz = node->m_x3d.GetParameters().GetItem( L"rotz" );

			node->m_fOldX = x.GetValue();
			node->m_fOldY = y.GetValue();
			node->m_fOldZ = z.GetValue();
			node->m_fOldRX = rx.GetValue();
			node->m_fOldRY = ry.GetValue();
			node->m_fOldRZ = rz.GetValue();


			XSI::FCurve fcrvx;
			x.AddFCurve( XSI::siStandardFCurve, fcrvx	);
			XSI::FCurve fcrvy;
			y.AddFCurve( XSI::siStandardFCurve, fcrvy	);
			XSI::FCurve fcrvz;
			z.AddFCurve( XSI::siStandardFCurve, fcrvz	);
			XSI::FCurve fcrvrx;
			rx.AddFCurve( XSI::siStandardFCurve, fcrvrx	);
			XSI::FCurve fcrvry;
			ry.AddFCurve( XSI::siStandardFCurve, fcrvry	);
			XSI::FCurve fcrvrz;
			rz.AddFCurve( XSI::siStandardFCurve, fcrvrz	);
			
			XSI::CTimeArray time(node->m_pKeys.GetUsed());
			XSI::CValueArray xv(node->m_pKeys.GetUsed());
			XSI::CValueArray yv(node->m_pKeys.GetUsed());
			XSI::CValueArray zv(node->m_pKeys.GetUsed());
			
			XSI::CValueArray rxv(node->m_pKeys.GetUsed());
			XSI::CValueArray ryv(node->m_pKeys.GetUsed());
			XSI::CValueArray rzv(node->m_pKeys.GetUsed());
	
			if ( node->m_pParent ==NULL )
			{
				for (int k=0;k<node->m_pKeys.GetUsed();k++)
				{
				
					if ( node->GetKey(k)->m_fTime < animStart )
					{
						animStart = node->GetKey(k)->m_fTime;
					}

					if ( node->GetKey(k)->m_fTime > animEnd )
					{
						animEnd = node->GetKey(k)->m_fTime;
					}

					XSI::MATH::CTransformation xfo1;
					XSI::MATH::CTransformation xfo2;
					
					xfo1.SetRotationFromXYZAnglesValues ( node->GetKey(k)->m_vRotation.GetX(), node->GetKey(k)->m_vRotation.GetY(), node->GetKey(k)->m_vRotation.GetZ() );
					xfo1.SetTranslationFromValues ( node->GetKey(k)->m_vPosition.GetX(), node->GetKey(k)->m_vPosition.GetY(), node->GetKey(k)->m_vPosition.GetZ() );

					xfo2.SetRotationFromXYZAnglesValues ( -1.570796, 0.0, 0.0 );
					xfo1.MulInPlace(xfo2);

					double dx,dy,dz;
					double drx, dry, drz;

					xfo1.GetTranslationValues ( dx, dy, dz);
					xfo1.GetRotationFromXYZAnglesValues(drx, dry, drz);

					time[k] = k;
					xv[k] = dx;
					yv[k] = dy;
					zv[k] = dz;
					
					rxv[k] = drx * 57.29577951308232286465;
					ryv[k] = dry * 57.29577951308232286465;
					rzv[k] = drz * 57.29577951308232286465;

				}
				
			} else {
				
				for (int k=0;k<node->m_pKeys.GetUsed();k++)
				{
					
					if ( node->GetKey(k)->m_fTime < animStart )
					{
						animStart = node->GetKey(k)->m_fTime;
					}

					if ( node->GetKey(k)->m_fTime > animEnd )
					{
						animEnd = node->GetKey(k)->m_fTime;
					}

					time[k] = k;
					xv[k] = node->GetKey(k)->m_vPosition.GetX();
					yv[k] = node->GetKey(k)->m_vPosition.GetY();
					zv[k] = node->GetKey(k)->m_vPosition.GetZ();
					
					rxv[k] = node->GetKey(k)->m_vRotation.GetX() * 57.29577951308232286465;
					ryv[k] = node->GetKey(k)->m_vRotation.GetY() * 57.29577951308232286465;
					rzv[k] = node->GetKey(k)->m_vRotation.GetZ() * 57.29577951308232286465;
				}
			}

			fcrvx.SetKeys ( time, xv );
			fcrvy.SetKeys ( time, yv );
			fcrvz.SetKeys ( time, zv );

			fcrvrx.SetKeys ( time, rxv );
			fcrvry.SetKeys ( time, ryv );
			fcrvrz.SetKeys ( time, rzv );

			LPWSTR l_wszModelName;
			DSA2W(&l_wszModelName,FixName(node->m_szName));
			XSI::CString cname = l_wszModelName;

			actionSource.AddSourceItem ( cname + L".kine.local.posx", fcrvx, true ); 
			actionSource.AddSourceItem ( cname + L".kine.local.posy", fcrvy, true );
			actionSource.AddSourceItem ( cname + L".kine.local.posz", fcrvz, true );

			actionSource.AddSourceItem ( cname + L".kine.local.rotx", fcrvrx, true ); 
			actionSource.AddSourceItem ( cname + L".kine.local.roty", fcrvry, true );
			actionSource.AddSourceItem ( cname + L".kine.local.rotz", fcrvrz, true );
		
			// build up the string list of objects that we want to remove animation from
			if (animatedObjects.IsEmpty() == false) {
				animatedObjects += L", ";
			}
			animatedObjects += node->m_x3d.GetFullName();
		}
	}

	if ( actionSource.IsValid() )
	{
		actionSource.PutParameterValue(L"FrameStart", (double)animStart);
		actionSource.PutParameterValue(L"FrameEnd", (double)animEnd);
	}

	// remove animation on all objects that were imported
	// and animated
	if (animatedObjects.IsEmpty() == false) {
		XSI::Application app;
		XSI::CValue out;
		XSI::CValueArray args(4);

		args[0] = animatedObjects;
		args[1] = XSI::CValue();
		args[2] = (long)XSI::siBranch;
		args[3] = (long)(XSI::siFCurveSource);
		app.ExecuteCommand(L"RemoveAllAnimation", args, out);
	}
}


void	SMDImporter::CreateGeometry()
{

	if ( !m_pTriangles.GetUsed() )
		return;

	long t;

	XSI::MATH::CTransformation xfo;
	xfo.SetRotationFromXYZAnglesValues ( -1.570796, 0.0, 0.0 );

	for (t=0;t<m_pTriangles.GetUsed();t++)
	{
		for (int v=0;v<3;v++)
		{
			XSI::MATH::CVector3 vec = XSI::MATH::MapObjectPositionToWorldSpace ( xfo, m_pTriangles[t]->m_pVertex[v].m_vPosition );
			
			long outindex;
			compress.AddVertex ( vec.GetX(), 
									vec.GetY(), 
									vec.GetZ(), 
									m_pTriangles[t]->m_pVertex[v].m_vUV.GetX(), 
									m_pTriangles[t]->m_pVertex[v].m_vUV.GetY(),
									&m_pTriangles[t]->m_pVertex[v],
									&outindex);

			m_lVertexMap.Extend(1);
			m_lVertexMap[m_lVertexMap.GetUsed()-1] = outindex;
		}
	}

	XSI::MATH::CVector3Array verts(compress.GetCount());
	long vindex = 0;
	long cnt  = compress.GetCount ();
	for (t=0;t<compress.GetCount ();t++)
	{
		uvvec vec;
		compress.GetVertex (t, &vec);
		verts[t] = XSI::MATH::CVector3 ( vec.x, vec.y, vec.z );
	}
	
	
	XSI::CLongArray indices((m_pTriangles.GetUsed() * 3) + m_pTriangles.GetUsed());

	long iindex = 0;
	

	char *l_szGlobalTexture = m_pTriangles[0]->m_szTexture;
	CSIBCArray<TriCluster>	ClusterList;
	
	for (t=0;t<m_pTriangles.GetUsed();t++)
	{
		
		XSI::MATH::CVector3 vec1 = XSI::MATH::MapObjectPositionToWorldSpace ( xfo, m_pTriangles[t]->m_pVertex[0].m_vPosition );
		XSI::MATH::CVector3 vec2 = XSI::MATH::MapObjectPositionToWorldSpace ( xfo, m_pTriangles[t]->m_pVertex[1].m_vPosition );
		XSI::MATH::CVector3 vec3 = XSI::MATH::MapObjectPositionToWorldSpace ( xfo, m_pTriangles[t]->m_pVertex[2].m_vPosition );

		long i1 = compress.GetIndex ( vec1.GetX(), vec1.GetY(), vec1.GetZ(), m_pTriangles[t]->m_pVertex[0].m_vUV.GetX(), m_pTriangles[t]->m_pVertex[0].m_vUV.GetY());
		long i2 = compress.GetIndex ( vec2.GetX(), vec2.GetY(), vec2.GetZ(), m_pTriangles[t]->m_pVertex[1].m_vUV.GetX(), m_pTriangles[t]->m_pVertex[1].m_vUV.GetY());
		long i3 = compress.GetIndex ( vec3.GetX(), vec3.GetY(), vec3.GetZ(), m_pTriangles[t]->m_pVertex[2].m_vUV.GetX(), m_pTriangles[t]->m_pVertex[2].m_vUV.GetY());

		indices[iindex] = 3;
		indices[iindex+1] = i1;
		indices[iindex+2] = i2;
		indices[iindex+3] = i3;
		iindex += 4;

		if ( strcmp ( l_szGlobalTexture, m_pTriangles[t]->m_szTexture ))
		{
			//
			// found a local material
			//

			TriCluster* cls = NULL;

			for (int c=0;c<ClusterList.GetUsed();c++)
			{
				if ( !strcmp ( ClusterList[c].m_szName, m_pTriangles[t]->m_szTexture))
				{
					cls = &ClusterList[c];
					break;
				}

			}

			if ( cls == NULL )
			{
				ClusterList.Extend(1);
				strcpy ( ClusterList[ClusterList.GetUsed()-1].m_szName, m_pTriangles[t]->m_szTexture );
				cls = &ClusterList[ClusterList.GetUsed()-1];
			}

			cls->m_indices.Add ( t );

		}

	}

	char mname[1024];
	sprintf (mname, "mesh" );
	if ( m_pMeshNode )
	{
		sprintf (mname, FixName(m_pMeshNode->m_szName));
	}

	LPWSTR l_wszModelName;
	DSA2W(&l_wszModelName,mname);

	m_pModel.AddPolygonMesh ( verts, indices, l_wszModelName, m_pMesh );


	XSI::Application	app;
	XSI::CValueArray	args(4);
	XSI::CValue			outArg;
	XSI::CStatus		st;

	args[0] = XSI::CValue( XSI::CString(L"") );
	args[1] = XSI::CValue(false);
	args[0] = XSI::CValue(m_pMesh.GetRef());
	args[1] = XSI::CValue((long)XSI::siTxtUV);
	args[2] = XSI::CValue((long)XSI::siTxtDefaultSpherical);
	args[3] = XSI::CValue(XSI::CString(L"Texture_Support"));

	app.ExecuteCommand( L"CreateTextureSupport", args, outArg );

	XSI::CValueArray	moreargs(1);
	XSI::CValueArray	moreoutargs(3);
	moreargs[0] = m_pMesh.GetRef();
	app.ExecuteCommand(L"FreezeObj",moreargs, outArg);

	XSI::Material		l_matMaterial;
	st = m_pMesh.AddMaterial(L"Phong", true, L"CubeMat", l_matMaterial);
		
	XSI::OGLTexture		l_oglTexture(l_matMaterial.GetOGLTexture());

	XSI::CString		l_szFullNameDefaultOut	= l_oglTexture.GetFullName();
	int			l_nHeightDefaultOut		= l_oglTexture.GetHeight();
	int			l_nWidthDefaultOut		= l_oglTexture.GetWidth();

	

	// Now actually add a texture, so we can test it.
	args[0] = XSI::CValue( XSI::CString(L"Image") );
	args[1] = XSI::CValue(m_pMesh.GetRef());
	args[2] = XSI::CValue((short)1);
	args[3] = XSI::CValue(false);
	st = app.ExecuteCommand( L"BlendInPresets", args, outArg );

	
	//
	// create the texture and connect
	//
	
	XSI::CValueArray	clipargs(3);
	XSI::ImageClip2 l_pClip;
	
	char l_szTextureFullname[1024];
	sprintf ( l_szTextureFullname, "%s%s", m_szDirectory, m_pTriangles[0]->m_szTexture);
	char clipname[1024];
	_splitpath ( m_pTriangles[0]->m_szTexture, NULL, NULL, clipname, NULL );

	LPWSTR l_wszClipName;
	DSA2W(&l_wszClipName,l_szTextureFullname);

	LPWSTR l_wszClipName2;
	DSA2W(&l_wszClipName2,clipname);

	
	
	clipargs[0] = XSI::CValue( XSI::CString(l_wszClipName) );
	clipargs[1] = XSI::CValue( XSI::CString(l_wszClipName2) );
	clipargs[2] = XSI::CValue(l_pClip.GetRef());
	app.ExecuteCommand( L"SICreateImageClip", clipargs, outArg );
	
	XSI::CString		l_szMaterialName	= l_matMaterial.GetFullName(); 
	XSI::CString		l_szImageNode = l_szMaterialName + L".CubeMat.ambient_blend.Image.tex";
	XSI::CString		l_szFullclipname = L"Clips." +  XSI::CString(l_wszClipName2);
	
	XSI::CValueArray	clipargs2(2);
	clipargs2[0] = XSI::CValue( XSI::CString(l_szFullclipname) );
	clipargs2[1] = XSI::CValue( XSI::CString(l_szImageNode) );
	
	app.ExecuteCommand( L"SIConnectShaderToCnxPoint", clipargs2, outArg );

	//
	// Create all clusters
	//

	XSI::Geometry geom( m_pMesh.GetActivePrimitive().GetGeometry() );
	
	for (int b=0;b<ClusterList.GetUsed();b++)
	{
		TriCluster* cls = &ClusterList[b];
		
		
		sprintf ( l_szTextureFullname, "%s%s", m_szDirectory, cls->m_szName);
		_splitpath ( cls->m_szName, NULL, NULL, clipname, NULL );
		
		DSA2W(&l_wszClipName,l_szTextureFullname);
		DSA2W(&l_wszClipName2,clipname);
		
		XSI::CLongArray array;
		XSI::Cluster polyCluster ;
		geom.AddCluster( XSI::siPolygonCluster, l_wszClipName2, cls->m_indices, polyCluster ) ;
		
		st = polyCluster.AddMaterial(L"Phong", true, L"CubeMat", l_matMaterial);
		
		XSI::OGLTexture		l_oglTexture(l_matMaterial.GetOGLTexture());
	
		// Now actually add a texture, so we can test it.
		args[0] = XSI::CValue( XSI::CString(L"Image") );
		args[1] = XSI::CValue(polyCluster.GetRef());
		args[2] = XSI::CValue((short)1);
		args[3] = XSI::CValue(false);
		st = app.ExecuteCommand( L"BlendInPresets", args, outArg );
			

		
		
		
		clipargs[0] = XSI::CValue( XSI::CString(l_wszClipName) );
		clipargs[1] = XSI::CValue( XSI::CString(l_wszClipName2) );
		clipargs[2] = XSI::CValue(l_pClip.GetRef());
		app.ExecuteCommand( L"SICreateImageClip", clipargs, outArg );
		
		l_szMaterialName	= l_matMaterial.GetFullName(); 
		l_szImageNode = l_szMaterialName + L".CubeMat.ambient_blend.Image.tex";
		l_szFullclipname = L"Clips." +  XSI::CString(l_wszClipName2);
		
		clipargs2[0] = XSI::CValue( XSI::CString(l_szFullclipname) );
		clipargs2[1] = XSI::CValue( XSI::CString(l_szImageNode) );
		
		app.ExecuteCommand( L"SIConnectShaderToCnxPoint", clipargs2, outArg );
		
	}

	
	if ( m_pMesh.IsValid () )
	{
		XSI::Geometry geom( m_pMesh.GetActivePrimitive().GetGeometry() );
		XSI::PolygonMesh mesh(m_pMesh.GetActivePrimitive().GetGeometry());

		XSI::CPointRefArray Geompoints = geom.GetPoints();
		
				
		XSI::CTriangleRefArray triangles(geom.GetTriangles());
		XSI::ClusterProperty UVWProp(m_pMesh.GetMaterial().GetCurrentUV());
		
		if ( UVWProp.IsValid() )
		{

		XSI::CClusterPropertyElementArray clusterPropertyElements = UVWProp.GetElements();
		XSI::CDoubleArray elementArray = clusterPropertyElements.GetArray();
		
		long totalUvCount = elementArray.GetCount ();
		int cc=0;
		int uvc = 0;
		for (int c=0;c<m_pTriangles.GetUsed();c++)
		{
			long l_iNumVertex = indices[cc];
			
			cc++;
			for (int i=0;i<l_iNumVertex;i++)
			{
				long l_iID = indices[cc];
				cc++;
				
				uvvec vec;
				compress.GetVertex (l_iID, &vec);

				elementArray[ uvc * 3 ] = vec.u;
				
				elementArray[ (uvc * 3) + 1] = vec.v;
				elementArray[ (uvc * 3) + 2] = 0.0f;
				
				uvc++;
			}
			
			
			
		}


			clusterPropertyElements.PutArray(elementArray);
		}
	}

}

void	SMDImporter::CreateShapeClips()
{
	if ( !m_pVTA.GetUsed() )
		return;

	XSI::Application app;

	for (int v=0;v<m_pVTA.GetUsed();v++)
	{
		XSI::Geometry geo = m_pMesh.GetActivePrimitive().GetGeometry();
		XSI::MATH::CVector3Array positions( geo.GetPoints().GetPositionArray() );

		for (int i=0;i<m_pVTA[v].m_pVertices.GetUsed();i++)
		{
			VTAVertex* p = &m_pVTA[v].m_pVertices[i];

			positions[m_lVertexMap[p->m_iIndex]] = p->m_vPosition;
//			positions[p->m_iIndex]

		}
		
		geo.GetPoints().PutPositionArray(positions);

		XSI::CValueArray args(7);
		XSI::CValue retval;
		
		args[i++]= m_pMesh;
		args[i++]= L"FacialExpression";
		args[i++]= (long)XSI::siShapeObjectReferenceMode;
		args[i++]= (long)v;
		args[i++]= (long)0;
		args[i++]= (long)0;
		args[i++]= (long)XSI::siShapeContentPrimaryShape;
		
		app.ExecuteCommand( L"StoreShapeKey", args, retval );

	}

}

void	SMDImporter::WeightGeometry()
{
	if ( !m_pTriangles.GetUsed() )
		return;

	XSI::CRefArray l_bones;
	CSIBCArray<XSI::CDoubleArray>	weightList;
	weightList.Extend ( m_pNodes.GetUsed() );

	for (int e=0;e<m_pNodes.GetUsed();e++)
	{
		weightList[e].Resize ( compress.GetCount() );
	}


	XSI::MATH::CTransformation xfo;
	xfo.SetRotationFromXYZAnglesValues ( -1.570796, 0.0, 0.0 );

	for (int n=0;n<m_pTriangles.GetUsed();n++)
	{
		SMDTriangle* tri = m_pTriangles[n];
		for (int v=0;v<3;v++)
		{
			//
			// get the index of this vertex
			//
			XSI::MATH::CVector3 vec1 = XSI::MATH::MapObjectPositionToWorldSpace ( xfo, tri->m_pVertex[v].m_vPosition );

			long index = compress.GetIndex (vec1.GetX(), 
											vec1.GetY(), 
											vec1.GetZ(),
											tri->m_pVertex[v].m_vUV.GetX(),
											tri->m_pVertex[v].m_vUV.GetY() );

			for (int w=0;w<tri->m_pVertex[v].m_vWeights.GetUsed();w++)
			{
				Weight* weight = &tri->m_pVertex[v].m_vWeights[w];

				XSI::X3DObject bone = GetNodeByID(weight->m_iBoneID);

				bool found = false;
				int ib = 0;
				for (int b=0;b<l_bones.GetCount();b++)
				{
					if ( l_bones[b] == bone )
					{
						found =true;
						break;
					}
					ib++;
				}

				if ( !found )
				{
					l_bones.Add ( bone );
				}

				weightList[ib];
				weightList[ib][index] = weight->m_fWeight * 100.0f;

			}
		}

	}

	//
	// apply envelope
	//

	XSI::Envelope myEnvelope;
	m_pMesh.ApplyEnvelope(	l_bones, XSI::siUnspecified, XSI::siUnspecified, myEnvelope);


	//
	// zero all weights
	//

	for (int z=0;z<l_bones.GetCount();z++)
	{

		XSI::X3DObject	X3d = l_bones[z];

		XSI::CDoubleArray refWeights = myEnvelope.GetDeformerWeights( X3d, 0.0 );

		for (int wc=0;wc<refWeights.GetCount();wc++)
		{
			refWeights[wc] = 0.0;
		}
	
		//
		// first set to zero
		myEnvelope.SetDeformerWeights( X3d, refWeights );


	}

	for (int f=0;f<l_bones.GetCount();f++)
	{

		XSI::X3DObject	X3d = l_bones[f];

		myEnvelope.SetDeformerWeights( X3d, weightList[f] );
	}

}

XSI::X3DObject SMDImporter::GetNodeByID(int ID )
{
	XSI::X3DObject ret;

	for (int c=0;c<m_pNodes.GetUsed();c++)
	{
		if ( m_pNodes[c]->m_iID == ID )
		{
			ret = m_pNodes[c]->m_x3d;
			break;
		}
	}

	return ret;

}


void fixNames ( char* buffer )
{
	long iindex = 0;

	while ( buffer [iindex] != 0 )
	{
		if ( buffer[iindex] == 34 )
		{
			while ( buffer[++iindex] != 34 )
			{

				if  ( ( buffer[iindex] == ' ' ) || 
					( buffer[iindex] == '$' ) ||
					( buffer[iindex] == '%' ) ||
					( buffer[iindex] == ',' ) ||
					( buffer[iindex] == '!' ) ||
					( buffer[iindex] == '@' ) ||
					( buffer[iindex] == '*' ) ||
					( buffer[iindex] == '(' ) ||
					( buffer[iindex] == ')' ))
				{
					buffer[iindex] = '_';
				}

			}
		}

		iindex ++;

	}
}

XSI::Null DoesObjectExist ( XSI::X3DObject in_pModel, XSI::CString in_szName )
{
	XSI::Null theNull;

	XSI::CStringArray emptyArray;
	XSI::CRefArray cRefArray = in_pModel.FindChildren( L"", L"", emptyArray, true );

	XSI::CStringArray nameArray(cRefArray.GetCount());
	for ( long i=0; i < cRefArray.GetCount(); i++ )
	{
		if ( in_szName == XSI::SIObject(cRefArray[i]).GetName() )
		{
			theNull = cRefArray[i];
			break;
		}
	}

	return theNull;

}