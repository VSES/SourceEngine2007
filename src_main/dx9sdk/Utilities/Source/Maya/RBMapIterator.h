#pragma once
#ifndef RBMAPITERATOR_H
#define RBMAPITERATOR_H

template < typename RBMAP >
class CRBMapIterator
{
private:
	RBMAP* Map;
	POSITION Position;

public:
	typedef typename RBMAP::KINARGTYPE KINARGTYPE;
	typedef typename RBMAP::KOUTARGTYPE KOUTARGTYPE;
	typedef typename RBMAP::VOUTARGTYPE VOUTARGTYPE;


	CRBMapIterator()
	{
		Map= NULL;
		Position= NULL;
	}

	CRBMapIterator(RBMAP* map)
	{
		Map= map;
		First();
	}

	UINT Count()
	{
		if(Map)
			return Map->GetCount();

		return 0;
	}

	CRBMapIterator& First()
	{
		if(Map)
			Position= Map->GetHeadPosition();

		return *this;
	}

	CRBMapIterator& Next()
	{
		if(Map && Position)
			Map->GetNext(Position);

		return *this;
	}

	CRBMapIterator& Lookup(KINARGTYPE key)
	{
		if(Map)
			Position = Map->Lookup(key);

		return *this;
	}

	bool Done() 
	{ 
		return NULL == Position; 
	}

	bool GetKey(KOUTARGTYPE key)
	{
		if( Done() )
			return false;

		key= Map->GetKeyAt(Position);
		
		return true;
	}

	bool GetValue(VOUTARGTYPE value)
	{
		if( Done() )
			return false;

		value= Map->GetValueAt(Position);
		
		return true;
	}
};

#endif

