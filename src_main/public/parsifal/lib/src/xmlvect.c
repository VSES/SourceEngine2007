/*===========================================================================
  xmlvect.c
    see parsifal.h for copyright info
===========================================================================*/

#include <stdlib.h>
#include <string.h> /* memmove */
#include "xmlvect.h"
#include "xmlcfg.h"

#define SETCAPACITY _SetCapacity
#define _SetCapacity(v,newsize) \
( (((v)->array = (BYTE*)realloc((v)->array, \
((newsize) * (v)->itemSize))) == (BYTE*)NULL) ? (LPXMLVECTOR)NULL : ((v)->capacity=(newsize), (v)) )
/*static LPXMLVECTOR SetCapacity(LPXMLVECTOR vector, int newsize);*/

/*
	Creates new vector. initialCapacity 0 sets capacity to
	default capacityIncrement which is 6.
	If you want to change capacityIncrement to 2 for example call:
	LPXMLVECTOR v = XMLVector_Create(&v, 2, sizeof(int)); 
	and after that set 
	v->capacityIncrement = 2;

	NOTE: All indices are 0-based.
*/
LPXMLVECTOR XMLAPI XMLVector_Create(LPXMLVECTOR *vector, int initialCapacity, int itemSize)
{	
	if (((LPXMLVECTOR)*vector = (LPXMLVECTOR)malloc(sizeof(XMLVECTOR))) == NULL) 
		return (LPXMLVECTOR)NULL;
	
	((LPXMLVECTOR)*vector)->length = 0;
	((LPXMLVECTOR)*vector)->capacityIncrement = 6; 
	((LPXMLVECTOR)*vector)->array = (BYTE*)NULL;
	((LPXMLVECTOR)*vector)->itemSize = itemSize;
	return SETCAPACITY((LPXMLVECTOR)*vector, 
		((initialCapacity) ? initialCapacity : ((LPXMLVECTOR)*vector)->capacityIncrement));
}

/*
	Frees the vector and data associated with it.
*/
void XMLAPI XMLVector_Free(LPXMLVECTOR vector) 
{
	if (vector->array != (BYTE*)NULL) free(vector->array);
	free(vector);
}

/*
	Resizes vector. Can be used to truncate Vector i.e. to remove
	items from the end of the vector or to clear the whole thing,
	in this case works as XMLVector_RemoveAll macro.
	If you set newsize to 0, capacity will be set default 6. 
	XMLVector_Resize Can be used to ensure capacity of newsize too (capacity
	will be set nearest blocksize (capacityIncrement) that can
	hold newsize num of items)
	returns 1 if succesful, 0 otherwise.
*/
int XMLAPI XMLVector_Resize(LPXMLVECTOR vector, int newsize)
{	
	int newCapacity;

	if (!newsize) {
		if (vector->capacity > vector->capacityIncrement) {
			if(!(vector = SETCAPACITY(vector, vector->capacityIncrement))) return 0;
		}
		vector->length = 0;
		return 1;
	}

	newCapacity = COUNTBUFSIZE(newsize, vector->capacityIncrement);
	if (newCapacity != vector->capacity) {		
		if(!(vector = SETCAPACITY(vector, newCapacity))) return 0;
	}
	
	if (vector->length > newsize) vector->length = newsize;
	return 1;
}

void XMLAPI *XMLVector_Get(LPXMLVECTOR vector, int index)
{
	return(_XMLVector_Get(vector, index));
}

/*
	Appends item at the end of the vector
	returns pointer to current item if succesful, NULL otherwise.
*/
void XMLAPI *XMLVector_Append(LPXMLVECTOR vector, void *item)
{	
	void *cur;
	if ((vector->length + 1) > vector->capacity) {
		vector->capacity = COUNTBUFSIZE(vector->length + 1, 
			vector->capacityIncrement);
		if(!(vector = SETCAPACITY(vector, vector->capacity))) return NULL;
	}
	cur = vector->array+vector->length*vector->itemSize;
	if (item != NULL) memcpy(cur, item, vector->itemSize);
	vector->length++;
	return(cur);
}

/*
	Inserts item before specified index.
	returns pointer to current item if succesful, NULL otherwise.
*/
void XMLAPI *XMLVector_InsertBefore(LPXMLVECTOR vector, int index, void *item)
{	
	void *cur;
	if (!index && !vector->length) return XMLVector_Append(vector, item);
	if (index < 0 || index > (vector->length - 1)) return NULL;
	
	if ((vector->length + 1) > vector->capacity) {
		vector->capacity = COUNTBUFSIZE(vector->length + 1, 
			vector->capacityIncrement);
		if(!(vector = SETCAPACITY(vector, vector->capacity))) return NULL;
	}
	
	vector->length++;
	cur = vector->array+index*vector->itemSize;
	memmove(vector->array+((index+1)*vector->itemSize), 
		cur, (vector->length - index) * vector->itemSize);
	if (item != NULL) memcpy(cur, item, vector->itemSize);
	return(cur);
}

/* 
	Replaces item at specified index.
	returns pointer to current item if succesful, NULL otherwise.
*/
void XMLAPI *XMLVector_Replace(LPXMLVECTOR vector, int index, void *item)
{
	if (index < 0 || index > (vector->length - 1)) {
		return NULL;
	}
	else {
		void *cur;
		cur = vector->array+index*vector->itemSize;
		if (item != NULL) memcpy(cur, item, vector->itemSize);
		return(cur);
	}
}

/*
	Removes item at specified index.
	returns 1 if succesful, 0 otherwise.
*/
int XMLAPI XMLVector_Remove(LPXMLVECTOR vector, int index)
{	
	int newCapacity;
	if (index < 0 || index > (vector->length - 1)) return 0;
	
	if (index != (--vector->length)) {
		memmove(vector->array+index*vector->itemSize, 
			vector->array+((index+1)*vector->itemSize), 
				(vector->length - index) * vector->itemSize);
	}
	
	newCapacity = COUNTBUFSIZE(vector->length, vector->capacityIncrement);
	if (newCapacity != vector->capacity) {
		vector->capacity = newCapacity;		
		if(!(vector = SETCAPACITY(vector, vector->capacity))) return 0;
	}
	return 1;
}

/*static 
LPXMLVECTOR SetCapacity(LPXMLVECTOR vector, int newsize)
{
	if ((vector->array = (BYTE*)realloc(vector->array, 
		(newsize * vector->itemSize))) == (BYTE*)NULL) return NULL;
	vector->capacity = newsize;
	return vector;			
}*/



