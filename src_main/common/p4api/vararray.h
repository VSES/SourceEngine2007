/*
 * Copyright 1995, 1996 Perforce Software.  All rights reserved.
 */

/*
 * VarArray.h - manage a list of void *'s
 *
 * Class Defined:
 *
 *	VarArray - list of void *'s
 *
 * Public methods:
 *
 *	VarArray::Clear() - zero out count for values
 *	VarArray::Count() - return count of elements in array
 *	VarArray::ElemTab() - return a pointer to the array
 *	VarArray::Get(i) - return a pointer to slot i in the array
 *	VarArray::Move(i,j) - move element i to j
 *	VarArray::Put(v) - set a new slot to v
 *	VarArray::WillGrow(i) - returns size of new vararray if it
 *	                        would have to grow in the next i rows.
 *
 * Private methods:
 *
 *	VarArray::New() - return a pointer to a new slot in the array
 */

class VarArray {
	
    public:

			VarArray();
			VarArray( int max );
			~VarArray();

	void		Clear() { numElems = 0; }
	int		Count() const { return numElems; }
	void **		ElemTab() { return elems; }
	void *		Get( int i ) const { return i<numElems ? elems[i]:0; }
	void 		Move( int i, int j ) { elems[j] = elems[i]; }
	void 		Exchange( int i, int j )
	    { void *t = elems[j]; elems[j] = elems[i]; elems[i] = t; }
	void *		Put( void *v ) { return *New() = v; }
	void		SetCount( int i ) { numElems = i; }
	int		WillGrow( int interval );

    private:
	void **		New();

	int		maxElems;
	int		numElems;
	void		**elems;
} ;

