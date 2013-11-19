//////////////////////////////////////////////////////////////////////////////
//
// This file implements the AxisInterpOp example plug-in for XSI in C++ using
// the CPP API SDK feature.
//
//////////////////////////////////////////////////////////////////////////////

#include <math.h>		// acos
#include <string.h>		// wcsstr
#include <stdio.h>		// wscanf
#include <assert.h>		// assert
#include <malloc.h>

#include <xsi_string.h>
#include <xsi_value.h>
#include <xsi_kinematicstate.h>
#include <xsi_math.h>
#include <xsi_operator.h>
#include <xsi_outputport.h>
#include <xsi_inputport.h>
#include <xsi_updatecontext.h>
#include <xsi_application.h>
#include <xsi_griddata.h>
#include <xsi_property.h>

using namespace XSI;
using namespace MATH;

#define d2r(val) DegreesToRadians(val)
#define r2d(val) RadiansToDegrees(val)

#ifndef XSIPLUGINCALLBACK
	// Use this define to properly export C++ API callbacks
	// in your dll, for example XSILoadPlugin()
	// Note for windows developers: Using this define means that you do not need to use a .def file 
	
	#ifdef unix
		#define XSIPLUGINCALLBACK extern "C"
	#else
		// XSI C++ API uses C-linkage.  Disable the warning
		// that reminds us that CStatus is a C++ object
		#pragma warning( disable : 4190 ) 
		#define XSIPLUGINCALLBACK extern "C" __declspec(dllexport)
	#endif
#endif

#ifdef _DEBUG_UPDATE
inline void DebugPrint( const XSI::CString& str, const XSI::MATH::CQuaternion& q )
{
			Application app;
			wchar_t wszBuf[256]; 

			XSI::MATH::CQuaternion q = tGParent.GetRotationQuaternion();

			double x, y, z;
			
			q.GetXYZAnglesValues(x,y,z);

			swprintf( wszBuf, L"%s R(%f,%f,%f)]", (const wchar_t*)str, r2d(x),r2d(y),r2d(z) );
			app.LogMessage( (const wchar_t*)wszBuf ); 
}
inline void DebugPrint( const XSI::CString& str, const XSI::MATH::CMatrix3& m3 )
{
	Application app;
	wchar_t wszBuf[256]; 

	double m0, m1, m2, m3, m4, m5, m6, m7, m8;
	
	m3.Get(m0, m1, m2, m3, m4, m5, m6, m7, m8);

	swprintf( wszBuf, L"%s\n%.2f,%.2f,%.2f\n%.2f,%.2f,%.2f\n%.2f,%.2f,%.2f", (const wchar_t*)str, m0, m1, m2, m3, m4, m5, m6, m7, m8 );
		
	app.LogMessage( (const wchar_t*)wszBuf ); 
}

#else
#define DebugPrint( x, y )
#endif

inline void MulInPlace( XSI::MATH::CMatrix3& m3lhs, const XSI::MATH::CMatrix3& m3rhs )
{
	XSI::MATH::CMatrix3  tmpMat3;

	for(int nR=0; nR<3; nR++)
		for(int nC=0; nC<3; nC++)
		{
			tmpMat3.SetValue(nR,nC , 
				m3lhs.GetValue(nR,0) * m3rhs.GetValue(0,nC) +
				m3lhs.GetValue(nR,1) * m3rhs.GetValue(1,nC) +
				m3lhs.GetValue(nR,2) * m3rhs.GetValue(2,nC) );
		}
	
	m3lhs = tmpMat3;
}

//////////////////////////////////////////////////////////////////////////////
// CHelperBoneOp operator declaration
//
//////////////////////////////////////////////////////////////////////////////
class CHelperBoneOp
{
public:
	CHelperBoneOp() :
		m_cTriggers(0),
		m_aEnabled(0),
		m_aWeights(0),
		m_aTriggerOri(0),
		m_aTriggerTol(0),
		m_aTargetOri(0),
		m_aTargetPos(0)
	{}
	~CHelperBoneOp() 
	{
		Cleanup();	
	}

	CStatus Init(UpdateContext&, long );
	CStatus Update(UpdateContext&,OutputPort&);
	CStatus Term(UpdateContext&, long ) { return CStatus::OK; }

private:
	void Cleanup()
	{
		if ( m_aEnabled )
			::free(m_aEnabled);
		m_aEnabled=0;
		if ( m_aWeights )
			::free(m_aWeights);
		m_aWeights=0;
		if ( m_aTriggerTol )
			::free(m_aTriggerTol);
		m_aTriggerTol=0;
		if ( m_aTriggerOri )
			::free(m_aTriggerOri);
		m_aTriggerOri=0;
		if ( m_aTargetOri )
			::free(m_aTargetOri);
		m_aTargetOri=0;
		if ( m_aTargetPos )
			::free(m_aTargetPos);
		m_aTargetPos=0;
		m_cTriggers = 0;
	}

	void ReadTriggerData( XSI::GridData& );

	void ComputeWeights( XSI::MATH::CQuaternion&, unsigned long, bool*, double*, double*, double* );
	void SumTargets( XSI::MATH::CQuaternion&, XSI::MATH::CVector3&, unsigned long, double*, double*, double* );

	unsigned long m_cTriggers;
	bool*   m_aEnabled;
	double* m_aWeights;
	double* m_aTriggerOri;
	double* m_aTriggerTol;
	double* m_aTargetOri;
	double* m_aTargetPos;
};

//////////////////////////////////////////////////////////////////////////////
// DLL Entry Points
//////////////////////////////////////////////////////////////////////////////

XSIPLUGINCALLBACK XSI::CStatus HelperBoneOp_Init
(
	UpdateContext&	ctx,
	long mode
)
{
	CHelperBoneOp* pThis = new CHelperBoneOp();	
	ctx.PutUserData( (CValue::siPtrType)pThis );

	return pThis->Init(ctx,mode);
}

XSIPLUGINCALLBACK XSI::CStatus HelperBoneOp_Term
(
	UpdateContext&	ctx,
	long 		mode
)
{
	CValue::siPtrType pUserData = ctx.GetUserData();
	CHelperBoneOp* pThis = (CHelperBoneOp*)pUserData;

	CStatus st = pThis->Term(ctx,mode);
	delete pThis;

	return st;
}

XSIPLUGINCALLBACK XSI::CStatus HelperBoneOp_Update
(
	UpdateContext&	ctx,
	OutputPort&	output
)
{
	CValue::siPtrType pUserData = ctx.GetUserData();
	CHelperBoneOp* pThis = (CHelperBoneOp*)pUserData;	

	return pThis->Update(ctx,output);
}

CStatus CHelperBoneOp::Init(UpdateContext& ctx, long )
{
	// clean up
	Cleanup();

	return CStatus::OK;
}

//////////////////////////////////////////////////////////////////////////////
// CHelperBoneOp implementation
//////////////////////////////////////////////////////////////////////////////

CStatus CHelperBoneOp::Update
(
	UpdateContext&	ctx,
	OutputPort&	output
)
{
	///////////////////////////////////////////////////////////////
	// get operator
	///////////////////////////////////////////////////////////////
	Operator op(ctx.GetOperator());

	///////////////////////////////////////////////////////////////
	// get output port
	///////////////////////////////////////////////////////////////
 
	KinematicState gkHelper(output.GetValue());

	///////////////////////////////////////////////////////////////
	// get helper bone data
	///////////////////////////////////////////////////////////////

	InputPort bonedataport(op.GetPort(L"bonedataport",L"HelperBoneGroup",0));
	Property  HelperBoneData(bonedataport.GetValue());

	bool enabled = HelperBoneData.GetParameterValue(L"Enabled");

	// not enabled: do nothing
	if (!enabled) 
		return CStatus::OK;

	///////////////////////////////////////////////////////////////
	// evaluate new transformation for helperbone
	///////////////////////////////////////////////////////////////
	XSI::MATH::CTransformation tNewPose;

	bool bComputeInWorldSpace = (0!=(long)HelperBoneData.GetParameterValue(L"ComputationSpace"));

	///////////////////////////////////////////////////////////////
	// get objects connected to input & output ports
	///////////////////////////////////////////////////////////////

	InputPort rootboneport(op.GetPort(L"globalkineport",L"RootBoneGroup",0));
	InputPort parentboneport(op.GetPort(L"globalkineport",L"ParentBoneGroup",0));
	InputPort childboneport(op.GetPort(L"globalkineport",L"ChildBoneGroup",0));

	KinematicState gkRoot(rootboneport.GetValue());
	KinematicState gkParent(parentboneport.GetValue());
	KinematicState gkChild(childboneport.GetValue());

	// get helperbonedata values
	XSI::MATH::CVector3 vBasePos(
		HelperBoneData.GetParameterValue(L"BoneOffsetX"), 
		HelperBoneData.GetParameterValue(L"BoneOffsetY"), 
		HelperBoneData.GetParameterValue(L"BoneOffsetZ") );

	double perc_along_root = (double)HelperBoneData.GetParameterValue(L"BoneDistance") / 100.0;
	double root_bone_length = (double)HelperBoneData.GetParameterValue(L"RootBoneLength");

	XSI::GridData griddata(HelperBoneData.GetParameterValue(L"Triggers"));

	// read triggers
	ReadTriggerData(griddata);

	// GET TRANSFORMATIONS OF ROOT, PARENT & CHILD
	CTransformation tGRoot = gkRoot.GetTransform();
	CTransformation tGParent = gkParent.GetTransform();
	CTransformation tGChild = gkChild.GetTransform();

	///////////////////////////////////////////////////////////////
	// compute new orientation and position based on triggers
	///////////////////////////////////////////////////////////////
	DebugPrint( L"parent", tGParent.GetRotationQuaternion() );
	DebugPrint( L"child", tGChild.GetRotationQuaternion() );

	// GET LOCAL TRANSFORM OF CHILD RELATIVE TO PARENT
	XSI::MATH::CMatrix3 m3Parent( tGParent.GetRotationMatrix3() );
	DebugPrint(L"m3Parent->", m3Parent);

	XSI::MATH::CMatrix3 m3Child( tGChild.GetRotationMatrix3() );
	DebugPrint(L"m3Parent->", m3Child);

	m3Parent.TransposeInPlace();
	DebugPrint(L"m3Parent.TransposeInPlace->", m3Parent);

	// bug #90494
	// m3Child.MulInPlace( m3Parent );
	MulInPlace( m3Child, m3Parent );
	DebugPrint(L"m3Child.MulInPlace->", m3Child);

	// GET ORIENTATION OF BONE2 RELATIVE TO BONE1 AS A QUATERNION
	XSI::MATH::CQuaternion qBone2 = m3Child.GetQuaternion();
	DebugPrint( L"child2parent", qBone2 );

	// MATCH QUATERNIONS
	ComputeWeights( qBone2, m_cTriggers, m_aEnabled, m_aWeights, m_aTriggerOri, m_aTriggerTol );

	// SUM TARGET ORIENTATIONS & POSITIONS
	XSI::MATH::CQuaternion qNewOri;
	XSI::MATH::CVector3 vNewPos;

	SumTargets( qNewOri, vNewPos, m_cTriggers, m_aWeights, m_aTargetOri, m_aTargetPos );	
	DebugPrint( L"qNewPos->", qNewPos );
	DebugPrint( L"vNewOri->", qNewOri );

	// UPDATE TRANSFORMATION
	if (bComputeInWorldSpace)
	{
		// not implemented
	}
	else // Root object space
	{
		// compute initial helperbone position
		vBasePos.PutX( vBasePos.GetX() + (root_bone_length * perc_along_root) );
		vNewPos.AddInPlace(vBasePos);

		// apply changes from triggers
		tNewPose.SetRotationFromQuaternion( qNewOri );
		tNewPose.SetTranslation( vNewPos );

		// map root object space to worldspace
		tNewPose.MulInPlace( tGRoot );
	}

	///////////////////////////////////////////////////////////////
	// update output port
	///////////////////////////////////////////////////////////////

	gkHelper.PutTransform( tNewPose );


	return CStatus::OK;
}

//****************************************************************************************
// function		: ReadTriggerData
//****************************************************************************************
void CHelperBoneOp::ReadTriggerData( XSI::GridData& in_griddata )
{ 
	long cRows( in_griddata.GetRowCount() );

	// allocate memory
	if (!m_aEnabled || m_cTriggers != (unsigned long)cRows )
	{
		size_t size(0);
		
		size = sizeof(bool) * cRows;
		m_aEnabled = (bool*)((m_aEnabled) ? ::realloc(m_aEnabled,size) : ::malloc(size)); 
		::memset( m_aEnabled, 0, size );

		size = sizeof(double) * cRows;
		m_aWeights = (double*)((m_aWeights) ? ::realloc(m_aWeights,size) : ::malloc(size)); 
		::memset( m_aWeights, 0, size);

		size = sizeof(double) * cRows * 3;
		m_aTriggerTol = (double*)((m_aTriggerTol) ? ::realloc(m_aTriggerTol,size) : ::malloc(size)); 
		::memset( m_aTriggerTol, 0, size );

		size = sizeof(double) * cRows * 3;
		m_aTriggerOri = (double*)((m_aTriggerOri) ? ::realloc(m_aTriggerOri,size) : ::malloc(size)); 
		::memset( m_aTriggerOri, 0, size );

		size = sizeof(double) * cRows * 3;
		m_aTargetOri = (double*)((m_aTargetOri) ? ::realloc(m_aTargetOri,size) : ::malloc(size)); 
		::memset( m_aTargetOri, 0, size );

		size = sizeof(double) * cRows * 3;
		m_aTargetPos = (double*)((m_aTargetPos) ? ::realloc(m_aTargetPos,size) : ::malloc(size)); 
		::memset( m_aTargetPos, 0, size );
	}

	long iRow;
	for ( iRow=0; iRow < cRows; iRow++ )
	{
		long iCell=0;

		m_aEnabled[iRow] = (bool)in_griddata.GetCell(iCell++,iRow);
		m_aTriggerTol[iRow] = (double)in_griddata.GetCell(iCell++,iRow);

		m_aTriggerOri[(iRow*3)+0] = (double)in_griddata.GetCell(iCell++,iRow);
		m_aTriggerOri[(iRow*3)+1]= (double)in_griddata.GetCell(iCell++,iRow);
		m_aTriggerOri[(iRow*3)+2]= (double)in_griddata.GetCell(iCell++,iRow);

		m_aTargetOri[(iRow*3)+0]= (double)in_griddata.GetCell(iCell++,iRow);
		m_aTargetOri[(iRow*3)+1]= (double)in_griddata.GetCell(iCell++,iRow);
		m_aTargetOri[(iRow*3)+2]= (double)in_griddata.GetCell(iCell++,iRow);

		m_aTargetPos[(iRow*3)+0]= (double)in_griddata.GetCell(iCell++,iRow);
		m_aTargetPos[(iRow*3)+1]= (double)in_griddata.GetCell(iCell++,iRow);
		m_aTargetPos[(iRow*3)+2]= (double)in_griddata.GetCell(iCell++,iRow);
	};

	m_cTriggers = iRow;
}

//***************************************************************************************
// function		: ComputeWeights
// description	: return an array of weights for each trigger orientation. The
// weights are computed using the dot product between bone2 and the trigger orientation.
// The sum of the weights will always be 0 <= w <= 1
//
// q1			: source quaternion of child bone relative to parent bone
// size			: number of triggers
// aweights		: result array of computed weights 
// atriggers	: array of trigger angles (degrees)
// atolerances	: array of tolerances for each trigger 
//***************************************************************************************

inline double _acos(double X) 
{
	static double _PI = 4 * ::atan(1.0);

	return ( X >= 1 ) ? 0 : (( X <= -1 ) ? _PI : ::atan(-X / ::sqrt(-X * X + 1)) + (_PI/2));
}

// #define _DEBUG_COMPUTEWEIGHTS

void CHelperBoneOp::ComputeWeights
( 
	XSI::MATH::CQuaternion& qbone2, 
	unsigned long size,			// number of triggers
	bool* aenabled, 
	double* aweights, 
	double* atriggers, 
	double* atolerances 
)
{
	XSI::MATH::CQuaternion q2;

#ifdef _DEBUG_COMPUTEWEIGHTS
	Application app;
	wchar_t wszBuf[256]; 

	swprintf( wszBuf, L"ComputeWeights qbone2->[%f V(%f,%f,%f)]",  qbone2.GetW(), qbone2.GetX(), qbone2.GetY(), qbone2.GetZ() );
	app.LogMessage( (const wchar_t*)wszBuf ); 

	double x, y, z;
	
	qbone2.GetXYZAnglesValues(x,y,z);

	swprintf( wszBuf, L"ComputeWeights qbone2->R(%f,%f,%f)]",  r2d(x),r2d(y),r2d(z) );
	app.LogMessage( (const wchar_t*)wszBuf ); 
#endif

	// dim tsec : tsec = timer
	// fntrace "ComputeWeights("&j&") bone2 angle: " & fnstr(q2rot(qbone2))

	double sumw(0), weight(0), dot_product(0), tolerance(0);
	
	for ( unsigned long j=0,i=0; i < size*3; i=i+3,j=j+1 )
	{
		// skip trigger not enabled.
		if (!aenabled[j]) 
		{
			aweights[j] = 0;
			continue;
		}

		tolerance = d2r(atolerances[j]);
#ifdef _DEBUG_COMPUTEWEIGHTS
		swprintf( wszBuf, L"trigger(%d) ori: (%f,%f,%f)", j, atriggers[i], atriggers[i+1], atriggers[i+2] );
		app.LogMessage( (const wchar_t*)wszBuf ); 

		swprintf( wszBuf, L"trigger(%d) tolerance: %f", j, tolerance );
		app.LogMessage( (const wchar_t*)wszBuf ); 
#endif

		q2.SetFromXYZAnglesValues( d2r(atriggers[i]), d2r(atriggers[i+1]), d2r(atriggers[i+2]) );

		// compute dot product of quaternion
		dot_product = 
			qbone2.GetX() * q2.GetX() + 
			qbone2.GetY() * q2.GetY() + 
			qbone2.GetZ() * q2.GetZ() + 
			qbone2.GetW() * q2.GetW();
#ifdef _DEBUG_COMPUTEWEIGHTS
		swprintf( wszBuf, L"trigger(%d) dot: %f", j, dot_product );
		app.LogMessage( (const wchar_t*)wszBuf ); 
#endif

		if ( tolerance == 0 ) {
			weight = 0;
		} else {
#ifdef _DEBUG_COMPUTEWEIGHTS
//		swprintf( wszBuf, L"ComputeWeights(%d) _acos(%f) = %f", j, fabs(dot_product), _acos(fabs(dot_product)) );
//		app.LogMessage( (const wchar_t*)wszBuf ); 
		swprintf( wszBuf, L"trigger(%d) acos(%f) = %f", j, fabs(dot_product), acos(fabs(dot_product)) );
		app.LogMessage( (const wchar_t*)wszBuf ); 
#endif
			weight = 1.0 - (2.0 * acos(fabs(dot_product)) / tolerance);
#ifdef _DEBUG_COMPUTEWEIGHTS
		swprintf( wszBuf, L"trigger(%d) weight: %f", j, weight );
		app.LogMessage( (const wchar_t*)wszBuf ); 
#endif
			if ( weight < 0 ) {
				weight = 0;
			}
		}
#ifdef _DEBUG_COMPUTEWEIGHTS
		swprintf( wszBuf, L"trigger(%d) computed weight: %f", j, weight );
		app.LogMessage( (const wchar_t*)wszBuf ); 
#endif

		aweights[j] = weight;
		// fntrace "ComputeWeights("&j&") raw: " & aW(j)

		sumw=sumw+aweights[j];
	}

	// make sure sum of weights totals 1
	if ( sumw != 0 ) 
	{
		for ( unsigned long i=0; i < size; i++ )
		{
			if ( aweights[i] != 0 ) {
				aweights[i] = aweights[i] / sumw;
				//fntrace "ComputeWeights("&i&") normalized: " & aW(i)
			}  
#ifdef _DEBUG_COMPUTEWEIGHTS
			swprintf( wszBuf, L"trigger(%d) normalized = %f", i, aweights[i] );
			app.LogMessage( (const wchar_t*)wszBuf ); 
#endif
		}
	}
	//fntrace "AxisInterpOp::ComputeWeights: took " & timer-tsec & " seconds"

}

//***************************************************************************************
// function	:	SumTargets
// description: multiply all the weights by the set of new orientations and positions
// and sum the result to give the new orientation and position.
//
// aweights	: array of weights
// qrot  : resulting summed rotation quaternion
// vpos  : resulting summed position vector3
// atargetori : array of target orientations
// atargetpos : array of target positions
//***************************************************************************************
void CHelperBoneOp::SumTargets( XSI::MATH::CQuaternion& qrot, XSI::MATH::CVector3&  vpos, 
	unsigned long size, double* aweights, double* atargetori, double* atargetpos )
{
	double w;

	XSI::MATH::CRotation r1;
	XSI::MATH::CQuaternion q1;
	XSI::MATH::CQuaternion q2;
	XSI::MATH::CVector3 v1;

	qrot.Set(0,0,0,0);

	for ( unsigned long i=0; i < size; i++ )  
	{
		w = aweights[i];

		if ( w != 0 ) { 
			q1.SetFromXYZAnglesValues( d2r(atargetori[(i*3)+0]), d2r(atargetori[(i*3)+1]), d2r(atargetori[(i*3)+2]) );

			// w.qi
			q2.Set( q1.GetW() * w, q1.GetX() * w, q1.GetY() * w, q1.GetZ() * w );
			qrot.AddInPlace( q2 );

			// w.vi
			v1.Set( atargetpos[(i*3)+0], atargetpos[(i*3)+1], atargetpos[(i*3)+2] );
			v1.ScaleInPlace( w );

			vpos.AddInPlace( v1 );
		}

	}

	qrot.Normalize();
}
