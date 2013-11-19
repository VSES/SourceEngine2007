#ifndef _C3DVECTORPROCESSOR_H_
#define _C3DVECTORPROCESSOR_H_

#include <SIBCVector.h>
#include <map>
#include <vector>

typedef	const CSIBCVector3D & REFDOTXSIVECTOR3;

class C3DCompareVector
{
public:
	inline bool operator()( REFDOTXSIVECTOR3 in_vec1, REFDOTXSIVECTOR3 in_vec2) const 
	{
		if (in_vec1.GetX() != in_vec2.GetX()) return in_vec1.GetX() < in_vec2.GetX();
		if (in_vec1.GetY() != in_vec2.GetY()) return in_vec1.GetY() < in_vec2.GetY();
		return in_vec1.GetZ() < in_vec2.GetZ();
	}
} ;

typedef std::map<CSIBCVector3D, ULONG, C3DCompareVector> C3DVectorMap;
typedef C3DVectorMap::iterator                           C3DVectorMapIterator;

typedef std::vector<CSIBCVector3D>						 C3DVectorIndex;
typedef	C3DVectorIndex::iterator						 C3DVectorIndexIterator;


class C3DVectorCompressor
{
public:
	C3DVectorCompressor() { m_bCompress = TRUE; };
	~C3DVectorCompressor();
	HRESULT		AddVertex( double x, double y, double z );
	LONG		GetIndex ( double x, double y, double z );
	LONG		GetCount ( );
	HRESULT		GetVertex( ULONG index, CSIBCVector3D* );
	void		SetCompress ( BOOL in_bCompress ) { m_bCompress = in_bCompress; };
private:
	C3DVectorMap	m_VectorMap;
	C3DVectorIndex	m_VectorIndex;
	BOOL			m_bCompress;
};

#endif
