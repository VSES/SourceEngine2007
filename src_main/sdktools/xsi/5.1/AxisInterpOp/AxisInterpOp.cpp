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

#include <xsi_string.h>
#include <xsi_value.h>
#include <xsi_kinematicstate.h>
#include <xsi_math.h>
#include <xsi_operator.h>
#include <xsi_outputport.h>
#include <xsi_inputport.h>
#include <xsi_updatecontext.h>
#include <xsi_application.h>

using namespace XSI;
using namespace MATH;

#define d2r(val) DegreesToRadians(val)
#define r2d(val) RadiansToDegrees(val)

//////////////////////////////////////////////////////////////////////////////
// CAxisInterpOp operator declaration
//
//////////////////////////////////////////////////////////////////////////////
class CAxisInterpOp
{
public:
	CAxisInterpOp() :
		m_cTriggers(0),
		m_aWeights(0),
		m_aTriggerOri(0),
		m_aTriggerTol(0),
		m_aTargetOri(0),
		m_aTargetPos(0)
	{}
	~CAxisInterpOp() 
	{
		if ( m_aWeights )
			delete []m_aWeights;
		if ( m_aTriggerTol )
			delete []m_aTriggerTol;
		if ( m_aTriggerOri )
			delete []m_aTriggerOri;
		if ( m_aTargetOri )
			delete []m_aTargetOri;
		if ( m_aTargetPos )
			delete []m_aTargetPos;
	}

	CStatus Init(UpdateContext&, long );
	CStatus Update(UpdateContext&,OutputPort&);
	CStatus Term(UpdateContext&, long ) { return CStatus::OK; }

private:
	void ComputeBaseOffset( XSI::MATH::CVector3& rvBaseOffset, double dBonePerc, double dBoneLen );
	void ComputeWeights( XSI::MATH::CQuaternion&, unsigned long, double*, double*, double* );
	void SumTargets( XSI::MATH::CQuaternion&, XSI::MATH::CVector3&, unsigned long, double*, double*, double* );

	CString	m_csTriggers;
	unsigned long m_cTriggers;
	double* m_aWeights;
	double* m_aTriggerOri;
	double* m_aTriggerTol;
	double* m_aTargetOri;
	double* m_aTargetPos;
};

//////////////////////////////////////////////////////////////////////////////
// DLL Entry Points
//////////////////////////////////////////////////////////////////////////////

//
// //Update// Entry Point, called by XSI when the operator must evaluate the
// output of the AxisInterpOp plug-in.
		//

#ifdef UNIX
extern "C"
#endif
XSIPLUGINCALLBACK XSI::CStatus CppAxisInterpOp_Init
(
	UpdateContext&	ctx,
	long mode
)
{
	CAxisInterpOp* pThis = new CAxisInterpOp();	
	ctx.PutUserData( (CValue::siPtrType)pThis );

	return pThis->Init(ctx,mode);
}

#ifdef UNIX
extern "C"
#endif
XSIPLUGINCALLBACK XSI::CStatus CppAxisInterpOp_Term
(
	UpdateContext&	ctx,
	long 		mode
)
{
	CValue::siPtrType pUserData = ctx.GetUserData();
	CAxisInterpOp* pThis = (CAxisInterpOp*)pUserData;

	CStatus st = pThis->Term(ctx,mode);
	delete pThis;

	return st;
}

#ifdef UNIX
extern "C"
#endif
XSIPLUGINCALLBACK XSI::CStatus CppAxisInterpOp_Update
(
	UpdateContext&	ctx,
	OutputPort&	output
)
{
	CValue::siPtrType pUserData = ctx.GetUserData();
	CAxisInterpOp* pThis = (CAxisInterpOp*)pUserData;	

	return pThis->Update(ctx,output);
}

XSI::CStatus CAxisInterpOp::Init(UpdateContext& ctx, long )
{
	// clean up
	if ( m_aWeights )
		delete []m_aWeights;
	if ( m_aTriggerTol )
		delete []m_aTriggerTol;
	if ( m_aTriggerOri )
		delete []m_aTriggerOri;
	if ( m_aTargetOri )
		delete []m_aTargetOri;
	if ( m_aTargetPos )
		delete []m_aTargetPos;
	m_cTriggers = 0;

	static const wchar_t* wcsTriggerTag = L"<trigger> ";

	wchar_t* startpos = (wchar_t*)m_csTriggers.GetWideString();
	if ( !startpos )  // no triggers defined
		return( CStatus::Fail );

	wchar_t* endpos = startpos + m_csTriggers.Length(); 

	// allocate memory
	unsigned long cBuffSize(10);

	m_aWeights = new double[cBuffSize]; 
	::memset( m_aWeights, 0, sizeof(double)*cBuffSize );
	m_aTriggerTol = new double[cBuffSize];
	::memset( m_aTriggerTol, 0, sizeof(double)*cBuffSize );
	m_aTriggerOri = new double[cBuffSize*3];
	::memset( m_aTriggerOri, 0, sizeof(double)*cBuffSize*3 );
	m_aTargetOri = new double[cBuffSize*3];
	::memset( m_aTargetOri, 0, sizeof(double)*cBuffSize*3 );
	m_aTargetPos = new double[cBuffSize*3];
	::memset( m_aTargetPos, 0, sizeof(double)*cBuffSize*3 );

	// ParseHelperEntry
	wchar_t* nexttrigger = ::wcsstr( startpos, wcsTriggerTag);

	while ( nexttrigger < endpos && nexttrigger != 0 && m_cTriggers<cBuffSize ) {

		::swscanf( nexttrigger,
			L"<trigger> %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", 
			&m_aTriggerTol[m_cTriggers],

			&m_aTriggerOri[(m_cTriggers*3)+0],
			&m_aTriggerOri[(m_cTriggers*3)+1],
			&m_aTriggerOri[(m_cTriggers*3)+2],

			&m_aTargetOri[(m_cTriggers*3)+0],
			&m_aTargetOri[(m_cTriggers*3)+1],
			&m_aTargetOri[(m_cTriggers*3)+2],

			&m_aTargetPos[(m_cTriggers*3)+0],
			&m_aTargetPos[(m_cTriggers*3)+1],
			&m_aTargetPos[(m_cTriggers*3)+2]
			);

		m_cTriggers++;
		
		nexttrigger = ::wcsstr( nexttrigger+1, wcsTriggerTag);
	};

	
	return CStatus::OK;
}

// cpp
// 31.0 no bone helpers no boxes
// 29.5 no bone helper 
// 30.4 (20.0) no algorithm + no getport
// 26.5 no bone length + no algorithm + no reads
// 18.8 no bone length + no algorithm 
// 13.6 (14.4) no algorithm (GetPort)
// 13.0 no algorithm (GetPortAt)

// vbscript
// 13.4 (14.4) no algorithm
// 9.4 (10.1) full algorithm

//////////////////////////////////////////////////////////////////////////////
// CAxisInterpOp implementation
//////////////////////////////////////////////////////////////////////////////

XSI::CStatus CAxisInterpOp::Update
(
	UpdateContext&	ctx,
	OutputPort&	output
)
{
	Operator op(ctx.GetOperator());

	///////////////////////////////////////////////////////////////
	// get operator parameters
	///////////////////////////////////////////////////////////////

	XSI::CString triggers(op.GetParameterValue(L"Triggers"));

	// triggers changed
	if ( m_csTriggers != triggers )
	{
		m_csTriggers = triggers;

		Init( ctx, 0 );
	}

	double boneperc = op.GetParameterValue(L"BoneDist");

	///////////////////////////////////////////////////////////////
	// get objects connected to input & output ports
	///////////////////////////////////////////////////////////////
 
	InputPort rootboneport(op.GetPort(L"globalkineport",L"RootBoneGroup",0));
	InputPort parentboneport(op.GetPort(L"globalkineport",L"ParentBoneGroup",0));
	InputPort parentbonelenport(op.GetPort(L"bonelengthport",L"ParentBoneGroup",0));
	InputPort childboneport(op.GetPort(L"globalkineport",L"ChildBoneGroup",0));

	KinematicState gkRoot(rootboneport.GetValue());
	KinematicState gkParent(parentboneport.GetValue());
	double parentbonelen(parentbonelenport.GetValue());
	KinematicState gkChild(childboneport.GetValue());
	KinematicState gkHelper(output.GetValue());

	// GET TRANSFORMATIONS OF ROOT, PARENT & CHILD
	CTransformation tGRoot = gkRoot.GetTransform();
	CTransformation tGBone1 = gkParent.GetTransform();
	CTransformation tGBone2 = gkChild.GetTransform();

#ifdef _DEBUG_UPDATE
	{
		Application app;
		wchar_t wszBuf[256]; 

		XSI::MATH::CQuaternion q = tGBone1.GetRotationQuaternion();

		double x, y, z;
		
		q.GetXYZAnglesValues(x,y,z);

		swprintf( wszBuf, L"parent R(%f,%f,%f)]",  r2d(x),r2d(y),r2d(z) );
		app.LogMessage( (const wchar_t*)wszBuf ); 
	}
#endif
#ifdef _DEBUG_UPDATE
	{
		Application app;
		wchar_t wszBuf[256]; 

		XSI::MATH::CQuaternion q = tGBone2.GetRotationQuaternion();

		double x, y, z;
		
		q.GetXYZAnglesValues(x,y,z);

		swprintf( wszBuf, L"child R(%f,%f,%f)]",  r2d(x),r2d(y),r2d(z) );
		app.LogMessage( (const wchar_t*)wszBuf ); 
	}
#endif

	///////////////////////////////////////////////////////////////
	// perform update function
	///////////////////////////////////////////////////////////////
 
	// GET LOCAL TRANSFORM OF CHILD RELATIVE TO PARENT
	XSI::MATH::CMatrix3 mBone1( tGBone1.GetRotationMatrix3() );
	XSI::MATH::CMatrix3 mBone2( tGBone2.GetRotationMatrix3() );

#ifdef _DEBUG_UPDATE
	{
		Application app;
		wchar_t wszBuf[256]; 

		double m0, m1, m2, m3, m4, m5, m6, m7, m8;
		
		mBone1.Get(m0, m1, m2, m3, m4, m5, m6, m7, m8);

		swprintf( wszBuf, L"mBone1->\n%.2f,%.2f,%.2f\n%.2f,%.2f,%.2f\n%.2f,%.2f,%.2f", m0, m1, m2, m3, m4, m5, m6, m7, m8 );
			
		app.LogMessage( (const wchar_t*)wszBuf ); 
	}
#endif
#ifdef _DEBUG_UPDATE
	{
		Application app;
		wchar_t wszBuf[256]; 

		double m0, m1, m2, m3, m4, m5, m6, m7, m8;
		
		mBone2.Get(m0, m1, m2, m3, m4, m5, m6, m7, m8);

		swprintf( wszBuf, L"mBone2->\n%.2f,%.2f,%.2f\n%.2f,%.2f,%.2f\n%.2f,%.2f,%.2f", m0, m1, m2, m3, m4, m5, m6, m7, m8 );
			
		app.LogMessage( (const wchar_t*)wszBuf ); 
	}
#endif
	mBone1.TransposeInPlace();
#ifdef _DEBUG_UPDATE
	{
		Application app;
		wchar_t wszBuf[256]; 

		double m0, m1, m2, m3, m4, m5, m6, m7, m8;
		
		mBone1.Get(m0, m1, m2, m3, m4, m5, m6, m7, m8);

		swprintf( wszBuf, L"mBone1.TransposeInPlace->\n%.2f,%.2f,%.2f\n%.2f,%.2f,%.2f\n%.2f,%.2f,%.2f", m0, m1, m2, m3, m4, m5, m6, m7, m8 );
			
		app.LogMessage( (const wchar_t*)wszBuf ); 
	}
#endif

	XSI::MATH::CMatrix3  tmpMat3;

	for(int nR=0; nR<3; nR++)
		for(int nC=0; nC<3; nC++)
		{
			tmpMat3.SetValue(nR,nC , 
				mBone2.GetValue(nR,0) * mBone1.GetValue(0,nC) +
				mBone2.GetValue(nR,1) * mBone1.GetValue(1,nC) +
				mBone2.GetValue(nR,2) * mBone1.GetValue(2,nC) );
		}
	
	mBone2 = tmpMat3;

	// bug #90494
	// mBone2.MulInPlace( mBone1 );
#ifdef _DEBUG_UPDATE
	{
		Application app;
		wchar_t wszBuf[256]; 

		double m0, m1, m2, m3, m4, m5, m6, m7, m8;
		
		mBone2.Get(m0, m1, m2, m3, m4, m5, m6, m7, m8);

		swprintf( wszBuf, L"mBone2.MulInPlace( mBone1 )->\n%.2f,%.2f,%.2f\n%.2f,%.2f,%.2f\n%.2f,%.2f,%.2f", m0, m1, m2, m3, m4, m5, m6, m7, m8 );
			
		app.LogMessage( (const wchar_t*)wszBuf ); 
	}
#endif

	// GET ORIENTATION OF BONE2 RELATIVE TO BONE1 AS A QUATERNION
	XSI::MATH::CQuaternion qBone2 = mBone2.GetQuaternion();

	// MATCH QUATERNIONS
	XSI::MATH::CVector3 vBasePos(
		op.GetParameterValue(L"BasePoseX"), 
		op.GetParameterValue(L"BasePoseY"), 
		op.GetParameterValue(L"BasePoseZ") );
	
#ifdef _DEBUG_UPDATE
	{
		Application app;
		wchar_t wszBuf[256]; 

		double x, y, z;
		
		qBone2.GetXYZAnglesValues(x,y,z);

		swprintf( wszBuf, L"child2parent R(%f,%f,%f)]",  r2d(x),r2d(y),r2d(z) );
		app.LogMessage( (const wchar_t*)wszBuf ); 
	}
#endif

	ComputeBaseOffset(vBasePos, boneperc, parentbonelen );

	ComputeWeights( qBone2, m_cTriggers, m_aWeights, m_aTriggerOri, m_aTriggerTol );

	// SUM TARGET ORIENTATIONS & POSITIONS
	XSI::MATH::CQuaternion qNewOri;

	SumTargets( qNewOri, vBasePos, m_cTriggers, m_aWeights, m_aTargetOri, m_aTargetPos );	

#ifdef _DEBUG_UPDATE
	Application app;
	wchar_t wszBuf[256]; 

	double x, y, z;
	
	qNewOri.GetXYZAnglesValues(x,y,z);

	swprintf( wszBuf, L"qNewOri->R(%f,%f,%f)]",  r2d(x),r2d(y),r2d(z) );
	app.LogMessage( (const wchar_t*)wszBuf ); 
#endif

	// UPDATE TRANSFORMATION
	XSI::MATH::CTransformation tNewPose;

	tNewPose.SetRotationFromQuaternion( qNewOri );
	tNewPose.SetTranslation( vBasePos );
	tNewPose.MulInPlace( tGRoot );

	///////////////////////////////////////////////////////////////
	// update output port
	///////////////////////////////////////////////////////////////

	gkHelper.PutTransform( tNewPose );

	return CStatus::OK;
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

const double _PI = 4 * (double)atan((double)1);

#define _acos(X) \
( X >= 1 ) ? 0 : (( X <= -1 ) ? _PI : ::atan(-X / ::sqrt(-X * X + 1)) + (_PI/2)) 

// #define _DEBUG_COMPUTEWEIGHTS

void CAxisInterpOp::ComputeWeights
( 
	XSI::MATH::CQuaternion& qbone2, 
	unsigned long size,			// number of triggers
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
void CAxisInterpOp::SumTargets( XSI::MATH::CQuaternion& qrot, XSI::MATH::CVector3&  vpos, 
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

//****************************************************************************************
// function			: ComputeBaseOffset
// description		: compute the position based on the percentage along the bone
// with an added offset. 
//
// vOffeset			: vector3, offset to add to final position
// dtBone2LPose			: double, percentage along bone length where 0 <= v <= 100
// dtBone2LPose			: double, bone length
//
// return value	:	SIVector3 
//****************************************************************************************
void CAxisInterpOp::ComputeBaseOffset( XSI::MATH::CVector3& rvBaseOffset, double dBonePerc, double dBoneLen )
{	
	double delta = 0;

	if ( dBonePerc != 0 ) {
		delta = dBonePerc / 100.0;
	}

	// move the position a percentage along the bone length	
	rvBaseOffset.PutX( rvBaseOffset.GetX() + ( dBoneLen * delta ) );

}