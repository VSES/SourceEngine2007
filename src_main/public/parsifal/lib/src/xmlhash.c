/*
** public domain code by Jerry Coffin, with improvements by HenkJan Wolthuis.
**
** Tested with Visual C 1.0 and Borland C 3.1.
** Compiles without warnings, and seems like it should be pretty
** portable.
*/
/*  
    Modifications by T. Uusitalo:

    +   Hash algorithm replaced by new one (stolen from somewhere, can't remember...)
    +   All names of "global" functions changed 
        to more appropriate
        XMLHTable_<name of method> form + parameter
        order changed to more logical form.
    +   Bug fixed in _Enumerate (see comment in _Enumerate)
    +   got rid of global variables to make code thread safe.
    +   XMLHTable_Destroy uses optional enumeration function
        for freeing user memory
    Still, most CREDITS GO TO COFFIN... ;)
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "xmlhash.h"
#include "xmlcfg.h" /* for xmlMemdup */
#define cHT_Ptr table->bucket

static unsigned char rand8[] = {
     41, 35,190,132,225,108,214,174,
     82,144, 73,241,187,233,235,179,
    166,219, 60,135, 12, 62,153, 36,
     94, 13, 28,  6,183, 71,222, 18,
     77,200, 67,139, 31,  3, 90,125,
      9, 56, 37, 93,212,203,252,150,
    245, 69, 59, 19,137, 10, 50, 32,
    154, 80,238, 64,120, 54,253,246,
    158,220,173, 79, 20,242, 68,102,
    208,107,196, 48,161, 34,145,157,
    218,176,202,  2,185,114, 44,128,
    126,197,213,178,234,201,204, 83,
    191,103, 45,142,131,239, 87, 97,
    255,105,143,205,209, 30,156, 22,
    230, 29,240, 74,119,215,232, 57,
     51,116,244,159,164, 89, 53,207,
    211, 72,117,217, 42,229,192,247,
     43,129, 14, 95,  0,141,123,  5,
     21,  7,130, 24,112,146,100, 84,
    206,177,133,248, 70,106,  4,115,
     47,104,118,250, 17,136,121,254,
    216, 40, 11, 96, 61,151, 39,138,
    194,  8,165,193,140,169,149,155,
    168,167,134,181,231, 85, 78,113,
    226,180,101,122, 99, 38,223,109,
     98,224, 52, 63,227, 65, 15, 27,
    243,160,127,170, 91,184, 58, 16,
     76,236, 49, 66,124,228, 33,147,
    175,111,  1, 23, 86,198,249, 55,
    189,110, 92,195,163,152,199,182,
     81, 25, 46,188,148, 75, 88,210,
    172, 26,162,237,251,221,186,171};

/*
** Hashes a string to produce an unsigned short, which should be
** sufficient for most purposes.
*/

static unsigned hash(char *str, size_t n, size_t *keylen) 
{
    if (n < 256) {
        unsigned char h = 0;
		char *key = str;
        while (*str) { h = rand8[h ^ *str]; str++; }
		if (keylen) *keylen = str - key;
        return h % n;
    } else {
        int h;
        unsigned char h1, h2;
        
        if (*str == 0) return 0;
        h1 = *str; h2 = *str + 1;
        str++;
        while (*str) {
            h1 = rand8[h1 ^ *str];
            h2 = rand8[h2 ^ *str];
            str++;
        }

        /* h is in range 0..65535 */
		h = ((int)h1 << 8)|(int)h2;
        /* use division method to scale */
        return h % n;
	}
}

/* modified 072002 end */

/* Initialize the XMLHTABLE to the size asked for.  Allocates space
** for the correct number of pointers and sets them to NULL.  If it
** can't allocate sufficient memory, signals error by setting the size
** of the table to 0.
*/

LPXMLHTABLE XMLAPI XMLHTable_Create(LPXMLHTABLE table, size_t size)
{
      size_t i;
	  table = (LPXMLHTABLE)malloc(sizeof(XMLHTABLE));	
	  if (!table) return table;

      table -> size  = size;
      table -> table = (XMLHTABLEBUCKET * *)malloc(sizeof(XMLHTABLEBUCKET *) * size);

      if (!table -> table) {
         free(table);
         return (LPXMLHTABLE)NULL;
      }

      for (i=0;i<size;i++)
            (table->table)[i] = NULL;
      return table;
}

/*
** Insert 'key' into hash table.
** Returns pointer to old data associated with the key, if any, or
** NULL if the key wasn't in the table previously.
*/

void XMLAPI *XMLHTable_Insert(LPXMLHTABLE table, char *key, void *data)
{
	  size_t keylen;
      unsigned val = hash(key, table->size, &keylen);
      /*
      ** NULL means this XMLHTABLEBUCKET hasn't been used yet.  We'll simply
      ** allocate space for our new XMLHTABLEBUCKET and put our data there, with
      ** the table pointing at it.
      */

      if (NULL == (table->table)[val])
      {
            (table->table)[val] = (XMLHTABLEBUCKET *)malloc(sizeof(XMLHTABLEBUCKET));
            cHT_Ptr = (table->table)[val];
			if (NULL==cHT_Ptr)
                  return NULL;

            cHT_Ptr -> key = xmlMemdup(key, keylen+1);
			if (!cHT_Ptr -> key) {
				free(cHT_Ptr);
				return NULL;
			}
            cHT_Ptr -> next = NULL;
            cHT_Ptr -> data = data;
            return cHT_Ptr -> data;
      }

      /*
      ** This spot in the table is already in use.  See if the current string
      ** has already been inserted, and if so, increment its count.
      */

      for (cHT_Ptr = (table->table)[val];NULL != cHT_Ptr; cHT_Ptr = cHT_Ptr -> next)
            if (0 == strcmp(key, cHT_Ptr->key))
            {
                  void *old_data;

                  old_data = cHT_Ptr->data;
                  cHT_Ptr -> data = data;
                  return old_data;
            }

      /*
      ** This key must not be in the table yet.  We'll add it to the head of
      ** the list at this spot in the hash table.  Speed would be
      ** slightly improved if the list was kept sorted instead.  In this case,
      ** this code would be moved into the loop above, and the insertion would
      ** take place as soon as it was determined that the present key in the
      ** list was larger than this one.
      */

      cHT_Ptr = (XMLHTABLEBUCKET *)malloc(sizeof(XMLHTABLEBUCKET));
      if (NULL==cHT_Ptr) return NULL;
      cHT_Ptr -> key = xmlMemdup(key, keylen+1);
	  if (!cHT_Ptr -> key) {
			free(cHT_Ptr);
			return NULL;
	  }
      cHT_Ptr -> data = data;
      cHT_Ptr -> next = (table->table)[val];
      (table->table)[val] = cHT_Ptr;
      return data;
}


/*
** Look up a key and return the associated data.  Returns NULL if
** the key is not in the table.
*/

void XMLAPI *XMLHTable_Lookup(LPXMLHTABLE table, char *key)
{
      unsigned val = hash(key, table->size, (size_t)NULL);

      if (NULL == (table->table)[val])
            return NULL;

      for (cHT_Ptr = (table->table)[val];NULL != cHT_Ptr; cHT_Ptr = cHT_Ptr->next )
      {
            if (0 == strcmp(key, cHT_Ptr -> key ) )
                  return cHT_Ptr->data;
      }
      return NULL;
}

/*
** Delete a key from the hash table and return associated
** data, or NULL if not present.
*/

void XMLAPI *XMLHTable_Remove(LPXMLHTABLE table, char *key)
{
      unsigned val = hash(key, table->size, (size_t)NULL);
      void *data;
      XMLHTABLEBUCKET *ptr, *last = NULL;

      if (NULL == (table->table)[val])
            return NULL;

      /*
      ** Traverse the list, keeping track of the previous node in the list.
      ** When we find the node to delete, we set the previous node's next
      ** pointer to point to the node after ourself instead.  We then delete
      ** the key from the present node, and return a pointer to the data it
      ** contains.
      */

      for (last = NULL, ptr = (table->table)[val];
            NULL != ptr;
            last = ptr, ptr = ptr->next)
      {
            if (0 == strcmp(key, ptr -> key))
            {
                  if (last != NULL )
                  {
                        data = ptr -> data;
                        last -> next = ptr -> next;
                        free(ptr->key);
                        free(ptr);
                        return data;
                  }

                  /*
                  ** If 'last' still equals NULL, it means that we need to
                  ** delete the first node in the list. This simply consists
                  ** of putting our own 'next' pointer in the array holding
                  ** the head of the list.  We then dispose of the current
                  ** node as above.
                  */

                  else
                  {
                        data = ptr->data;
                        (table->table)[val] = ptr->next;
                        free(ptr->key);
                        free(ptr);
                        return data;
                  }
            }
      }

      /*
      ** If we get here, it means we didn't find the item in the table.
      ** Signal this by returning NULL.
      */

      return NULL;
}

/*
** free_table iterates the table, calling this repeatedly to free
** each individual node. 
*/

static int free_node(char *key, void *data, void *table)
{
     XMLHTable_Remove((LPXMLHTABLE)table,key);
	 return 0;
}

/*
** Frees a complete table by iterating over it and freeing each node.
** the second parameter is the address of a function similar to
** function param to _Enumerate (with exception of last param
** being the table itself), when spesified, function is 
** responsible for freeing the item in table. For example the
** following function removes item from table and frees char data
** associated with it:

	void destroyTableProc(char *key, void *data, LPXMLHTABLE table)
	{
		XMLHTable_Remove(table, key);
		if (data!=(char*)NULL) free(data);
	}

**	if func param isn't spesified, _Destroy frees the table items
**	which is suitable in situations where data isn't dynamically
**	allocated and thus can't be freed.
*/

int XMLAPI XMLHTable_Destroy(LPXMLHTABLE table, int (*func)(char *, void *, void *), int FreeTable)
{	
	int ret;
	table->userdata = table; /* can use userdata, since it will be trashed anyway... */

	if (!func) 
		ret = XMLHTable_Enumerate(table, free_node);		
	else 
		ret = XMLHTable_Enumerate(table, func);
	
	if (!ret && FreeTable) {
		free(table->table);
		free(table);
	}
	return ret;
}

/*
** Simply invokes the function given as the second parameter for each
** node in the table, passing it the key and the associated data +
** userdata (can be pointer to destination table if Enumerate is used
** for copying hashtable for example).
*/

int XMLAPI XMLHTable_Enumerate(LPXMLHTABLE table, int (*func)(char *, void *, void *))
{
      unsigned i, ret;
      XMLHTABLEBUCKET *temp, *next;

      for (i=0;i<table->size; i++)
      {
            if ((table->table)[i] != NULL)
            {
                  for (temp = (table->table)[i];
                        NULL != temp;
                        /*temp = temp -> next 
						bug fix: have to save <temp->next>
						to <next> since it might get destroyed!
						*/
						temp = next)
                  {
						next = temp->next;
                        if ((ret = func(temp->key, (void*)temp->data, table->userdata))) return ret;
                  }
            }
      }
	  return 0;
}

#undef cHT_Ptr

