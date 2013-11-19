/*===========================================================================
  xmlpool.c
    Based heavily on alloc-pool.c in gcc (some old code)
===========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xmlpool.h"
#include "xmlcfg.h"

#define ALIGN_FOUR(n) (((n+3) >> 2) << 2)
#define ALIGN_EIGHT(n) (((n+7) >> 3) << 3)

LPXMLPOOL XMLAPI XMLPool_Create(int itemSize, int itemsPerBlock)
{
	LPXMLPOOL pool;
	int hdrSize;

	if (itemSize < (int)sizeof(LPXMLPOOLLIST))
		itemSize = sizeof(LPXMLPOOLLIST);

	/* make item size to a multiple of 4.  */
	itemSize = ALIGN_FOUR(itemSize);
	pool = (LPXMLPOOL)malloc(sizeof(struct tagLPXMLPOOL));
	if (!pool) return (LPXMLPOOL)NULL;

	pool->itemSize = itemSize;
	pool->itemsPerBlock = itemsPerBlock;

	/* make header size to a multiple of 8 */
	hdrSize = ALIGN_EIGHT(sizeof(struct tagLPXMLPOOLLIST));

	pool->blockSize = (itemSize * itemsPerBlock) + hdrSize;
	pool->itemsAllocated = pool->itemsFree = pool->blocksAllocated = 0;
	pool->freeList = pool->blockList = NULL;
	return (pool);
}

void XMLAPI XMLPool_FreePool(LPXMLPOOL pool)
{
	LPXMLPOOLLIST nextBlock;
	LPXMLPOOLLIST block = pool->blockList;
	
	while(block) {
		nextBlock = block->next;
		free(block);
		block = nextBlock;
	}
	free(pool);
}

void XMLAPI *XMLPool_Alloc(LPXMLPOOL pool)
{
	LPXMLPOOLLIST hdr;
	char *block;

	if (!pool->freeList) {
		int i;
		LPXMLPOOLLIST blockhdr;

		block = (char*)malloc(pool->blockSize);
		if (!block) return NULL;
		blockhdr = (LPXMLPOOLLIST) block;
		block += ALIGN_EIGHT(sizeof(struct tagLPXMLPOOLLIST));

		/* add block to the block list: */
		blockhdr->next = pool->blockList;
		pool->blockList = blockhdr;

		/* add new items to the free list: */
		for (i=0; i<pool->itemsPerBlock; i++, block+=pool->itemSize) {
			hdr = (LPXMLPOOLLIST)block;
			hdr->next = pool->freeList;
			pool->freeList = hdr;
		}
		
		pool->itemsAllocated += pool->itemsPerBlock;
		pool->itemsFree += pool->itemsPerBlock;
		pool->blocksAllocated++;
	}

	hdr = pool->freeList;
	pool->freeList = hdr->next;
	pool->itemsFree--;
	return((void*)hdr);
}

void XMLAPI XMLPool_Free(LPXMLPOOL pool, void *ptr)
{
	LPXMLPOOLLIST hdr = (LPXMLPOOLLIST)ptr;

	if (ptr && pool->itemsFree+1 > pool->itemsAllocated) return;
	hdr->next = pool->freeList;
	pool->freeList = hdr;
	pool->itemsFree++;
}


