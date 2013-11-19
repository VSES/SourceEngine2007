#ifndef _C3DVECTORPROCESSOR_H_
#define _C3DVECTORPROCESSOR_H_

#include <map>
#include <vector>

#include "SMDVertex.h"

typedef struct uvvec_tag
{
	float	x;
	float	y;
	float	z;
	float	u;
	float	v;
	SMDVertex * refVertex;
} uvvec;

typedef	const uvvec & REFDOTXSIVECTOR3;

class C3DCompareVector
{
public:
	inline bool operator()( REFDOTXSIVECTOR3 in_vec1, REFDOTXSIVECTOR3 in_vec2) const 
	{
		if (in_vec1.x != in_vec2.x) return in_vec1.x < in_vec2.x;
		if (in_vec1.y != in_vec2.y) return in_vec1.y < in_vec2.y;
		if (in_vec1.z != in_vec2.z) return in_vec1.z < in_vec2.z;
		if (in_vec1.u != in_vec2.u) return in_vec1.u < in_vec2.u;
		return in_vec1.v < in_vec2.v;
	}
} ;

typedef std::map<uvvec, ULONG, C3DCompareVector> C3DVectorMap;
typedef C3DVectorMap::iterator                           C3DVectorMapIterator;

typedef std::vector<uvvec>						 C3DVectorIndex;
typedef	C3DVectorIndex::iterator						 C3DVectorIndexIterator;


class C3DVectorCompressor
{
public:
	C3DVectorCompressor() { m_bCompress = TRUE; };
	~C3DVectorCompressor() {};
	HRESULT		AddVertex( double x, double y, double z, double u, double v, SMDVertex* refVertex, long* out_Index = NULL );
	LONG		GetIndex ( double x, double y, double z, double u, double v );
	LONG		GetCount ( );
	HRESULT		GetVertex( ULONG index, uvvec* );
	void		SetCompress ( BOOL in_bCompress ) { m_bCompress = in_bCompress; };
private:
	C3DVectorMap	m_VectorMap;
	C3DVectorIndex	m_VectorIndex;
	BOOL			m_bCompress;
};

#endif
